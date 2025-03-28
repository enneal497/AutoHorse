#include "InputEventHandler.h"
#include "MarkerHandler.h"
#include "Settings.h"
#include "Utility.h"

namespace AutoHorse {

    InputEventHandler* InputEventHandler::GetSingleton() {
        static InputEventHandler singleton;
        return std::addressof(singleton);
    }

    void InputEventHandler::Register() {
        logger::info("Initializing InputEventHandler event sink...");
        const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
        if (inputDeviceManager) {
            inputDeviceManager->AddEventSink(GetSingleton());
            logger::info("InputEventHandler event sink initialized.");

            g_isReady = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::g_isReadyID, Settings::espName);
            g_tutorial = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::g_tutorialID, Settings::espName);
            g_speed = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::g_speedID, Settings::espName);
            controlQuest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(Settings::questID, Settings::espName);

        }
        else {
            stl::report_and_fail("Failed to initialize InputEventHandler event sink.");
        }
    }

    void InputEventHandler::StartAutopilot() {
        if (!MarkerHandler::GetMarker()) {
            return;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        auto mount = InputEventHandler::GetSingleton()->mount;

        if (!isPaused) {
            isWalking = false;
            g_tutorial->value = 1.0f;
        }
        if (RE::PlayerControls::GetSingleton()->data.autoMove) {
            RE::PlayerControls::GetSingleton()->data.autoMove = false;
        }

        g_speed->value = static_cast<float>(2);
        controlQuest->Start();
        Utility::ForceMountToAlias(mount);

        g_isReady->value = 1.0f;
        mount.get()->SetPlayerControls(false);
        player->SetAIDriven(true);
        mount.get()->EvaluatePackage(true, false);

        
    }

    void InputEventHandler::ForceStopAutopilot() {
        if (!isPaused && g_isReady->value == 0.0f) { return; }
        isPaused = false;
        isActive = false;
        StopAutopilot(false);
    }

    void InputEventHandler::StopAutopilot(bool dismount) {
        if (controlQuest->IsRunning()) {
            auto* player = RE::PlayerCharacter::GetSingleton();
            auto mount = InputEventHandler::GetSingleton()->mount;

            if (!isPaused) {
                player->GetPlayerRuntimeData().playerFlags.isSprinting = isSprinting;
            }

            g_isReady->value = 0.0f;
            mount->SetPlayerControls(true);
            mount->EnableAI(true);
            mount->EvaluatePackage();
            player->SetAIDriven(false);

            if (dismount) {
                mount->ActivateRef(player->AsReference(), 0, nullptr, 0, true);
            }

            controlQuest->Stop();

            mount.reset();
        }
    }

    //Is player currently under AI control
    bool InputEventHandler::IsRunning() {
        return (isActive && InputEventHandler::GetSingleton()->mount);
    }
    
    RE::BSEventNotifyControl InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {

        UNREFERENCED_PARAMETER(a_eventSource);

        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }
        auto player = RE::PlayerCharacter::GetSingleton();
        auto UI = RE::UI::GetSingleton();

        if (!player || !player->Is3DLoaded() || UI->GameIsPaused() || UI->IsMenuOpen(RE::MapMenu::MENU_NAME)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (a_event[0]) {
            Settings::GetMappedControls(a_event[0]->GetDevice());
        }

        mount.reset();
        if (!player->GetMount(mount)) {
            if (isActive) { ForceStopAutopilot(); }
            return RE::BSEventNotifyControl::kContinue;
        }

        for (auto event = *a_event; event; event = event->next) {
            if (const auto button = event->AsButtonEvent(); button) {
                const auto device = event->GetDevice();

                Settings::GetMappedControls(device);
                auto key = button->GetIDCode();
                bool isPressed = button->IsPressed();
                bool isHeld = button->IsHeld();

                //If key is not pressed for the first time
                if (isHeld == true) {
                    continue;
                }

                if (key == Settings::ReturnControls(KeyType::Start)) {
                    // Start autopilot
                    if (isPressed == true) {
                        if (isActive) {
                            ForceStopAutopilot();
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        if (player->GetCurrentPackage()) {
                            //Player under AI control
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        if (player->GetPlayerRuntimeData().playerFlags.isSprinting) {
                            //Player sprinting
                            return RE::BSEventNotifyControl::kContinue;
                        }

                        StartAutopilot();
                        isPaused = false;
                        isActive = true;
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
                else if (key == Settings::ReturnControls(KeyType::Activate)) {
                    // Stop autopilot and dismount
                    if (isPressed && isActive) {
                        logger::info("Stop autopilot and dismount");
                        StopAutopilot(true);
                        isPaused = false;
                        isActive = false;
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
                else if (key == Settings::ReturnControls(KeyType::Sprint)) {
                    //Toggle sprint
                    if (isPressed && !isWalking) {
                        isSprinting = !isSprinting;
                        g_speed->value = static_cast<float>((isSprinting) ? 3 : 2);
                        mount.get()->EvaluatePackage(true, false);
                        player->EvaluatePackage(true, false);
                        
                        continue;
                    }
                }

                //Get control map and compare against keypress event
                if (device == RE::INPUT_DEVICE::kKeyboard) {

                    if (key == Settings::ReturnControls(KeyType::Forward) || key == Settings::ReturnControls(KeyType::Back)) {
                        // Stop autopilot
                        if (isPressed && isActive) {
                            ForceStopAutopilot();
                            return RE::BSEventNotifyControl::kContinue;
                        }
                    }
                    else if (key == Settings::ReturnControls(KeyType::Left) || key == Settings::ReturnControls(KeyType::Right)) {
                        if (!isActive) {
                            continue;
                        }
                        if (isPressed) {
                            // Pause autopilot
                            isPaused = true;
                            StopAutopilot(false);
                            continue;
                        }
                        else if (isPaused) {
                            //Resume autopilot
                            StartAutopilot();
                            isPaused = false;
                            continue;
                        }
                    }
                    else if (key == Settings::ReturnControls(KeyType::Walk)) {
                        //Toggle walk
                        if (isPressed && !isSprinting) {
                            isWalking = !isWalking;
                            g_speed->value = static_cast<float>((isWalking) ? 1 : 2);
                            mount.get()->EvaluatePackage(true, false);
                            player->EvaluatePackage(true, false);
                            continue;
                        }
                    }
                    else if (key == Settings::ReturnControls(KeyType::Shift)) {
                        //Hold walk
                        if (!isSprinting) {
                            isWalking = !isWalking;
                            g_speed->value = static_cast<float>((isWalking) ? 1 : 2);
                            mount.get()->EvaluatePackage(true, false);
                            player->EvaluatePackage(true, false);
                            continue;
                        }
                    }
                    else if (key == Settings::ReturnControls(KeyType::AutoMoveKey)) {
                        //Switch off autopilot
                        if (isPressed && isActive) {
                            ForceStopAutopilot();
                            return RE::BSEventNotifyControl::kStop;
                        }
                    }
                }
            }
            else if (const auto thumbstick = event->AsThumbstickEvent(); thumbstick) {
                if (thumbstick->IsMainHand() || !isActive) {
                    continue;
                }

                if (RE::BSInputDeviceManager::GetSingleton()->GetGamepad()->IsPressed(Settings::ReturnControls(KeyType::CameraMod))) {
                    return RE::BSEventNotifyControl::kContinue;
                }

                auto x = thumbstick->xValue;
                auto y = thumbstick->yValue;

                if (abs(y) > Settings::thumbstickThreshold) {
                    // Stop autopilot
                    ForceStopAutopilot();
                    continue;
                }
                if (abs(x) > Settings::thumbstickThreshold) {
                    // Pause autopilot
                    isPaused = true;
                    StopAutopilot(false);
                    continue;
                }
                else if (isPaused) {
                    //Resume autopilot
                    StartAutopilot();
                    isPaused = false;
                    continue;
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}

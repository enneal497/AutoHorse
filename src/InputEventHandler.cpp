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

            g_tutorial = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::g_tutorialID, Settings::espName);
            g_dismount = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::g_dismountID, Settings::espName);
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
        if (!isPaused) {
            isSprinting = false;
            isWalking = false;
            g_speed->value = static_cast<float>(2);
            g_tutorial->value = static_cast<float>(1);
        }
        controlQuest->Start();
    }

    void InputEventHandler::ForceStopAutopilot() {
        logger::info("Stop autopilot");
        isPaused = false;
        isActive = false;
        StopAutopilot(false, InputEventHandler::GetSingleton()->mount);
    }

    void InputEventHandler::StopAutopilot(bool dismount, RE::ActorPtr mount) {
        if (controlQuest->IsRunning()) {
            g_dismount->value = static_cast<float>(dismount);
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

        if (mount && mount.get()->IsDead() && isActive) {
            ForceStopAutopilot();
        }

        if (a_event[0]) {
            Settings::GetMappedControls(a_event[0]->GetDevice());
        }

        if (!(player->GetMount(mount))) {
            //logger::info("Not on mount");
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

                //auto keyname = SKSE::InputMap::GetKeyName(key);
                //logger::info("key {}", keyname);
                //logger::info("key DXScanCode: {}", key);
                //logger::info("Expected Start Key: {}", Settings::ReturnControls(KeyType::Start));

                if (key == Settings::ReturnControls(KeyType::Start)) {
                    // Start autopilot
                    if (isPressed == true) {
                        if (isActive) {
                            ForceStopAutopilot();
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        if (player->GetCurrentPackage()) {
                            logger::info("Player under AI control");
                            //Player under AI control
                            return RE::BSEventNotifyControl::kContinue;
                        }
                        
                        logger::info("Start autopilot");
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
                        StopAutopilot(true, mount);
                        isPaused = false;
                        isActive = false;
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
                else if (key == Settings::ReturnControls(KeyType::Sprint)) {
                    //Toggle sprint
                    if (isPressed && !isWalking) {
                        isSprinting = !isSprinting;
                        logger::info("Start sprinting? {}", isSprinting);
                        g_speed->value = static_cast<float>((isSprinting) ? 3 : 2);
                        mount.get()->EvaluatePackage(true, false);
                        player->EvaluatePackage(true, false);
                        continue;
                    }
                }

                //Get control map and compare against keypress event
                if (device == RE::INPUT_DEVICE::kKeyboard) {

                    if (key == Settings::ReturnControls(KeyType::Forward)) {
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
                            logger::info("Pause autopilot");
                            StopAutopilot(false, mount);
                            isPaused = true;
                            continue;
                        }
                        else if (isPaused) {
                            //Resume autopilot
                            logger::info("Resume autopilot");
                            StartAutopilot();
                            isPaused = false;
                            continue;
                        }
                    }
                    else if (key == Settings::ReturnControls(KeyType::Walk)) {
                        //Toggle walk
                        logger::info("trying to toggle walk");
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
                        logger::info("trying to hold walk");
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
                    logger::info("Stop autopilot");
                    StopAutopilot(false, mount);
                    isPaused = false;
                    isActive = false;
                    continue;
                }
                if (abs(x) > Settings::thumbstickThreshold) {
                    // Pause autopilot
                    logger::info("Pause autopilot");
                    StopAutopilot(false, mount);
                    isPaused = true;
                    continue;
                }
                else if (isPaused) {
                    //Resume autopilot
                    logger::info("Resume autopilot");
                    StartAutopilot();
                    isPaused = false;
                    continue;
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}

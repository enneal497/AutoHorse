#include "InputEventHandler.h"
#include "MarkerHandler.h"
#include "Settings.h"

namespace AutoHorse {

    InputEventHandler* InputEventHandler::GetSingleton() {
        static InputEventHandler singleton;
        return std::addressof(singleton);
    }

    void InputEventHandler::Register() {
        logger::info("Initializing event sink...");
        const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
        if (inputDeviceManager) {
            inputDeviceManager->AddEventSink(GetSingleton());
            logger::info("Event sink initialized.");

            isReady = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::globalID, Settings::espName);
            controlQuest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(Settings::questID, Settings::espName);

        }
        else {
            stl::report_and_fail("Failed to initialize event sink.");
        }
    }

    //NOT CURRENTLY IN USE
    /*
    bool InputEventHandler::SetStage(RE::TESQuest* a_quest, std::uint16_t a_stage)
    {
        using func_t = decltype(&SetStage);
        REL::Relocation<func_t> func{ RELOCATION_ID(24482, 25004) };
        return func(a_quest, a_stage);
    }
    */

    void InputEventHandler::StartAutopilot() {
        bool markerReady = MarkerHandler::GetMarker();
        if (markerReady) {
            controlQuest->Start();
            //SetStage(controlQuest, 0);
        }

    }

    void InputEventHandler::StopAutopilot(bool dismount, RE::ActorPtr mount) {
        if (controlQuest->IsRunning()) {
            controlQuest->Stop();
            if (dismount) {
                //Dismount horse
                const auto player = RE::PlayerCharacter::GetSingleton();
                if (!player || !player->Is3DLoaded()) {
                    return;
                }

                mount->ActivateRef(player, 0, mount->GetBaseObject(), 1, false);
            }
        }
    }
    
    RE::BSEventNotifyControl InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {

        UNREFERENCED_PARAMETER(a_eventSource);

        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->Is3DLoaded()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        RE::NiPointer<RE::Actor> mount;
        if (!(player->GetMount(mount))) {
            //logger::info("Not on mount");
            return RE::BSEventNotifyControl::kContinue;
        }


        for (auto event = *a_event; event; event = event->next) {
            if (const auto button = event->AsButtonEvent(); button) {
                const auto device = event->GetDevice();

                auto key = button->GetIDCode();
                bool isPressed = button->IsPressed();
                bool isHeld = button->IsHeld();

                //If key is not pressed for the first time
                if (isHeld == true) {
                    continue;
                }
                if (isPressed == false) {
                    //Resume autopilot
                }

                //Get control map and compare against keypress event
                Settings::GetMappedControls(device);
                auto keyname = SKSE::InputMap::GetKeyName(key);

                if (device == RE::INPUT_DEVICE::kKeyboard) {
                    logger::info("key {}", keyname);

                    if (key == Settings::ReturnControls(KeyType::Start)) {
                        // Start autopilot
                        logger::info("Start autopilot");
                        StartAutopilot();

                    }
                    else if (key == Settings::ReturnControls(KeyType::Activate)) {
                        // Stop autopilot and dismount
                        logger::info("Stop autopilot and dismount");
                        StopAutopilot(true, mount);

                    }
                    else if (key == Settings::ReturnControls(KeyType::Forward)) {
                        // Stop autopilot
                        logger::info("Stop autopilot");
                        StopAutopilot(false, mount);

                    }
                    else if (key == Settings::ReturnControls(KeyType::Left) || key == Settings::ReturnControls(KeyType::Right)) {
                        // Pause autopilot
                        logger::info("Pause autopilot");

                    }

                }

            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}

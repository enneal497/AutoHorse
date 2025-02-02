#include "InputEventHandler.h"
#include "QuestHandler.h"
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
        }
        else {
            stl::report_and_fail("Failed to initialize event sink.");
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

                    if (key == Settings::ReturnControls(KeyType::Activate)) {
                        // Start autopilot
                        logger::info("Start autopilot");
                        //QuestHandler::GetMarker();

                    } else if (key == Settings::ReturnControls(KeyType::Dismount)) {
                        // Stop autopilot and dismount
                        logger::info("Stop autopilot and dismount");

                    } else if (key == Settings::ReturnControls(KeyType::Forward) || key == Settings::ReturnControls(KeyType::Back)) {
                        // Stop autopilot
                        logger::info("Stop autopilot");

                    } else if (key == Settings::ReturnControls(KeyType::Left) || key == Settings::ReturnControls(KeyType::Right)) {
                        // Pause autopilot
                        logger::info("Pause autopilot");

                    }
                    
                }

            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}

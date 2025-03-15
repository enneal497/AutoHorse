#include "InputEventHandler.h"
#include "MarkerHandler.h"
#include "Settings.h"
#include "Utility.h"
#include "Events.h"
#include "Hooks.h"

//SKSE Messages
void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        Settings::LoadSettings();

        Hooks::Install();
        AutoHorse::InputEventHandler::Register();
        AutoHorse::ActivateEventHandler::Register();
        AutoHorse::CellEventHandler::Register();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    Init(skse);

    SKSE::AllocTrampoline(42);

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::init();

    logger::info("{} {} is loading...", name, version);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener)) {
        return false;
    }

    logger::info("{} has finished loading.", name);
    logger::info("");

    return true;
}

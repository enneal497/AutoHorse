#include "Events.h"
#include "Utility.h"

namespace AutoHorse
{
    RE::BSEventNotifyControl ActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) noexcept
    {
        UNREFERENCED_PARAMETER(a_eventSource);

        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }
        if (a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (a_event->objectActivated.get()->GetBaseObject()->HasKeywordByEditorID("ActorTypeHorse")) {
            logger::info("Player activated horse");
            Utility::ShowPrompt();
        }

        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events

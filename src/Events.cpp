#include "Events.h"
//#include "InputEventHandler.h"

namespace AutoHorse
{
    RE::BSEventNotifyControl CombatEventHandler::ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) noexcept
    {
        UNREFERENCED_PARAMETER(a_eventSource);

        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto InputSingleton = InputEventHandler::GetSingleton();
        if (!InputSingleton->IsRunning() || static_cast<RE::Actor*>(a_event->targetActor.get()) != InputSingleton->mount.get())
        {
            return RE::BSEventNotifyControl::kContinue;
        }

        logger::info("Target is player mount");
        InputEventHandler::ForceStopAutopilot();

        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events

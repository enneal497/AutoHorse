#pragma once
#include "InputEventHandler.h"

namespace AutoHorse
{
    class CombatEventHandler final : public EventHandler<CombatEventHandler, RE::TESCombatEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) noexcept override;
    };
} // namespace Events

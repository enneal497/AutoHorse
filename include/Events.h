#pragma once
#include "InputEventHandler.h"

namespace AutoHorse
{
    class ActivateEventHandler final : public EventHandler<ActivateEventHandler, RE::TESActivateEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) noexcept override;
    };

} // namespace Events

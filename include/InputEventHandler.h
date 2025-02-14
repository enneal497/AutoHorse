#pragma once

namespace AutoHorse {

    class InputEventHandler final : public RE::BSTEventSink<RE::InputEvent*> {
    public:

        static InputEventHandler* GetSingleton();

        static void Register();

        static bool IsRunning();
        static void ForceStopAutopilot();

        RE::NiPointer<RE::Actor> mount;

    private:
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

        InputEventHandler() = default;
        InputEventHandler(const InputEventHandler&) = delete;
        InputEventHandler(InputEventHandler&&) = delete;
        ~InputEventHandler() override = default;

        static void StartAutopilot();
        static void StopAutopilot(bool dismount, RE::ActorPtr mount);
        //static bool SetStage(RE::TESQuest *a_quest, std::uint16_t a_stage);

        static inline RE::TESQuest *controlQuest;
        static inline RE::TESGlobal *g_isReady;
        static inline RE::TESGlobal* g_dismount;

        static inline bool isPaused;
        static inline bool isActive;

        InputEventHandler& operator=(const InputEventHandler&) = delete;
        InputEventHandler& operator=(InputEventHandler&&) = delete;

    };

}

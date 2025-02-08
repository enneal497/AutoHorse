
namespace AutoHorse {

    class InputEventHandler final : public RE::BSTEventSink<RE::InputEvent*> {
    public:

        static InputEventHandler* GetSingleton();

        static void Register();

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
        static inline RE::TESGlobal *isReady;

        static inline bool isPaused;
        static inline bool isActive;

        InputEventHandler& operator=(const InputEventHandler&) = delete;
        InputEventHandler& operator=(InputEventHandler&&) = delete;

    };

}

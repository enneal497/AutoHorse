
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

        InputEventHandler& operator=(const InputEventHandler&) = delete;
        InputEventHandler& operator=(InputEventHandler&&) = delete;

    };

}

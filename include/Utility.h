#pragma once


namespace AutoHorse {
    class Utility
    {
    public:
        static void ShowPrompt();
        static void HidePrompt();

        static void RegisterSerialisation() noexcept;
        static void SaveCallback(SKSE::SerializationInterface* intfc) noexcept;
        static void LoadCallback(SKSE::SerializationInterface* intfc) noexcept;
    };
}

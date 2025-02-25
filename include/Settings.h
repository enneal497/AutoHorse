#pragma once

enum class KeyType
{
    Forward,
    Left,
    Right,
    Activate,
    Start,
    Sprint,
    Walk,
    Shift,
};

class Settings
{
public:
    static void LoadSettings() noexcept;
    static void GetMappedControls(RE::INPUT_DEVICE device);

    inline static bool debug_logging{};

    static uint32_t ReturnControls(KeyType keytype);

    inline static std::string espName{ "AutoHorse.esp" };
    inline static int questID{ 0x0800 };
    inline static int g_dismountID{ 0x0805 };
    inline static int g_speedID{ 0x0808 };
    inline static int g_tutorialID{ 0x809 };
    inline static int markerID{ 0x0801 };

    inline static float thumbstickThreshold{ 0.5 };

private:
    //Retrieve at runtime
    inline static uint32_t ForwardKey{ 17 };
    inline static uint32_t LeftKey{ 30 };
    inline static uint32_t RightKey{ 32 };
    inline static uint32_t ActivateKey{ 18 };
    //Speed
    inline static uint32_t SprintKey{ 56 };
    inline static uint32_t WalkKey{ 58 };
    inline static uint32_t ShiftKey{ 42 };

    //Retrieve from INI
    inline static uint32_t KStart{ 44 };
    inline static uint32_t GStart{ 1 };

    //Platform-dependent variables
    inline static uint32_t StartKey;

    static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
};

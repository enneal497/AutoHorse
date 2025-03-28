#pragma once

enum class KeyType
{
    Forward,
    Back,
    Left,
    Right,
    Activate,
    Start,
    Sprint,
    Walk,
    Shift,
    AutoMoveKey,
    CameraMod,
};

class Settings
{
public:
    static void LoadSettings() noexcept;
    static void GetMappedControls(const RE::INPUT_DEVICE &device);

    static uint32_t ReturnControls(KeyType keytype);

    inline static std::string espName{ "AutoHorse.esp" };
    inline static int questID{ 0x0800 };
    inline static int g_speedID{ 0x0808 };
    inline static int g_tutorialID{ 0x809 };
    inline static int g_isReadyID{ 0x803 };
    inline static int markerID{ 0x0801 };

    inline static float thumbstickThreshold{ 0.5 };
    inline static bool bShowTutorial{ true };
    inline static bool bDebugLogging{ false };

    inline static RE::INPUT_DEVICE cDevice;

private:
    //Retrieve at runtime
    inline static uint32_t ForwardKey{ 17 };
    inline static uint32_t BackKey{ 31 };
    inline static uint32_t LeftKey{ 30 };
    inline static uint32_t RightKey{ 32 };
    inline static uint32_t ActivateKey{ 18 };
    inline static uint32_t AutoMoveKey{ 46 };
    //Speed
    inline static uint32_t SprintKey{ 56 };
    inline static uint32_t WalkKey{ 58 };
    inline static uint32_t ShiftKey{ 42 };

    //Retrieve from INI
    inline static uint32_t KStart{ 44 };
    inline static uint32_t GStart{ 1 };
    inline static uint32_t GCameraMod{ 128 };

    //Platform-dependent variables
    inline static uint32_t StartKey;

    static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
    static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
    static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
};

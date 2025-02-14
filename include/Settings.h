#pragma once

enum class KeyType
{
    Forward,
    Left,
    Right,
    Activate,
    Start,
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
    inline static int markerID{ 0x0801 };

private:
    //Retrieve at runtime
    inline static uint32_t ForwardKey{ 17 };
    inline static uint32_t LeftKey{ 30 };
    inline static uint32_t RightKey{ 32 };
    inline static uint32_t ActivateKey{ 18 };

    //Retrieve from INI
    inline static uint32_t kStart{ 44 };

    //Platform-dependent variables
    inline static uint32_t StartKey;

    static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
};

#pragma once

enum class KeyType
{
    Forward,
    Back,
    Left,
    Right,
    Activate,
    Dismount,
};

class Settings
{
public:
    static void LoadSettings() noexcept;
    static void GetMappedControls(RE::INPUT_DEVICE device);

    inline static bool debug_logging{};

    inline static uint32_t kActivate{ 18 };
    inline static uint32_t kDismount{ 1 };

    static uint32_t ReturnControls(KeyType keytype);

private:
    inline static uint32_t ForwardKey{ 17 };
    inline static uint32_t BackKey{ 31 };
    inline static uint32_t LeftKey{ 30 };
    inline static uint32_t RightKey{ 32 };

    inline static uint32_t ActivateKey{ 18 };
    inline static uint32_t DismountKey{ 1 };

    static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
};

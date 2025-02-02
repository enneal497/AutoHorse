#include "Settings.h"

//Read setting and return as uint32_t
void Settings::ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
    const char* bFound = nullptr;
    bFound = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        a_setting = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
    }
}

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\AutoHorse.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    //Keyboard
    ReadUInt32Setting(ini, "Keyboard", "ActivateKey", (uint32_t&)kActivate);
    ReadUInt32Setting(ini, "Keyboard", "DismountKey", (uint32_t&)kDismount);

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    // Load settings

    logger::info("Loaded settings");
    logger::info("");
}

void Settings::GetMappedControls(RE::INPUT_DEVICE device)
{
    auto controlmap = RE::ControlMap::GetSingleton();

    ForwardKey = controlmap->GetMappedKey("Forward", device);
    BackKey = controlmap->GetMappedKey("Back", device);
    LeftKey = controlmap->GetMappedKey("Strafe Left", device);
    RightKey = controlmap->GetMappedKey("Strafe Right", device);

    //Change to allow for other devices
    ActivateKey = kActivate;
    DismountKey = kDismount;

}

uint32_t Settings::ReturnControls(KeyType keytype)
{
    switch (keytype) {
    case KeyType::Forward:
        return ForwardKey;
    case KeyType::Back:
        return BackKey;
    case KeyType::Left:
        return LeftKey;
    case KeyType::Right:
        return RightKey;
    case KeyType::Activate:
        return ActivateKey;
    case KeyType::Dismount:
        return DismountKey;
    default:
        return NULL;
    }
}

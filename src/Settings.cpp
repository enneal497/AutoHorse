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

    auto bShowTutorial = ini.GetBoolValue("General", "bShowTutorial");
    if (bShowTutorial == false) {
        RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(g_tutorialID, espName)->value = 1;
    }

    //Keyboard
    ReadUInt32Setting(ini, "Keyboard", "StartKey", (uint32_t&)kStart);

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
    LeftKey = controlmap->GetMappedKey("Strafe Left", device);
    RightKey = controlmap->GetMappedKey("Strafe Right", device);
    ActivateKey = controlmap->GetMappedKey("Activate", device);

    SprintKey = controlmap->GetMappedKey("Sprint", device);
    WalkKey = controlmap->GetMappedKey("Toggle Always Run", device);
    ShiftKey = controlmap->GetMappedKey("Run", device);

    //Change to allow for other devices
    StartKey = kStart;

}

uint32_t Settings::ReturnControls(KeyType keytype)
{
    switch (keytype) {
    case KeyType::Forward:
        return ForwardKey;
    case KeyType::Left:
        return LeftKey;
    case KeyType::Right:
        return RightKey;
    case KeyType::Activate:
        return ActivateKey;
    case KeyType::Start:
        return StartKey;
    case KeyType::Sprint:
        return SprintKey;
    case KeyType::Walk:
        return WalkKey;
    case KeyType::Shift:
        return ShiftKey;
    default:
        return NULL;
    }
}

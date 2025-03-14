#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\AutoHorse.ini)");

    ReadBoolSetting(ini, "General", "bDebug", (bool&)bDebugLogging);
    ReadBoolSetting(ini, "General", "bShowTutorial", (bool&)bShowTutorial);

    ReadUInt32Setting(ini, "Keyboard", "StartKey", (uint32_t&)KStart);
    ReadUInt32Setting(ini, "GamePad", "StartKey", (uint32_t&)GStart);
    ReadUInt32Setting(ini, "GamePad", "CameraModKey", (uint32_t&)GCameraMod);

    ReadFloatSetting(ini, "GamePad", "fThumbstickThreshold", (float&)thumbstickThreshold);

    Settings::GetMappedControls(RE::INPUT_DEVICE::kKeyboard);

    logger::info("bShowTutorial?: {}", bShowTutorial);
    if (bShowTutorial == false) {
        RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(g_tutorialID, espName)->value = 1;
    }    

    if (bDebugLogging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    logger::info("Loaded settings");
    logger::info("");
}

void Settings::GetMappedControls(const RE::INPUT_DEVICE &device)
{
    if (device != RE::INPUT_DEVICE::kKeyboard && device != RE::INPUT_DEVICE::kGamepad) {
        //;logger::info("Incompatible input device");
        return;
    }

    auto controlmap = RE::ControlMap::GetSingleton();

    ForwardKey = controlmap->GetMappedKey("Forward", device);
    LeftKey = controlmap->GetMappedKey("Strafe Left", device);
    RightKey = controlmap->GetMappedKey("Strafe Right", device);
    ActivateKey = controlmap->GetMappedKey("Activate", device);
    AutoMoveKey = controlmap->GetMappedKey("Auto-Move", device);

    SprintKey = controlmap->GetMappedKey("Sprint", device);
    WalkKey = controlmap->GetMappedKey("Toggle Always Run", device);
    ShiftKey = controlmap->GetMappedKey("Run", device);

    if (device == RE::INPUT_DEVICE::kKeyboard) {
        StartKey = KStart;
    }
    else if (device == RE::INPUT_DEVICE::kGamepad) {
        StartKey = GStart;
    }

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
    case KeyType::AutoMoveKey:
        return AutoMoveKey;
    case KeyType::CameraMod:
        return GCameraMod;
    default:
        return NULL;
    }
}

void Settings::ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
    const char* bFound = nullptr;
    bFound = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        a_setting = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
    }
}
void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
    const char* bFound = nullptr;
    bFound = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
    }
}
void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
    const char* bFound = nullptr;
    bFound = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        a_setting = static_cast<float>(a_ini.GetLongValue(a_sectionName, a_settingName));
    }
}

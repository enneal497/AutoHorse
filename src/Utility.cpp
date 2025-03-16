#include "MarkerHandler.h"
#include "Utility.h"
#include "Settings.h"
#include "Events.h"

namespace AutoHorse {

    void Utility::ShowPrompt()
    {
        auto g_tutorial = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(Settings::g_tutorialID, Settings::espName);
        if (g_tutorial->value == 1) {
            AutoHorse::ActivateEventHandler::Deregister();
            return;
        }

        if (!MarkerHandler::GetMarker()) {
            return;
        }

        g_tutorial->value = 1;

        auto keyName = SKSE::InputMap::GetKeyName(Settings::ReturnControls(KeyType::Start));
        auto message = std::format("Press [{}] to start autopilot", keyName);

        auto ui = RE::UI::GetSingleton();
        if (!ui) return;

        auto hudMenu = ui->GetMenu<RE::HUDMenu>();
        if (!hudMenu || !hudMenu->uiMovie) return;

        RE::GFxValue args[2];
        args[0].SetString(message);
        args[1].SetBoolean(true);
        hudMenu->uiMovie->Invoke("_root.HUDMovieBaseInstance.ShowTutorialHintText", nullptr, args, 2);

        stl::add_thread_task(Utility::HidePrompt, 3s);
    }

    void Utility::HidePrompt()
    {
        auto hud = RE::UI::GetSingleton()->GetMenu(RE::HUDMenu::MENU_NAME);
        if (hud) {
            RE::GFxValue args[2];
            args[0].SetString("");
            args[1].SetBoolean(false);
            hud->uiMovie->Invoke("_root.HUDMovieBaseInstance.ShowTutorialHintText", nullptr, args, 2);
        }

    }

    
    bool ForceRefToAlias(RE::TESQuest* a_quest, uint32_t a_aliasID, RE::TESObjectREFR* a_reference)
    {
        using func_t = decltype(&ForceRefToAlias);
        REL::Relocation<func_t> func{ RELOCATION_ID(24523, 25052) };
        return func(a_quest, a_aliasID, a_reference);
    }
    

    void Utility::ForceMountToAlias(RE::ActorPtr a_actor)
    {
        auto* quest = InputEventHandler::GetSingleton()->controlQuest;
        auto aliasID = quest->aliases[0]->aliasID;
        auto* actorRef = a_actor->AsReference();

        logger::info("Using alias {}", quest->aliases[0]->aliasName.c_str());
        ForceRefToAlias(quest, aliasID, actorRef);

    }

}

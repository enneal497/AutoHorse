#include "MarkerHandler.h"
#include "Settings.h"

namespace AutoHorse {

    bool MarkerHandler::GetMarker()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        RE::ObjectRefHandle markerRef = REL::Module::IsVR() ? player->GetVRInfoRuntimeData()->playerMapMarker : player->GetInfoRuntimeData().playerMapMarker;

        if (!markerRef) {
            GetQuestMarker();
        }
        else {
            GetPlayerMarker(markerRef);
        }

        //TEMP
        return true;

    }

    //Get custom player-set marker location
    void MarkerHandler::GetPlayerMarker(RE::ObjectRefHandle markerRef)
    {
        auto marker = markerRef.get().get();
        if (!marker) { return; }
        //auto m = marker->data.location;
        auto markerPos = marker->GetPosition();

        logger::info("x: {}", markerPos.x);

        MoveTargetMarker(markerPos);
    }

    //Get closest quest marker if available
    void MarkerHandler::GetQuestMarker()
    {
        
    }

    void MarkerHandler::MoveTargetMarker(RE::NiPoint3 markerPos)
    {
        //Replace with actual quest ID
        /*
        RE::TESQuest* controlQuest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(Settings::questID, Settings::espName);
        auto aliases = controlQuest->aliases;
        if (aliases.size() < 2) {
            return;
        }
        auto refAlias = static_cast<RE::BGSRefAlias*>(aliases[2]);
        logger::info("RefAlias name: {}", refAlias->aliasName.c_str());
        if (refAlias) {
            logger::info("refAlias obtained");
            auto ref = refAlias->GetReference();
            if (ref) {
                ref->data.location = markerPos;
            }
            else {
                logger::info("No ref found");
            }
        }
        */
        auto ref = RE::TESDataHandler::GetSingleton()->LookupForm(Settings::markerID, Settings::espName)->As<RE::TESObjectREFR>();
        if (!ref) {
            logger::info("No ref found");
        }
        else {
            logger::info("Ref found");
            ref->data.location = markerPos;
            ref->Update3DPosition(true);
        }

        //auto alias = targetQuest->aliases[0];

        //markerRef->data.location = markerPosition;

    }

}

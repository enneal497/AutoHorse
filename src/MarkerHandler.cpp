#include "MarkerHandler.h"
#include "Settings.h"

namespace AutoHorse {

    inline RE::NiPoint3 GetRealPosition(const RE::TESObjectREFR* a_objRef)
    {
        RE::NiPoint3 position = a_objRef->GetPosition();
        if (const RE::TESWorldSpace* worldSpace = a_objRef->GetWorldspace())
        {
            RE::NiPoint3 worldSpaceOffset{ worldSpace->worldMapOffsetData.mapOffsetX,
                                                  worldSpace->worldMapOffsetData.mapOffsetY,
                                                  worldSpace->worldMapOffsetData.mapOffsetZ };
            position += worldSpaceOffset * worldSpace->worldMapOffsetData.mapScale;
        }
        return position;
    }

    bool MarkerHandler::GetMarker()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        RE::ObjectRefHandle markerRef = REL::Module::IsVR() ? player->GetVRInfoRuntimeData()->playerMapMarker : player->GetInfoRuntimeData().playerMapMarker;

        if (!markerRef) {
            return GetQuestMarker();
        }
        else {
            return GetPlayerMarker(markerRef);
        }

    }

    //Get player-set marker location
    bool MarkerHandler::GetPlayerMarker(RE::ObjectRefHandle markerRef)
    {
        auto marker = markerRef.get().get();
        if (!marker) { return false; }
        
        auto markerPos = GetRealPosition(marker);
        //logger::info("x: {}", markerPos.x);

        return MoveTargetMarker(markerPos);
    }

    //Get available quest marker
    bool MarkerHandler::GetQuestMarker()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        
        RE::BSTArray<RE::BGSInstancedQuestObjective>& playerObjectives = REL::Module::IsVR() ?
            player->GetVRPlayerRuntimeData().objectives :
            player->GetPlayerRuntimeData().objectives;
        
        for (int i = playerObjectives.size() - 1; i >= 0; i--)
        {
            RE::BGSInstancedQuestObjective* playerObjective = &playerObjectives[i];
            RE::BGSQuestObjective* questObjective = playerObjective->Objective;

            if (!(questObjective->ownerQuest->IsActive() && questObjective->state == RE::QUEST_OBJECTIVE_STATE::kDisplayed))
            {
                logger::info("Quest objective not displayed");
                continue;
            }

            for (uint32_t j = 0; j < questObjective->numTargets; j++)
            {
                auto questTarget = questObjective->targets[j];
                auto aliasID = questTarget->alias;
                auto alias = questObjective->ownerQuest->aliases[aliasID];


                auto &name = alias->aliasName;
                logger::info("Alias name: {}", name.c_str());

                if (RE::BGSRefAlias *refAlias = static_cast<RE::BGSRefAlias*>(alias))
                {
                    if (auto ref = refAlias->GetReference()) {

                        if (ref->GetWorldspace() == player->GetWorldspace()) {
                            logger::info("Marker is in current worldspace");
                            auto loc = GetRealPosition(ref);

                            logger::info("Loc x: {}", loc.x);
                            logger::info("Loc y: {}", loc.y);
                            logger::info("Loc z: {}", loc.z);

                            return MoveTargetMarker(loc);
                        }
                        else {
                            logger::info("Marker is not in current worldspace");
                        }
                    }
                }
            }

        }

        return false;
    }

    //Move the player-set marker
    //A custom marker is used so that the AI works even if the player removes the 'Custom Destination' marker
    bool MarkerHandler::MoveTargetMarker(RE::NiPoint3 markerPos)
    {
        
        auto ref = RE::TESDataHandler::GetSingleton()->LookupForm(Settings::markerID, Settings::espName)->As<RE::TESObjectREFR>();
        if (!ref) {
            logger::info("No ref found");
            return false;
        }
        else {
            logger::info("Ref found");
            ref->data.location = markerPos;
            ref->Update3DPosition(true);
            return true;
        }

    }
    /*
    void MarkerHandler::ForceRefTo(RE::TESQuest* a_quest, uint32_t a_aliasID, RE::TESObjectREFR* a_reference)
    {
        using func_t = decltype(&ForceRefTo);
        REL::Relocation<func_t> func{ RELOCATION_ID(24523, 25052) };
        return func(a_quest, a_aliasID, a_reference);
    }
    */

}

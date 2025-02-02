#include "QuestHandler.h"

namespace AutoHorse {

    /*
    //Iterate through available quest objectives
    bool QuestHandler::GetQuestLocation()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        RE::BSTArray<RE::BGSInstancedQuestObjective>& playerObjectives = player->GetPlayerRuntimeData().objectives;

        if (playerObjectives.size() == 0) {
            return false;
        }

        for (int i = playerObjectives.size() -1; i >= 0; i--) {
            auto currentObjective = playerObjectives[i].Objective;
            auto quest = currentObjective->ownerQuest;

            //Check if quest objective is currently active
            if (currentObjective->state.get() != RE::QUEST_OBJECTIVE_STATE::kDisplayed || quest->IsActive() == false) {
                continue;
            }

            auto targets = currentObjective->targets;
            int targetSize = currentObjective->numTargets;

            //0 targets found for this quest
            if (targetSize == 0) {
                continue;
            }

            auto& questName = quest->formEditorID;
            logger::info("Quest Editor ID: {}", questName.c_str());

            for (int j = 0; j < targetSize; j++)
            {
                auto objectiveTarget = targets[j];
                //Alias id of objective target
                auto targetAlias = objectiveTarget->alias;

                QuestHandler::GetQuestAlias(quest, targetAlias);
                //return true;
            }
        }
        return false;

    }

    void QuestHandler::GetMarker()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto& marker = REL::Module::IsVR() ? player->GetVRInfoRuntimeData()->playerMapMarker : player->GetInfoRuntimeData().playerMapMarker;

        if (!marker) {
            return;
        }
        else {
            auto markerRef = marker.get().get();
            auto markerloc = markerRef->GetParentCell()->GetFormEditorID();
            logger::info("Player map marker: ", markerloc);
        }

    }

    void QuestHandler::GetQuestAlias(RE::TESQuest *ownerQuest, uint8_t aliasNum)
    {
        //Get (aliasNum)nth alias under ownerQuest
        auto &aliases = ownerQuest->aliases;
        RE::BGSBaseAlias* baseAlias = aliases[aliasNum];
        if (baseAlias == nullptr) {
            logger::info("Quest has no matching aliases");
            return;
        }

        auto refAlias = static_cast<RE::BGSRefAlias*>(baseAlias);
        if (!refAlias) {
            logger::info("Alias cannot be cast to BGSRefAlias");
            return;
        }

        objectiveRef = refAlias->GetReference();
        auto refName = objectiveRef->GetName();

        logger::info("refName: {}", refName);

        bool refPossible = QuestHandler::TestRefLocation();
    }

    bool QuestHandler::TestRefLocation()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player->GetParentCell()->IsInteriorCell()) {
            //Player is not in exterior worldspace
            //return false;
        }

        auto player_worldspace = player->GetWorldspace();
        auto worldname = player_worldspace->GetName();
        logger::info("Player worldspace name: {}", worldname);

        if (!objectiveRef) {
            logger::info("Reference invalid");
            return false;
        }

        auto refParentCell = objectiveRef->GetParentCell();
        if (!refParentCell) {
            logger::info("Ref parent cell not found");
            return false;
        }

        if (refParentCell->IsInteriorCell()) {
            //Objective ref is in interior cell
            logger::info("Reference is in an interior cell");
            return false;
        }
        else {
            logger::info("Reference is in an exterior cell");
            auto ref_worldspace = objectiveRef->GetWorldspace();

            if (ref_worldspace) {
                auto refworldname = ref_worldspace->GetName();
                logger::info("Target worldspace name: {}", refworldname);
            }
            else {
                logger::info("Target reference has no worldspace");
                return false;
            }
        }

        return true;
    }
    */

}

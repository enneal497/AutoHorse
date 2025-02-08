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

        markerPosition = marker->GetPosition();

        float xPos = marker->GetPositionX();
        float yPos = marker->GetPositionY();
        float zPos = marker->GetPositionZ();

        logger::info("xPos: {}", xPos);
        logger::info("yPos: {}", yPos);
        logger::info("zPos: {}", zPos);

        //MoveTargetMarker();
    }

    //Get closest quest marker if available
    void MarkerHandler::GetQuestMarker()
    {
        
    }

    void MarkerHandler::MoveTargetMarker()
    {
        //Replace with actual quest ID
        RE::TESQuest* controlQuest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(Settings::questID, Settings::espName);

        //auto alias = targetQuest->aliases[0];

        //markerRef->data.location = markerPosition;

    }

}

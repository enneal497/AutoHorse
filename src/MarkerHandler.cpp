#include "MarkerHandler.h"

namespace AutoHorse {

    void MarkerHandler::GetMarker()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        RE::ObjectRefHandle markerRef = REL::Module::IsVR() ? player->GetVRInfoRuntimeData()->playerMapMarker : player->GetInfoRuntimeData().playerMapMarker;

        if (!markerRef) {
            GetQuestMarker();
        }
        else {
            GetPlayerMarker(markerRef);
        }

    }

    //Get custom player-set marker location
    void MarkerHandler::GetPlayerMarker(RE::ObjectRefHandle markerRef)
    {
        auto marker = markerRef.get().get();

        markerPosition = marker->GetPosition();

        xPos = marker->GetPositionX();
        yPos = marker->GetPositionY();
        zPos = marker->GetPositionZ();

        logger::info("xPos: {}", xPos);
        logger::info("yPos: {}", yPos);
        logger::info("zPos: {}", zPos);


    }

    //Get closest quest marker if available
    void MarkerHandler::GetQuestMarker()
    {

    }

}

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

        if (markerRef) {
            return GetPlayerMarker(markerRef);
        }

        return false;

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

}

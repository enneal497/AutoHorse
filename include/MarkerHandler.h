
namespace AutoHorse {

    
    class MarkerHandler
    {
    public:
        static bool GetMarker();
        static void GetPlayerMarker(RE::ObjectRefHandle markerRef);
        static void GetQuestMarker();
        static void MoveTargetMarker();
    private:
        static inline RE::NiPoint3 markerPosition;
        //static void GetQuestAlias(RE::TESQuest* ownerQuest, uint8_t aliasNum);
        //static bool TestRefLocation();
        //static inline RE::TESObjectREFR *objectiveRef;
    };
    

}

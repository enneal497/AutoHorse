
namespace AutoHorse {

    
    class MarkerHandler
    {
    public:
        static bool GetMarker();
        static bool GetPlayerMarker(RE::ObjectRefHandle markerRef);
        static bool MoveTargetMarker(RE::NiPoint3 markerPos);

    private:
        //static void GetQuestAlias(RE::TESQuest* ownerQuest, uint8_t aliasNum);
        //static bool TestRefLocation();
        //static inline RE::TESObjectREFR *objectiveRef;
    };
    

}

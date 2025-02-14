
namespace AutoHorse {

    
    class MarkerHandler
    {
    public:
        static bool GetMarker();
        static bool GetPlayerMarker(RE::ObjectRefHandle markerRef);
        static bool GetQuestMarker();
        static bool MoveTargetMarker(RE::NiPoint3 markerPos);

        static void ForceRefTo(RE::TESQuest* controlQuest, uint32_t aliasID, RE::TESObjectREFR* a_ref);
    private:
        //static void GetQuestAlias(RE::TESQuest* ownerQuest, uint8_t aliasNum);
        //static bool TestRefLocation();
        //static inline RE::TESObjectREFR *objectiveRef;
    };
    

}

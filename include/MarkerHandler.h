
namespace AutoHorse {

    
    class MarkerHandler
    {
    public:
        static void GetMarker();
        static void GetPlayerMarker(RE::ObjectRefHandle markerRef);
        static void GetQuestMarker();
    private:
        static inline float xPos;
        static inline float yPos;
        static inline float zPos;

        static inline RE::NiPoint3 markerPosition;
        //static void GetQuestAlias(RE::TESQuest* ownerQuest, uint8_t aliasNum);
        //static bool TestRefLocation();
        //static inline RE::TESObjectREFR *objectiveRef;
    };
    

}

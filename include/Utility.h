#pragma once

namespace AutoHorse {

    bool ForceRefToAlias(RE::TESQuest* script, unsigned int aliasID, RE::TESObjectREFR* ref);

    class Utility
    {
    public:
        static void ShowPrompt();
        static void HidePrompt();
        static void ForceMountToAlias(RE::ActorPtr a_actor);
    };
}

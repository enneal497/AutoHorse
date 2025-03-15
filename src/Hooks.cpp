#include "Hooks.h"
#include "PCH.h"

namespace Hooks {

    float UpdateRotation_Hook(RE::Actor* actor)
    {
        auto* player = RE::PlayerCharacter::GetSingleton();

        if (player->GetActorRuntimeData().movementController.get()->GetAIDriven()) {
            return 0.0;
        }
        return UpdateRotationF(actor);
    }

    float GetAngleX_Hook(RE::Character*)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        return player->data.angle.x;
    }

    void HandleRotationX_Hook(RE::Character* character, float rot)
    {
        auto* player = RE::PlayerCharacter::GetSingleton();
        character->data.angle.x = rot + player->GetAngleX();
    }

    void Install()
    {
        REL::Relocation<uintptr_t> UpdateRotation{ RELOCATION_ID(49840, 50771) };
        REL::Relocation<uintptr_t> GetAngleX{ RELOCATION_ID(49839, 50770) };
        REL::Relocation<uintptr_t> HandleRotationX{ RELOCATION_ID(49907, 50840) };

        auto& trampoline = SKSE::GetTrampoline();

        UpdateRotationF = trampoline.write_call<5>(UpdateRotation.address() + 132, &UpdateRotation_Hook);
        trampoline.write_call<5>(GetAngleX.address() + REL::Relocate(151, 140), &GetAngleX_Hook);
        trampoline.write_call<5>(HandleRotationX.address() + REL::Relocate(509, 516), &HandleRotationX_Hook);
    }
}

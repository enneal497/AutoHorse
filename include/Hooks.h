#pragma once

namespace Hooks
{
    void Install();

    float UpdateRotation_Hook(RE::Actor* actor);
    float GetAngleX_Hook(RE::Character*);
    void HandleRotationX_Hook(RE::Character* character, float rot);
    static REL::Relocation<decltype(UpdateRotation_Hook)> UpdateRotationF;
};

#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>
#include <Wiwa/ecs/components/game/items/Item.h>
#include <Wiwa/ecs/components/game/Character.h>
#include "../CharacterInventory.h"

namespace Wiwa
{
    class WI_API UltronGUISystem : public System
    {
    private:
        bool bossIsDead = false;
        bool activeBossCanvas = false;
        bool introBossFight = true;
        bool activeIntroCanvas = false;
        float counterIntroAnim = 0.0f;
        float timeIntroAnim = 10.0f;
    public:
        UltronGUISystem() = default;
        virtual ~UltronGUISystem() = default;

        void OnUpdate() override;

        bool AnimationIntroBoss(Wiwa::GuiManager& gm);

        void OnCollisionEnter(Object* body1, Object* body2) override;
    };
}

REGISTER_SYSTEM(Wiwa::UltronGUISystem);
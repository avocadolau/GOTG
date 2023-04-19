#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>

namespace Wiwa
{
    // This system only takes care of the pickup system
    class WI_API PlayerGUISystem : public System
    {
    private:
        int CanvasHUD = 0;
        int DeathHUD = 4;
        bool deathHud = false;
        Ability** abilitiesList;
        Buff** buffsList;
    public:
        PlayerGUISystem() = default;
        virtual ~PlayerGUISystem() = default;

        void OnUpdate() override;

        void DeathHud(Wiwa::GuiManager& gm);

        void CooldownState(Ability** ability,Wiwa::GuiManager& gm);
        void CooldownState(Buff** buff, Wiwa::GuiManager& gm);

        void HandleActiveAbilities(Ability** ability, Wiwa::GuiManager& gm);
        void HandleActiveBuffs(Buff** buff, Wiwa::GuiManager& gm);
        void PlayerElements(Wiwa::GuiManager& gm, Character* character);

    };
}

REGISTER_SYSTEM(Wiwa::PlayerGUISystem);
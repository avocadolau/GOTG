#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>

namespace Wiwa
{
    // This system only takes care of the pickup system
    class WI_API PlayerGUISystem : public System
    {
    private:
        int CurrentHUD = 0;
        int CanvasHUD = 0;
        int PauseHUD = 1;
        int OptionsHUD = 2;
        int DeathHUD = 3;
        bool deathHud = false;
        bool returnToHUD = false;
        bool returnToPauseHUD = false;
        bool pauseGame = false;
        size_t lastCoins = -1;
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

        void PauseElementsUpdate(Ability** ability, Buff** buff, Wiwa::GuiManager& gm);

        void Coins(uint32_t coins, Wiwa::GuiManager& gm);

    };
}

REGISTER_SYSTEM(Wiwa::PlayerGUISystem);
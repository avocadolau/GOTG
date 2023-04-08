#include <wipch.h>
#include "Item.h"

#include "Wiwa/ecs/components/game/Character.h"
#include "Wiwa/game/GameStateManager.h"
#include "Wiwa/scene/Scene.h"

namespace Wiwa
{
    Character* GetPlayerComp()
    {
        return GameStateManager::GetCurrentScene()->GetEntityManager().GetComponent<Character>(GameStateManager::GetPlayerId());
    }

    
    void Ability::Use()
    {
        switch (AbilityType)
        {
        case AbilityType::YONDUS_FIN:
            {
                YondusFin();
            }
            break;
        case AbilityType::GROOTS_SEEDS:
            {
                GrootsSeeds();
            }
            break;
        case AbilityType::PHYLAS_QUANTUM_SWORD:
            {
                PhylasQuantumSword();
            }
            break;
        case AbilityType::STARHAWKS_BLAST:
            {
                StarhawksBlast();
            }
            break;
        }
    }

    void Ability::YondusFin()
    {
    }

    void Ability::GrootsSeeds()
    {
    }

    void Ability::PhylasQuantumSword()
    {
    }

    void Ability::StarhawksBlast()
    {
    }

    void PassiveSkill::Use()
    {
        Character* player = GetPlayerComp();
        if(!player)
        {
            WI_CORE_ERROR("Player can't be found");
            return;
        }
        const float buffPercent = ((float)BuffPercent / 100.f);
        switch (PassiveType)
        {
        case Wiwa::PassiveType::MOVEMENT:
            {
                player->Speed += player->Speed * buffPercent;
            }
            break;
        case Wiwa::PassiveType::ROF:
            {
                player->RateOfFire += player->RateOfFire * buffPercent;
            }
            break;
        case Wiwa::PassiveType::BUFF:
            {
                // TODO: Increase current buffs duration
            }
            break;
        case Wiwa::PassiveType::ATTACK:
            {
                player->Damage += (int)((float)player->Damage * buffPercent);
            }
            break;
        case PassiveType::HEALTH:
            {
                player->MaxHealth += (int)((float)player->MaxHealth * buffPercent);
                player->Health += (int)((float)player->Health * buffPercent);
            }
            break;
        case PassiveType::SHIELD_CHARGE:
            {
                
            }
            break;
        case PassiveType::PROJECTILE:
            {
                
            }
            break;
        case PassiveType::RANGE:
            {
                
            }
            break;
        }
    }
    void Buff::Use()
    {
        Character* player = GetPlayerComp();
        if(!player)
        {
            WI_CORE_ERROR("Player can't be found");
            return;
        }
        IsActive = true;
        switch (buffType)
        {
        case Wiwa::BuffType::MAJOR_VICTORY_SHIELD:
            {
                
            }
            break;
        case Wiwa::BuffType::NIKKIS_TOUCH:
            {
                
            }
            break;
        case Wiwa::BuffType::COSMOS_PAW:
            {
                // TODO: Spawn some kind of feedback
                const float buffPercent = ((float)BuffPercent / 100.f);
                MaxHealthInc = (int)((float)player->MaxHealth * buffPercent);
                HealthInc = (int)((float)player->Health * buffPercent);
                MaxShieldInc = (int)((float)player->MaxShield * buffPercent);
                ShieldInc = (int)((float)player->Shield * buffPercent);
                
                player->MaxHealth += MaxHealthInc;
                player->Health += HealthInc;
                player->MaxShield += MaxShieldInc;
                player->Shield += ShieldInc;
            }
            break;
        case Wiwa::BuffType::MARTINEX_THERMOKINESIS:
            {
                
            }
            break;
        case Wiwa::BuffType::BUGS_LEGS:
            {
                
            }
            break;
        case Wiwa::BuffType::CHARLIE27_FIST:
            {
                
            }
            break;
        }
    }

    void Buff::UnUse()
    {
        Character* player = GetPlayerComp();
        if(!player)
        {
            WI_CORE_ERROR("Player can't be found");
            return;
        }
        CoolDownTimer = 0.f;
        IsActive = false;
        switch (buffType)
        {
        case Wiwa::BuffType::MAJOR_VICTORY_SHIELD:
        {

        }
        break;
        case Wiwa::BuffType::NIKKIS_TOUCH:
        {

        }
        break;
        case Wiwa::BuffType::COSMOS_PAW:
        {
             // TODO: Spawn some kind of feedback
             player->MaxHealth -= MaxHealthInc;
             player->Health -= HealthInc;
             player->MaxShield -= MaxShieldInc;
             player->Shield -= ShieldInc;
        }
        break;
        case Wiwa::BuffType::MARTINEX_THERMOKINESIS:
        {

        }
        break;
        case Wiwa::BuffType::BUGS_LEGS:
        {

        }
        break;
        case Wiwa::BuffType::CHARLIE27_FIST:
        {

        }
        break;
        }
    }


    void Consumable::Use()
    {
        Character* player = GetPlayerComp();
        if(!player)
        {
            WI_CORE_ERROR("Player can't be found");
            return;
        }

        
        if(IsEgosHelp)
        {
            // TODO: Destroy all enemies (no boss)
            WI_CORE_INFO("Using Ego's help");
            return;
        }
        
        const float buffPercent = ((float)BuffPercent / 100.f);
        switch (Type)
        {
        case Wiwa::ConsumableType::HEAL:
            {
                int healAmount = (int)((float)player->Health * buffPercent);
                player->Health += healAmount;
                if(player->Health >= player->MaxHealth)
                    player->Health = player->MaxHealth;

                WI_CORE_INFO("Player healed {} health", healAmount);
            }
            break;
        case Wiwa::ConsumableType::SHIELD:
            {
                int shieldAmount = (int)((float)player->Shield * buffPercent);
                player->Shield += shieldAmount;
                if(player->Shield >= player->MaxShield)
                    player->Shield = player->MaxShield;

                WI_CORE_INFO("Player shielded {} shield", shieldAmount);
            }
            break;
        }
    }
}

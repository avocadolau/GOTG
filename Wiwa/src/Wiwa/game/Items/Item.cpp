#include <wipch.h>
#include "Item.h"

#include "Wiwa/ecs/components/game/Character.h"
#include "Wiwa/game/GameStateManager.h"
#include "Wiwa/scene/Scene.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

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
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId grootSeeds = em.LoadPrefab("assets/vfx/prefabs/vfx_finals/p_shield.wiprefab");
    }

    void Ability::PhylasQuantumSword()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId phylasSword = em.LoadPrefab("assets/Prefabs/PhylasSword.wiprefab");
    }

    void Ability::StarhawksBlast()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId starHawksBlast = em.LoadPrefab("assets/Prefabs/StarHawksBlast.wiprefab");
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
                Inventory& PlayerInventory = GameStateManager::GetPlayerInventory();
                Buff** playerCurrentBuffs = PlayerInventory.GetBuffs();
                for (int i = 0; i < 2; i++)
                {
                    playerCurrentBuffs[i]->Duration += playerCurrentBuffs[i]->Duration * buffPercent;
                }
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
                 Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
                 EntityId shieldId = em.LoadPrefab("assets/Prefabs/VictoryShield.wiprefab");   
            }
            break;
        case Wiwa::BuffType::NIKKIS_TOUCH:
            {
                const float buffPercent = ((float)BuffPercent / 100.f);
                AttackDmgInc = (int)(float)player->Damage * buffPercent;

                player->Damage += AttackDmgInc;
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
                //Done in the enemy
            }
            break;
        case Wiwa::BuffType::BUGS_LEGS:
            {
                const float buffPercent = ((float)BuffPercent / 100.f);
                RoFInc = ((float)player->RateOfFire * buffPercent);
                
                player->RateOfFire += RoFInc;
            }
            break;
        case Wiwa::BuffType::CHARLIE27_FIST:
            {

                const float buffPercent = ((float)BuffPercent / 100.f);
                AttackDmgInc = (int)((float)player->Damage * buffPercent);

                player->Damage += AttackDmgInc;
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
            //Also done
        }
        break;
        case Wiwa::BuffType::NIKKIS_TOUCH:
        {
            player->Damage -= AttackDmgInc;
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
            //Done in the enemy
        }
        break;
        case Wiwa::BuffType::BUGS_LEGS:
        {
            player->RateOfFire -= RoFInc;
        }
        break;
        case Wiwa::BuffType::CHARLIE27_FIST:
        {
            player->Damage -= AttackDmgInc;
        }
        break;
        }
    }


    void Consumable::Use()
    {
        Wiwa::EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
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

        EntityId playerParticles = em.GetEntityByName("Player");

        const float buffPercent = ((float)BuffPercent / 100.f);
        switch (Type)
        {
        case Wiwa::ConsumableType::HEAL:
            {
                if (playerParticles)
                {

                    EntityId pe_healing = em.GetChildByName(playerParticles, "PE_Healing");

                }

                int healAmount = (int)((float)player->Health * buffPercent);
                player->Health += healAmount;
                if(player->Health >= player->MaxHealth)
                    player->Health = player->MaxHealth;

                WI_CORE_INFO("Player healed {} health", healAmount);
            }
            break;
        case Wiwa::ConsumableType::SHIELD:
            {
                if (playerParticles)
                {

                    EntityId pe_shield = em.GetChildByName(playerParticles, "PE_Shield");

                }

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

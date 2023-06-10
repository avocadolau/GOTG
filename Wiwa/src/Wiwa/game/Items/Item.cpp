#include <wipch.h>
#include "Item.h"

#include "Wiwa/ecs/components/game/Character.h"
#include "Wiwa/game/GameStateManager.h"
#include "../../Editor/game_assembly/src/components/attack/SimpleBullet.h"
#include "Wiwa/scene/Scene.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
    Character* GetPlayerComp()
    {
        return GameStateManager::GetCurrentScene()->GetEntityManager().GetComponent<Character>(GameStateManager::GetPlayerId());
    }

    
    void Ability::Use()
    {
        Wiwa::EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
        EntityId playerId = em.GetEntityByName("StarLord");
        EntityId playerId2 = em.GetEntityByName("Rocket");
        if (playerId != EntityManager::INVALID_INDEX || playerId2 != EntityManager::INVALID_INDEX)
        {

            EntityId pe_healing = em.LoadPrefab("assets/vfx/prefabs/p_player_use.wiprefab");

            if (pe_healing != EntityManager::INVALID_INDEX)
            {
                if (playerId != EntityManager::INVALID_INDEX)
                    em.SetParent(pe_healing, playerId);

                if (playerId2 != EntityManager::INVALID_INDEX)
                    em.SetParent(pe_healing, playerId2);
                //WI_CORE_INFO("spawning regeneartion particle");

            }
            else
            {
                WI_CORE_INFO("Failed Spawning Use Particles");

            }

        }
        else
            WI_CORE_INFO("No valid Player ID");

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
        case AbilityType::CAPTAINS_UNIVERSE:
            {
                CaptainsUniverse();
            }
            break;
        case AbilityType::MANTIS_TELEPATHIC_THRUST:
            {
                MantisTelepathicThrust();
            }
            break;
        }
    }

    void Ability::YondusFin()
    {
        if (Wiwa::GameStateManager::GetActiveEnemies() > 0)
        {
            Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
            EntityId yondusFin = em.LoadPrefab("assets\\Prefabs\\YondusFin\\YundusFin_01.wiprefab");
            Transform3D* tr = em.GetComponent<Transform3D>(yondusFin);
            PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(yondusFin);
            NavAgentSystem* navAgentSys = em.GetSystem<NavAgentSystem>(yondusFin);
            *tr = *em.GetComponent<Transform3D>(GameStateManager::GetPlayerId());
        }
    }

    void Ability::GrootsSeeds()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId grootSeeds = em.LoadPrefab("assets/vfx/prefabs/vfx_finals/abilities/groot_seeds/p_groot_seed.wiprefab");
    }

    void Ability::PhylasQuantumSword()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId phylasSword = em.LoadPrefab("assets/Prefabs/AbilitiesPrefabs/PhylasSword.wiprefab");
    }

    void Ability::StarhawksBlast()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId starHawksBlast = em.LoadPrefab("assets/Prefabs/AbilitiesPrefabs/StarHawksBlast.wiprefab");
    }

    void Ability::CaptainsUniverse()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId CaptainsUniverse = em.LoadPrefab("assets/Prefabs/AbilitiesPrefabs/CaptainUniverseEnergy.wiprefab");
    }

    void Ability::MantisTelepathicThrust()
    {
        Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
        EntityId MantisTelepathicThrust = em.LoadPrefab("assets/Prefabs/AbilitiesPrefabs/MantisTelepathicThrust.wiprefab");
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
                if (*playerCurrentBuffs != nullptr)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (playerCurrentBuffs[i] != nullptr)
                        {
                            playerCurrentBuffs[i]->Duration += playerCurrentBuffs[i]->Duration * buffPercent;
                        }
                    }
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
                 player->ShieldMultiplier += (int)((float)player->ShieldMultiplier * buffPercent);
            }
            break;
        case PassiveType::PROJECTILE:
            {
                Inventory& PlayerInventory = GameStateManager::GetPlayerInventory();
                Ability** playerCurrentAbilities = PlayerInventory.GetAbilities();
                if (*playerCurrentAbilities != nullptr)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (playerCurrentAbilities[i] != nullptr)
                        {
                            if (playerCurrentAbilities[i]->itemTag[0] == ItemTags::PROJECTILE || playerCurrentAbilities[i]->itemTag[1] == ItemTags::PROJECTILE)
                            {
                                playerCurrentAbilities[i]->Damage += (int)((float)playerCurrentAbilities[i]->Damage * buffPercent);
                            }
                        }
                    }
                }
            }
            break;
        case PassiveType::RANGE:
            {
                //
               /* Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
                EntityId newBulletId = EntityManager::INVALID_INDEX;
                newBulletId = GameStateManager::s_PoolManager->s_StarLordBullets->GetFromPool();
                SimpleBullet* bullet = (SimpleBullet*)em.GetComponentByIterator(em.GetComponentIterator<SimpleBullet>(newBulletId));
                bullet->lifeTime += (int)((float)bullet->lifeTime * buffPercent);*/
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
                 EntityId shieldId = em.LoadPrefab("assets/Prefabs/AbilitiesPrefabs/VictoryShield.wiprefab");   
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
                Wiwa::EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
                EntityId shieldId = em.LoadPrefab("assets/vfx/prefabs/vfx_finals/p_shield.wiprefab");
                em.SetParent(shieldId, Wiwa::GameStateManager::GetPlayerId());

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

        EntityId playerId = em.GetEntityByName("StarLord");

        const float buffPercent = ((float)BuffPercent / 100.f);
        switch (Type)
        {
        case Wiwa::ConsumableType::HEAL:
            {
                if (playerId != EntityManager::INVALID_INDEX)
                {

                    EntityId pe_healing = em.LoadPrefab("assets/vfx/prefabs/p_player_regeneration.wiprefab");
                    
                    if (pe_healing != EntityManager::INVALID_INDEX)
                    {
                        em.SetParent(pe_healing, playerId);
                        //WI_CORE_INFO("spawning regeneartion particle");

                    }
                    else
                    {
                        WI_CORE_INFO("Failed Spawning Regeneration Particles");

                    }

                }
                else
                    WI_CORE_INFO("No valid Player ID");


                int healAmount = (int)((float)player->Health * buffPercent);
                player->Health += healAmount;
                if(player->Health >= player->MaxHealth)
                    player->Health = player->MaxHealth;

                WI_CORE_INFO("Player healed {} health", healAmount);
            }
            break;
        case Wiwa::ConsumableType::SHIELD:
            {
                if (playerId)
                {

                    EntityId pe_shield = em.GetChildByName(playerId, "PE_Shield");

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

    void ShopElement::Use()
    {
        const float buffPercent = ((float)PercentageIncreases.at(CurrentStep) / 100.f);
        Wiwa::EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
        Character* player = GetPlayerComp();
        int newDropChance = 0;
        switch (PassiveBoost)
        {
        case Wiwa::HowardElementType::FANCY_BOOTS:
            player->Speed += player->Speed * buffPercent;
            MovSpeedInc = player->Speed * buffPercent;
            break;
        case Wiwa::HowardElementType::EASY_TRIGGER:
            player->RateOfFireMultiplier += player->RateOfFireMultiplier * buffPercent;
            RofInc = player->RateOfFireMultiplier * buffPercent;
            break;
        case Wiwa::HowardElementType::NANO_BOOST:
            player->Damage += player->Damage * buffPercent;
            DamageInc = player->Damage * buffPercent;
            break;
        case Wiwa::HowardElementType::NANO_MACHINES:
            Wiwa::GameStateManager::DamageDivisor = buffPercent * 100;
            break;
        case Wiwa::HowardElementType::HEALTH_CAPACITOR:
            player->Health += player->Health * buffPercent;
            HealthInc = player->Health * buffPercent;
            player->MaxHealth += player->MaxHealth * buffPercent;
            MaxHealthInc = player->MaxHealth * buffPercent;
            break;
        case Wiwa::HowardElementType::SHIELD_FAN:
            player->ShieldRegeneration += player->ShieldRegeneration * buffPercent;
            break;
        case Wiwa::HowardElementType::RECOVERY_SHIELD:
            player->Shield += player->MaxShield * buffPercent;
            if (player->Shield >= player->MaxShield)
            {
                player->Shield = player->MaxShield;
            }
            break;
        case Wiwa::HowardElementType::SECOND_WIND:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::REROLL:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::BEGINNERS_LUCK:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::MIDAS_TOUCH:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::DEVOURER:
            newDropChance = Wiwa::GameStateManager::s_EnemyDropChances;
            newDropChance += newDropChance * buffPercent;
            Wiwa::GameStateManager::SetEnemyDropChance(newDropChance);
            break;
        case Wiwa::HowardElementType::FANATIC:
            Wiwa::GameStateManager::SetFanaticEffect(true);
            break;
        case Wiwa::HowardElementType::RECOVERY_HEALTH:
            player->Health += player->MaxHealth * buffPercent;
            if (player->Health >= player->MaxHealth)
            {
                player->Health = player->MaxHealth;
            }
            break;
        case Wiwa::HowardElementType::ULTIMATE_MIDAS_TOUCH:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::FRIENDLY_FACE:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::LETHAL_SHOOTER:
            Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
            EntityId newBulletId = EntityManager::INVALID_INDEX;
            newBulletId = GameStateManager::s_PoolManager->s_StarLordBullets->GetFromPool();
            SimpleBullet* bullet = (SimpleBullet*)em.GetComponentByIterator(em.GetComponentIterator<SimpleBullet>(newBulletId));
            bullet->lifeTime += ((float)bullet->lifeTime * buffPercent);
            RangeInc = ((float)bullet->lifeTime * buffPercent);
            break;
        }
    }

    void ShopElement::UnUse()
    {
        const float buffPercent = ((float)PercentageIncreases.at(CurrentStep - 1) / 100.f);
        Wiwa::EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
        Character* player = GetPlayerComp();

        switch (PassiveBoost)
        {
        case Wiwa::HowardElementType::FANCY_BOOTS:
            player->Speed -= MovSpeedInc;
            break;
        case Wiwa::HowardElementType::EASY_TRIGGER:
            player->RateOfFireMultiplier -= RofInc;
            break;
        case Wiwa::HowardElementType::NANO_BOOST:
            player->Damage -= DamageInc;
            break;
        case Wiwa::HowardElementType::NANO_MACHINES:
            break;
        case Wiwa::HowardElementType::HEALTH_CAPACITOR:
            player->Health -= HealthInc;
            player->MaxHealth -= MaxHealthInc;
            break;
        case Wiwa::HowardElementType::SHIELD_FAN:
            player->ShieldRegeneration -= ShieldRegenInc;
            break;
        case Wiwa::HowardElementType::RECOVERY_SHIELD:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::SECOND_WIND:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::REROLL:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::BEGINNERS_LUCK:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::MIDAS_TOUCH:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::DEVOURER:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::FANATIC:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::RECOVERY_HEALTH:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::ULTIMATE_MIDAS_TOUCH:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::FRIENDLY_FACE:
            //TODO: MAKE IT WORK
            break;
        case Wiwa::HowardElementType::LETHAL_SHOOTER:
            Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
            EntityId newBulletId = EntityManager::INVALID_INDEX;
            newBulletId = GameStateManager::s_PoolManager->s_StarLordBullets->GetFromPool();
            SimpleBullet* bullet = (SimpleBullet*)em.GetComponentByIterator(em.GetComponentIterator<SimpleBullet>(newBulletId));
            bullet->lifeTime -= RangeInc;
            break;
        }
    }
    void ShopElement::AugmentStep()
    {
        CurrentStep++;
        if (CurrentStep >= AmountOfSteps)
        {
            CurrentStep = AmountOfSteps;
        }
    }
}

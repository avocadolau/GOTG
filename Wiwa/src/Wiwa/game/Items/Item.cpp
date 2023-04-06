#include <wipch.h>
#include "Item.h"

namespace Wiwa
{
    void Ability::Use()
    {
        switch (AbilityType)
        {
        case AbilityType::YONDUS_SEEDS:
            {
                
            }
            break;
        case AbilityType::GROOTS_SEEDS:
            {
                
            }
            break;
        case AbilityType::PHYLAS_QUANTUM_SWORD:
            {
                
            }
            break;
        case AbilityType::STARHAWKS_BLAST:
            {
                
            }
            break;
        }
    }
    void PassiveSkill::Use()
    {
        switch (passiveType)
        {
        case Wiwa::PassiveType::MOVEMENT:
            break;
        case Wiwa::PassiveType::ROF:
            break;
        case Wiwa::PassiveType::BUFF:
            break;
        case Wiwa::PassiveType::ATTACK:
            break;
        default:
            break;
        }
    }
    void Buff::Use()
    {
        switch (buffType)
        {
        case Wiwa::BuffType::MAJOR_VICTORY_SHIELD:
            break;
        case Wiwa::BuffType::NIKKIS_TOUCH:
            break;
        case Wiwa::BuffType::COSMOS_PAW:
            break;
        case Wiwa::BuffType::MARTINEX_THERMOKINESIS:
            break;
        case Wiwa::BuffType::BUGS_LEGS:
            break;
        case Wiwa::BuffType::CHARLIE27_FIST:
            break;
        default:
            break;
        }
    }

    void Consumable::Use()
    {
        switch (Type)
        {
        case Wiwa::ConsumableType::HEAL:
            break;
        case Wiwa::ConsumableType::SHIELD:
            break;
        default:
            break;
        }
    }
}

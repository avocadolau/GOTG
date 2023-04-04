#include <wipch.h>
#include "Inventory.h"

#define MAX_ABILITIES 2
#define MAX_BUFFS 2

Wiwa::Inventory::Inventory()
{
    InitGame();
}

Wiwa::Inventory::~Inventory()
{

}

void Wiwa::Inventory::Serialize(JSONDocument* doc)
{
}

void Wiwa::Inventory::Deserialize(JSONDocument* doc)
{
}

void Wiwa::Inventory::InitGame()
{
    m_Abilities = new Ability*[MAX_ABILITIES] { nullptr, nullptr};
    m_Buffs = new Buff*[MAX_BUFFS] {nullptr, nullptr};
}

void Wiwa::Inventory::AddAbility(Ability* ability)
{
    
}

void Wiwa::Inventory::AddPassive(const PassiveSkill& skill)
{
    m_PassiveSkill.emplace_back(skill);
}

void Wiwa::Inventory::Update()
{
    
}

void Wiwa::Inventory::UseAbility(size_t index)
{
}

void Wiwa::Inventory::Clear()
{
    for(size_t i = 0; i < MAX_ABILITIES; i++)
    {
        delete m_Abilities[i];
    }
    delete[] m_Abilities;
    
    for(size_t i = 0; i < MAX_BUFFS; i++)
    {
        delete m_Buffs[i];
    }
    delete[] m_Buffs;
    
    m_PassiveSkill.clear();
}

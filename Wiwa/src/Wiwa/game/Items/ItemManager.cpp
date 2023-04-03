#include <wipch.h>
#include "ItemManager.h"

std::map<std::string, Wiwa::Ability> Wiwa::ItemManager::m_AbilityPool;
std::map<std::string, Wiwa::PassiveSkill> Wiwa::ItemManager::m_PassiveSkillPool;
std::map<std::string, Wiwa::Buff> Wiwa::ItemManager::m_BuffPool;
std::map<std::string, Wiwa::Consumable> Wiwa::ItemManager::m_ConsumablePool;

void Wiwa::ItemManager::AddAbility(Ability ability)
{
	m_AbilityPool.emplace(ability.Name, ability);
}

void Wiwa::ItemManager::DeleteAbility(std::string name)
{
	m_AbilityPool.erase(name);
}

void Wiwa::ItemManager::AddPassive(PassiveSkill passive)
{
	m_PassiveSkillPool[passive.Name] = passive;
}

void Wiwa::ItemManager::DeletePassive(std::string name)
{
	m_PassiveSkillPool.erase(name);
}

void Wiwa::ItemManager::AddBuff(Buff buff)
{
	m_BuffPool[buff.Name] = buff;
}

void Wiwa::ItemManager::DeleteBuff(std::string name)
{
	m_BuffPool.erase(name);
}

void Wiwa::ItemManager::AddConsumable(Consumable consumable)
{
	m_ConsumablePool[consumable.Name] = consumable;
}

void Wiwa::ItemManager::DeleteConsumable(std::string name)
{
	m_ConsumablePool.erase(name);
}

Wiwa::Ability* Wiwa::ItemManager::GetAbility(const char* name)
{
	return &m_AbilityPool[name];
}

Wiwa::PassiveSkill* Wiwa::ItemManager::GetPassive(const char* name)
{
	return &m_PassiveSkillPool[name];
}

Wiwa::Buff* Wiwa::ItemManager::GetBuff(const char* name)
{
	return &m_BuffPool[name];
}

Wiwa::Consumable* Wiwa::ItemManager::GetConsumable(const char* name)
{
	return &m_ConsumablePool[name];
}

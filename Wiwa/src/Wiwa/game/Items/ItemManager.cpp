#include <wipch.h>
#include "ItemManager.h"

#include <Wiwa/utilities/json/JSONDocument.h>

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

void Wiwa::ItemManager::Serialize(JSONDocument* doc)
{
	JSONValue actives = doc->AddMemberArray("actives");
	for (const auto& active : m_AbilityPool)
	{
		JSONValue activeObj = actives.PushBackObject();
		activeObj.AddMember("name", active.second.Name.c_str());
		activeObj.AddMember("description", active.second.Description.c_str());
	}

	JSONValue passives = doc->AddMemberArray("passives");
	for (const auto& passive : m_PassiveSkillPool)
	{
		JSONValue passiveObj = passives.PushBackObject();
		passiveObj.AddMember("name", passive.second.Name.c_str());
		passiveObj.AddMember("description", passive.second.Description.c_str());
	}

	JSONValue buffs = doc->AddMemberArray("buffs");
	for (const auto& buff : m_BuffPool)
	{
		JSONValue buffObj = buffs.PushBackObject();
		buffObj.AddMember("name", buff.second.Name.c_str());
		buffObj.AddMember("description", buff.second.Description.c_str());
	}

	JSONValue consumables = doc->AddMemberArray("consumables");
	for (const auto& consumable : m_ConsumablePool)
	{
		JSONValue consumObj = consumables.PushBackObject();
		consumObj.AddMember("name", consumable.second.Name.c_str());
		consumObj.AddMember("description", consumable.second.Description.c_str());
	}

}

void Wiwa::ItemManager::Deserialize(JSONDocument* doc)
{
	JSONDocument& document = *doc;
	if (doc->HasMember("actives"))
	{
		JSONValue actives = document["actives"];
		if (actives.IsArray())
		{
			for (size_t i = 0; i < actives.Size(); i++)
			{
				Wiwa::Ability ability;
				ability.Name = actives[i]["name"].as_string();
				ability.Description = actives[i]["description"].as_string();

				AddAbility(ability);
			}
		}
	}
	if (doc->HasMember("passives"))
	{
		JSONValue passives = document["passives"];
		if (passives.IsArray())
		{
			for (size_t i = 0; i < passives.Size(); i++)
			{
				Wiwa::PassiveSkill ability;
				ability.Name = passives[i]["name"].as_string();
				ability.Description = passives[i]["description"].as_string();

				AddPassive(ability);
			}
		}
	}
	if (doc->HasMember("buffs"))
	{
		JSONValue actives = document["buffs"];
		if (actives.IsArray())
		{
			for (size_t i = 0; i < actives.Size(); i++)
			{
				Wiwa::Buff ability;
				ability.Name = actives[i]["name"].as_string();
				ability.Description = actives[i]["description"].as_string();

				AddBuff(ability);
			}
		}
	}
	if (doc->HasMember("consumables"))
	{
		JSONValue actives = document["consumables"];
		if (actives.IsArray())
		{
			for (size_t i = 0; i < actives.Size(); i++)
			{
				Wiwa::Consumable ability;
				ability.Name = actives[i]["name"].as_string();
				ability.Description = actives[i]["description"].as_string();

				AddConsumable(ability);
			}
		}
	}
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

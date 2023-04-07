#include <wipch.h>
#include "ItemManager.h"

#include <Wiwa/utilities/json/JSONDocument.h>

std::map<std::string, Wiwa::Ability> Wiwa::ItemManager::m_AbilityPool;
std::map<std::string, Wiwa::PassiveSkill> Wiwa::ItemManager::m_PassiveSkillPool;
std::map<std::string, Wiwa::Buff> Wiwa::ItemManager::m_BuffPool;
std::map<std::string, Wiwa::Consumable> Wiwa::ItemManager::m_ConsumablePool;

void Wiwa::ItemManager::AddAbility(const Ability& ability)
{
	m_AbilityPool.emplace(ability.Name, ability);
}

void Wiwa::ItemManager::DeleteAbility(const std::string& name)
{
	m_AbilityPool.erase(name);
}

void Wiwa::ItemManager::AddPassive(const PassiveSkill& passive)
{
	m_PassiveSkillPool[passive.Name] = passive;
}

void Wiwa::ItemManager::DeletePassive(const std::string& name)
{
	m_PassiveSkillPool.erase(name);
}

void Wiwa::ItemManager::AddBuff(const Buff& buff)
{
	m_BuffPool[buff.Name] = buff;
}

void Wiwa::ItemManager::DeleteBuff(const std::string& name)
{
	m_BuffPool.erase(name);
}

void Wiwa::ItemManager::AddConsumable(const Consumable& consumable)
{
	m_ConsumablePool[consumable.Name] = consumable;
}

void Wiwa::ItemManager::DeleteConsumable(const std::string& name)
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
		activeObj.AddMember("path", Resources::getResourcePathById<Image>(active.second.Icon));
		activeObj.AddMember("damage", active.second.Damage);
		activeObj.AddMember("range", active.second.Range);
		activeObj.AddMember("area", active.second.Area);
		activeObj.AddMember("cooldown", active.second.Cooldown);
		activeObj.AddMember("price", active.second.Price);
		activeObj.AddMember("type", (int)active.second.AbilityType);
	}

	JSONValue passives = doc->AddMemberArray("passives");
	for (const auto& passive : m_PassiveSkillPool)
	{
		JSONValue passiveObj = passives.PushBackObject();
		passiveObj.AddMember("name", passive.second.Name.c_str());
		passiveObj.AddMember("description", passive.second.Description.c_str());
		passiveObj.AddMember("path", Resources::getResourcePathById<Image>(passive.second.Icon));
		passiveObj.AddMember("type", (int)passive.second.PassiveType);
	}

	JSONValue buffs = doc->AddMemberArray("buffs");
	for (const auto& buff : m_BuffPool)
	{
		JSONValue buffObj = buffs.PushBackObject();
		buffObj.AddMember("name", buff.second.Name.c_str());
		buffObj.AddMember("description", buff.second.Description.c_str());
		buffObj.AddMember("path", Resources::getResourcePathById<Image>(buff.second.Icon));
		buffObj.AddMember("buff_percent", buff.second.BuffPercent);
		buffObj.AddMember("duration", buff.second.Duration);
		buffObj.AddMember("cooldown", buff.second.Cooldown);
		buffObj.AddMember("price", buff.second.Price);
		buffObj.AddMember("type", (int)buff.second.buffType);
	}

	JSONValue consumables = doc->AddMemberArray("consumables");
	for (const auto& consumable : m_ConsumablePool)
	{
		JSONValue consumObj = consumables.PushBackObject();
		consumObj.AddMember("name", consumable.second.Name.c_str());
		consumObj.AddMember("path", Resources::getResourcePathById<Image>(consumable.second.Icon));
		consumObj.AddMember("type", (int)consumable.second.Type);
		consumObj.AddMember("buff_percent", consumable.second.BuffPercent);
		consumObj.AddMember("is_egos_help", consumable.second.IsEgosHelp);
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
				ability.Icon = Resources::Load<Image>(actives[i]["path"].as_string());
				ability.Damage = actives[i]["damage"].as_int();
				ability.Range = actives[i]["range"].as_float();
				ability.Area = actives[i]["area"].as_float();
				ability.Cooldown = actives[i]["cooldown"].as_float();
				ability.Price = actives[i]["price"].as_int();
				ability.AbilityType = (AbilityType)actives[i]["type"].as_int();
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
				ability.Icon = Resources::Load<Image>(passives[i]["path"].as_string());
				ability.PassiveType = (PassiveType)passives[i]["type"].as_int();
				AddPassive(ability);
			}
		}
	}
	if (doc->HasMember("buffs"))
	{
		JSONValue actives = document["buffs"];
		if (actives.IsArray())
		{
			for (uint32_t i = 0; i < actives.Size(); i++)
			{
				Wiwa::Buff ability;
				ability.Name = actives[i]["name"].as_string();
				ability.Description = actives[i]["description"].as_string();
				ability.Icon = Resources::Load<Image>(actives[i]["path"].as_string());
				ability.BuffPercent = actives[i]["buff_percent"].as_int();
				ability.Duration = actives[i]["duration"].as_float();
				ability.Cooldown = actives[i]["cooldown"].as_float();
				ability.Price = actives[i]["price"].as_int();
				ability.buffType = (BuffType)actives[i]["type"].as_int();
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
				ability.Type = (ConsumableType)actives[i]["type"].as_int();
				ability.Icon = Resources::Load<Image>(actives[i]["path"].as_string());
				ability.BuffPercent = actives[i]["buff_percent"].as_int();
				ability.IsEgosHelp = actives[i]["is_egos_help"].as_bool();
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

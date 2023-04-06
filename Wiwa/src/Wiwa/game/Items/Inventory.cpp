#include <wipch.h>
#include "Inventory.h"
#include <Wiwa/core/Input.h>

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
	JSONValue Abilites = doc->AddMemberArray("abilities");
	for (size_t i = 0; i < MAX_ABILITIES; i++)
	{
		if (m_Abilities[i])
		{
			JSONValue ability = Abilites.PushBackObject();
			ability.AddMember("name", m_Abilities[i]->Name.c_str());
			ability.AddMember("description", m_Abilities[i]->Description.c_str());
			ability.AddMember("icon", m_Abilities[i]->Icon);
			ability.AddMember("damage", m_Abilities[i]->Damage);
			ability.AddMember("range", m_Abilities[i]->Range);
			ability.AddMember("area", m_Abilities[i]->Area);
			ability.AddMember("cooldown", m_Abilities[i]->Cooldown);
			ability.AddMember("current_time", m_Abilities[i]->CurrentTime);
			ability.AddMember("price", m_Abilities[i]->Price);
			ability.AddMember("type", (int)m_Abilities[i]->abilityType);
			
		}
	}
	JSONValue Buffs = doc->AddMemberArray("buffs");
	for (size_t i = 0; i < MAX_BUFFS; i++)
	{
		if (m_Buffs[i] != NULL)
		{
			JSONValue buff = Buffs.PushBackObject();
			buff.AddMember("name", m_Buffs[i]->Name.c_str());
			buff.AddMember("description", m_Buffs[i]->Description.c_str());
			buff.AddMember("icon", m_Buffs[i]->Icon);
			buff.AddMember("buff_percent", m_Buffs[i]->BuffPercent);
			buff.AddMember("duration", m_Buffs[i]->Duration);
			buff.AddMember("cooldown", m_Buffs[i]->Cooldown);
			buff.AddMember("current_time", m_Buffs[i]->CurrentTime);
			buff.AddMember("cooldown_timer", m_Buffs[i]->CoolDownTimer);
			buff.AddMember("price", m_Buffs[i]->Price);
			buff.AddMember("type", (int)m_Buffs[i]->buffType);
			
		}
	}
	JSONValue Passives = doc->AddMemberArray("passives");
	if (!m_PassiveSkill.empty())
	{
		for (size_t i = 0; i < m_PassiveSkill.size(); i++)
		{
			JSONValue passive = Passives.PushBackObject();
			passive.AddMember("name", m_PassiveSkill.at(i).Name.c_str());
			passive.AddMember("description", m_PassiveSkill.at(i).Description.c_str());
			passive.AddMember("icon", m_PassiveSkill.at(i).Icon);
			passive.AddMember("type", (int)m_PassiveSkill.at(i).passiveType);
			
		}
	}
	
	/*JSONValue Tokens = doc->AddMemberObject("tokens");
	Tokens.AddMember("tokens", m_Tokens);*/
	
	doc->save_file("config/player_data.json");
}

void Wiwa::Inventory::Deserialize(JSONDocument* doc)
{
	for (size_t i = 0; i < MAX_ABILITIES; i++)
	{
		
		if (doc->HasMember("abilities")) {

			JSONValue abilities = (*doc)["abilities"];
			if (abilities.IsArray())
			{
				for (size_t i = 0; i < abilities.Size(); i++)
				{
					Wiwa::Ability ability;
					ability.Name = abilities[i]["name"].as_string();
					ability.Description = abilities[i]["description"].as_string();
					ability.Icon = abilities[i]["icon"];
					ability.Damage = abilities[i]["damage"].as_int();
					ability.Range = abilities[i]["range"].as_float();
					ability.Area = abilities[i]["area"].as_float();
					ability.Cooldown = abilities[i]["cooldown"].as_float();
					ability.CurrentTime = abilities[i]["current_time"].as_float();
					ability.Price = abilities[i]["price"].as_float();
					ability.abilityType = (AbilityType)abilities[i]["type"].as_int();
					AddAbility(&ability);
				}
			}
		}
	}
	for (size_t i = 0; i < MAX_BUFFS; i++)
	{
		if (doc->HasMember("buffs")) {

			JSONValue buffs = (*doc)["buffs"];
			if (buffs.IsArray())
			{
				for (size_t i = 0; i < buffs.Size(); i++)
				{
					Wiwa::Buff buff;
					buff.Name = buffs[i]["name"].as_string();
					buff.Description = buffs[i]["description"].as_string();
					buff.Icon = buffs[i]["icon"];
					buff.BuffPercent = buffs[i]["buff_percent"].as_int();
					buff.Duration = buffs[i]["duration"].as_float();
					buff.Cooldown = buffs[i]["cooldown"].as_float();
					buff.CurrentTime = buffs[i]["current_time"].as_float();
					buff.CoolDownTimer = buffs[i]["cooldown_timer"].as_float();
					buff.Price = buffs[i]["price"].as_float();
					buff.buffType = (BuffType)buffs[i]["type"].as_int();
					AddBuff(&buff);
				}
			}
		}
	}
	
	if (doc->HasMember("passives"))
	{
		JSONValue passives = (*doc)["passives"];
		if (passives.IsArray())
		{
			for (size_t i = 0; i < passives.Size(); i++)
			{
				Wiwa::PassiveSkill passive;
				passive.Name = passives[i]["name"].as_string();
				passive.Description = passives[i]["description"].as_string();
				passive.Icon = passives[i]["icon"];
				passive.passiveType = (PassiveType)passives[i]["type"].as_int();
				AddPassive(passive);
			}
		}
	}
	/*if (doc->HasMember("tokens")) {

		JSONValue tokens = (*doc)["tokens"];
		m_Tokens = tokens.as_int();

	}*/
}

void Wiwa::Inventory::InitGame()
{
    m_Abilities = new Ability*[MAX_ABILITIES] { nullptr, nullptr};
    m_Buffs = new Buff*[MAX_BUFFS] {nullptr, nullptr};
	m_Tokens = 0;
}

void Wiwa::Inventory::AddAbility(Ability* ability)
{
    for (size_t i = 0; i < MAX_ABILITIES; i++)
    {
		if (m_Abilities[i] != ability)
		{
			m_Abilities[i] = ability;
			break;
		}
    }
    
}

void Wiwa::Inventory::AddBuff(Buff* buff)
{
    for (size_t i = 0; i < MAX_BUFFS; i++)
    {
        if (m_Buffs[i] != buff)
        {
            m_Buffs[i] = buff;
            break;
        }
    }
}

void Wiwa::Inventory::AddPassive(const PassiveSkill& skill)
{
    m_PassiveSkill.emplace_back(skill);
}

void Wiwa::Inventory::AddConsumable(const Consumable& consumable)
{
	m_Consumable = consumable;
	m_Consumable.Use();
}

void Wiwa::Inventory::Update()
{
	
}

void Wiwa::Inventory::UseAbility(size_t index)
{
	m_Abilities[index]->Use();
}
void Wiwa::Inventory::UseBuff(size_t index)
{
	m_Buffs[index]->Use();
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

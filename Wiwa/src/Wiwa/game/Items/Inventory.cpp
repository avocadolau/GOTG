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

	doc->AddMember("Items",1);
	for (size_t i = 0; i < MAX_ABILITIES; i++)
	{
		if (m_Abilities[i] != NULL)
		{
			std::string key = "abilities" + std::to_string(i);
			JSONValue Abilites = doc->AddMemberObject(key.c_str());
			Abilites.AddMember("name", m_Abilities[i]->Name.c_str());
			Abilites.AddMember("description", m_Abilities[i]->Description.c_str());
			Abilites.AddMember("icon", m_Abilities[i]->Icon);
			Abilites.AddMember("damage", m_Abilities[i]->Damage);
			Abilites.AddMember("range", m_Abilities[i]->Range);
			Abilites.AddMember("area", m_Abilities[i]->Area);
			Abilites.AddMember("cooldown", m_Abilities[i]->Cooldown);
			Abilites.AddMember("current_time", m_Abilities[i]->CurrentTime);
			Abilites.AddMember("price", m_Abilities[i]->Price);
		}
	}
	for (size_t i = 0; i < MAX_BUFFS; i++)
	{
		if (m_Buffs[i] != NULL)
		{
			std::string key = "buff" + std::to_string(i);
			JSONValue Buffs = doc->AddMemberObject(key.c_str());
			Buffs.AddMember("name", m_Buffs[i]->Name.c_str());
			Buffs.AddMember("description", m_Buffs[i]->Description.c_str());
			Buffs.AddMember("icon", m_Buffs[i]->Icon);
			Buffs.AddMember("buff_percent", m_Buffs[i]->BuffPercent);
			Buffs.AddMember("duration", m_Buffs[i]->Duration);
			Buffs.AddMember("cooldown", m_Buffs[i]->Cooldown);
			Buffs.AddMember("current_time", m_Buffs[i]->CurrentTime);
			Buffs.AddMember("cooldown_timer", m_Buffs[i]->CoolDownTimer);
			Buffs.AddMember("price", m_Buffs[i]->Price);
		}
	}
	
	if (!m_PassiveSkill.empty())
	{
		for (size_t i = 0; i < m_PassiveSkill.size(); i++)
		{
			std::string key = "passive" + std::to_string(i);
			JSONValue passive_object = doc->AddMemberObject(key.c_str());
			passive_object.AddMember("name", m_PassiveSkill.at(i).Name.c_str());
			passive_object.AddMember("description", m_PassiveSkill.at(i).Description.c_str());
			passive_object.AddMember("icon", m_PassiveSkill.at(i).Icon);
		}
	}
	
	/*JSONValue Tokens = doc->AddMemberObject("tokens");
	Tokens.AddMember("tokens", m_Tokens);*/
	
	doc->save_file("config/player_data.json");
}

void Wiwa::Inventory::Deserialize(JSONDocument* doc)
{
	doc->load_file("config/player_data.json");
	if (doc->HasMember("Items")) {
		
	}
	for (size_t i = 0; i < MAX_ABILITIES; i++)
	{
		std::string key = "abilities" + std::to_string(i);
		if (doc->HasMember(key.c_str())) {

			JSONValue ability = (*doc)[key.c_str()];
			m_Abilities[i]->Name = ability["name"].as_string();
			m_Abilities[i]->Description = ability["description"].as_string();
			m_Abilities[i]->Icon = ability["icon"];
			m_Abilities[i]->Damage = ability["damage"].as_int();
			m_Abilities[i]->Range = ability["range"].as_float();
			m_Abilities[i]->Area = ability["area"].as_float();
			m_Abilities[i]->Cooldown = ability["cooldown"].as_float();
			m_Abilities[i]->CurrentTime = ability["current_time"].as_float();
			m_Abilities[i]->Price = ability["price"].as_int();
		}
	}
	for (size_t i = 0; i < MAX_BUFFS; i++)
	{
		std::string key = "buff" + std::to_string(i);
		if (doc->HasMember(key.c_str())) {

			JSONValue buff = (*doc)[key.c_str()];
			m_Buffs[i]->Name = buff["name"].as_string();
			m_Buffs[i]->Description = buff["description"].as_string();
			m_Buffs[i]->Icon = buff["icon"];
			m_Buffs[i]->BuffPercent = buff["buff_percent"].as_int();
			m_Buffs[i]->Duration = buff["duration"].as_float();
			m_Buffs[i]->Cooldown = buff["cooldown"].as_float();
			m_Buffs[i]->CurrentTime = buff["current_time"].as_float();
			m_Buffs[i]->CoolDownTimer = buff["cooldown_timer"].as_float();
			m_Buffs[i]->Price = buff["price"].as_int();
		}
	}
	for (size_t i = 0; i < m_PassiveSkill.size(); i++)
	{
		std::string key = "passive" + std::to_string(i);
		if (doc->HasMember(key.c_str())) {

			JSONValue buff = (*doc)[key.c_str()];
			m_PassiveSkill.at(i).Name = buff["name"].as_string();
			m_PassiveSkill.at(i).Description = buff["description"].as_string();
			m_PassiveSkill.at(i).Icon = buff["icon"];
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
        if (m_Abilities[i] == nullptr)
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
        if (m_Buffs[i] == nullptr)
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

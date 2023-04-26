#include <wipch.h>
#include "Inventory.h"
#include <Wiwa/core/Input.h>

#include "Wiwa/core/KeyCodes.h"
#include "Wiwa/utilities/time/Time.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/core/Application.h"
#include <Wiwa/ecs/components/game/items/Item.h>

#define MAX_ABILITIES 2
#define MAX_BUFFS 2

Wiwa::Inventory::Inventory()
{
    InitGame();
}

Wiwa::Inventory::~Inventory() = default;

void Wiwa::Inventory::Serialize(JSONDocument* doc)
{
	JSONValue abilities = doc->AddMemberArray("abilities");
	for (size_t i = 0; i < MAX_ABILITIES; i++)
	{
		if (m_Abilities[i])
		{
			JSONValue ability = abilities.PushBackObject();
			ability.AddMember("name", m_Abilities[i]->Name.c_str());
			ability.AddMember("description", m_Abilities[i]->Description.c_str());
			ability.AddMember("icon", (int)m_Abilities[i]->Icon);
			ability.AddMember("damage", m_Abilities[i]->Damage);
			ability.AddMember("range", m_Abilities[i]->Range);
			ability.AddMember("area", m_Abilities[i]->Area);
			ability.AddMember("cooldown", m_Abilities[i]->Cooldown);
			ability.AddMember("current_time", m_Abilities[i]->CurrentTime);
			ability.AddMember("price", m_Abilities[i]->Price);
			ability.AddMember("type", (int)m_Abilities[i]->AbilityType);
			
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
			buff.AddMember("icon", (int)m_Buffs[i]->Icon);
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
			passive.AddMember("icon", (int)m_PassiveSkill.at(i).Icon);
			passive.AddMember("type", (int)m_PassiveSkill.at(i).PassiveType);
			
		}
	}
	

	doc->AddMember("tokens", m_Tokens);
}

void Wiwa::Inventory::Deserialize(JSONDocument* doc)
{

	if (doc->HasMember("abilities")) {

		JSONValue abilities = (*doc)["abilities"];
		if (abilities.IsArray())
		{
			for (uint32_t i = 0; i < abilities.Size(); i++)
			{
				Wiwa::Ability ability;
				ability.Name = abilities[i]["name"].as_string();
				ability.Description = abilities[i]["description"].as_string();
				ability.Icon = (ResourceId)abilities[i]["icon"].as_int();
				ability.Damage = abilities[i]["damage"].as_int();
				ability.Range = abilities[i]["range"].as_float();
				ability.Area = abilities[i]["area"].as_float();
				ability.Cooldown = abilities[i]["cooldown"].as_float();
				ability.CurrentTime = abilities[i]["current_time"].as_float();
				ability.Price = abilities[i]["price"].as_int();
				ability.AbilityType = (AbilityType)abilities[i]["type"].as_int();
				AddAbility(&ability);
			}
		}
	}

	if (doc->HasMember("buffs"))
	{
		JSONValue buffs = (*doc)["buffs"];
		if (buffs.IsArray())
		{
			for (uint32_t i = 0; i < buffs.Size(); i++)
			{
				Wiwa::Buff buff;
				buff.Name = buffs[i]["name"].as_string();
				buff.Description = buffs[i]["description"].as_string();
				buff.Icon = (ResourceId)buffs[i]["icon"].as_int();
				buff.BuffPercent = buffs[i]["buff_percent"].as_int();
				buff.Duration = buffs[i]["duration"].as_float();
				buff.Cooldown = buffs[i]["cooldown"].as_float();
				buff.CurrentTime = buffs[i]["current_time"].as_float();
				buff.CoolDownTimer = buffs[i]["cooldown_timer"].as_float();
				buff.Price = buffs[i]["price"].as_int();
				buff.buffType = (BuffType)buffs[i]["type"].as_int();
				AddBuff(&buff);
			}
		}
	}
	
	if (doc->HasMember("passives"))
	{
		if (JSONValue passives = (*doc)["passives"]; passives.IsArray())
		{
			for (uint32_t i = 0; i < passives.Size(); i++)
			{
				Wiwa::PassiveSkill passive;
				passive.Name = passives[i]["name"].as_string();
				passive.Description = passives[i]["description"].as_string();
				passive.Icon = (ResourceId)passives[i]["icon"].as_int();
				passive.PassiveType = (PassiveType)passives[i]["type"].as_int();
				AddPassive(passive);
			}
		}
	}
	if (doc->HasMember("tokens")) {
		m_Tokens = (*doc)["tokens"].as_int();
	}
}

void Wiwa::Inventory::InitGame()
{
    m_Abilities = new Ability*[MAX_ABILITIES] { nullptr, nullptr};
    m_Buffs = new Buff*[MAX_BUFFS] {nullptr, nullptr};
	m_Tokens = 0;
}

void Wiwa::Inventory::AddAbility(const Ability* ability) const
{
	if (!ability)
		return;
	// If the first slot is occupied shift the ability one place
	if(m_Abilities[0])
	{
		// TODO: Instead of deleting just spawn the ability on the floor
		
		if (m_Abilities[1])
		{
			GameStateManager::SpawnItem(
				GameStateManager::GetPlayerTransform()->position + glm::vec3(0.f, 0.f, 3.f), 0, m_Abilities[1]->Name.c_str());
			delete m_Abilities[1];
		}
		m_Abilities[1] = m_Abilities[0];
	}
	
	m_Abilities[0] = new Ability(*ability);
}

void Wiwa::Inventory::AddBuff(const Buff* buff) const
{
	if (!buff)
		return;

	// If the first slot is occupied shift the buff one place
	if(m_Buffs[0])
	{
		// TODO: Instead of deleting just spawn the buff on the floor
		if (m_Buffs[1])
		{

			GameStateManager::SpawnItem(
				GameStateManager::GetPlayerTransform()->position + glm::vec3(0.f, 0.f, 3.f), 2, m_Buffs[1]->Name.c_str());
			delete m_Buffs[1];
		}
		m_Buffs[1] = m_Buffs[0];
	}

	m_Buffs[0] = new Buff(*buff);
	m_Buffs[0]->IsActive = false;
}

void Wiwa::Inventory::AddPassive(const PassiveSkill& skill)
{
    m_PassiveSkill.emplace_back(skill);
	m_PassiveSkill.back().Use();
}

void Wiwa::Inventory::AddConsumable(const Consumable& consumable)
{
	m_Consumable = consumable;
	m_Consumable.Use();
}

void Wiwa::Inventory::Update()
{
	if(Time::IsPlaying())
	{
		// Input
		const float rightTrigger = Input::GetAxis(Gamepad::GamePad1, Wiwa::Gamepad::RightTrigger);
		const float leftTrigger = Input::GetAxis(Gamepad::GamePad1, Gamepad::LeftTrigger);

		// Ability 1
		if(m_Abilities[0])
		{
			SwapUITexture(m_Abilities[0]->Icon, 3);
			m_Abilities[0]->CurrentTime += Time::GetDeltaTimeSeconds();
			if(Input::IsKeyPressed(Key::Q) || leftTrigger >= 0.f)
			{	
				UseAbility(0);
			}
		}
		if(m_Abilities[1])
		{
			SwapUITexture(m_Abilities[1]->Icon, 4);

			m_Abilities[1]->CurrentTime += Time::GetDeltaTimeSeconds();
			if(Input::IsKeyPressed(Key::E) || rightTrigger >= 0.f)
			{
				WI_CORE_INFO("Ability 2 activated");
				UseAbility(1);
			}
		}

		if(m_Buffs[0])
		{
			SwapUITexture(m_Buffs[0]->Icon, 5);
			if (m_Buffs[0]->IsActive)
			{
				m_Buffs[0]->CoolDownTimer += Time::GetDeltaTimeSeconds();
				if (m_Buffs[0]->CoolDownTimer >= m_Buffs[0]->Duration)
					m_Buffs[0]->UnUse();
			}
			else
				m_Buffs[0]->CurrentTime += Time::GetDeltaTimeSeconds();
			if(Input::IsKeyPressed(Key::R) || Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadX))
			{
				WI_CORE_INFO("Buff 1 activated");
				UseBuff(0);
			}
		}
		if(m_Buffs[1])
		{
			SwapUITexture(m_Buffs[1]->Icon, 6);
			if (m_Buffs[1]->IsActive)
			{
				m_Buffs[1]->CoolDownTimer += Time::GetDeltaTimeSeconds();
				if(m_Buffs[1]->CoolDownTimer >= m_Buffs[1]->Duration)
					m_Buffs[1]->UnUse();
			}
			else
				m_Buffs[1]->CurrentTime += Time::GetDeltaTimeSeconds();
			
			if(Input::IsKeyPressed(Key::F) || Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
			{
				WI_CORE_INFO("Buff 2 activated");
				UseBuff(1);
			}
		}
		
	}
}

void Wiwa::Inventory::UseAbility(size_t index) const
{
	if(m_Abilities[index]->CurrentTime >= m_Abilities[index]->Cooldown)
	{
		Wiwa::EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
		EntityId player = em.GetEntityByName("Player");

		if (player)
		{
			//EntityId pe_spark = em.GetChildByName(player, "PE_Use_Ability_Spark");
			//EntityId pe_line = em.GetChildByName(player, "PE_Use_Ability_Line");

		}

		m_Abilities[index]->CurrentTime = 0.f;
		m_Abilities[index]->Use();
		return;
	}
	WI_CORE_INFO("Ability {} is on cooldown", index);
}
void Wiwa::Inventory::UseBuff(size_t index) const
{
	if(m_Buffs[index]->CurrentTime >= m_Buffs[index]->Cooldown)
	{
		m_Buffs[index]->CurrentTime = 0.f;
		m_Buffs[index]->Use();
		
		return;
	}
	WI_CORE_INFO("Buff {} is on cooldown", index);
}

void Wiwa::Inventory::SwapUITexture(ResourceId id, int indexUI)
{
	/*Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	if (!em.GetEntitiesAlive()->empty())
	{
		Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		gm.canvas.at(0)->controls.at(indexUI)->texture = Wiwa::Resources::GetResourceById<Image>(id);
		r2d.UpdateInstancedQuadTexTexture(Wiwa::SceneManager::getActiveScene(), gm.canvas.at(0)->controls.at(indexUI)->id_quad_normal, gm.canvas.at(0)->controls.at(indexUI)->texture->GetTextureId());
	}*/
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

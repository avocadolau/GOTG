#include "wipch.h"
#include "CharacterInventory.h"
#include "Wiwa/game/Items/ItemManager.h"
#include "Wiwa/core/Input.h"
#include "Wiwa/ecs/components/game/items/Item.h"
#include "Wiwa/audio/Audio.h"

void Wiwa::CharacterInventory::OnInit()
{
	shopActive = false;
	Character* character = Wiwa::GameStateManager::GetPlayerCharacterComp();
	lastCharacter.Damage = 0;
	lastCharacter.MaxHealth = 0;
	lastCharacter.MaxShield = 0;
	lastCharacter.Speed = 0.0f;
	lastCharacter.RateOfFire = 0.0f;
}

void Wiwa::CharacterInventory::OnUpdate()
{
	if (shopActive)
	{
		ShopElement(currentItem);
		shopActive = false;
	}
}

void Wiwa::CharacterInventory::ShopElement(Item* item)
{
	size_t currentTokens = Wiwa::GameStateManager::GetPlayerInventory().GetTokens();
	if (item->item_type == 0)
	{
		//Abilities
		Ability* ability = Wiwa::ItemManager::GetAbility(item->Name);
		if (ability->Price <= currentTokens)
		{
			Wiwa::GameStateManager::GetPlayerInventory().SubstractTokens(ability->Price);
			Wiwa::GameStateManager::s_PlayerInventory->AddAbility(ability);
		}
	}
	if (item->item_type == 2)
	{
		//Buffs
		Buff* buff = Wiwa::ItemManager::GetBuff(item->Name);
		if (buff->Price <= currentTokens)
		{
			Wiwa::GameStateManager::GetPlayerInventory().SubstractTokens(buff->Price);
			Wiwa::GameStateManager::s_PlayerInventory->AddBuff(buff);
		}
	}

}

void Wiwa::CharacterInventory::OnCollisionEnter(Object* body1, Object* body2)
{
	std::string tag_item = "ITEM";
	if (body1->id == m_EntityId && tag_item == body2->selfTagStr)
	{
		Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		Wiwa::EntityManager& em = _scene->GetEntityManager();
		Item* item = em.GetComponent<Item>(body2->id);
		if (!item)
		{
			WI_CORE_ERROR("Item component can't be find");
			return;
		}

		if (GameStateManager::GetRoomType() == "ROOM_SHOP")//ABILITY
		{
			currentItem = item;
			currentItemShopId = body2->id;

			return;
		}
		 
		if (item->item_type == 0)
		{
			Ability* ability = Wiwa::ItemManager::GetAbility(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddAbility(ability);
			em.DestroyEntity(body2->id);

		}
		else if (item->item_type == 1)//PASSIVE
		{
			PassiveSkill* passive = Wiwa::ItemManager::GetPassive(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddPassive(*passive);
			passive->Use();
			em.DestroyEntity(body2->id);
		}
		else if (item->item_type == 2)
		{
			Buff* buff = Wiwa::ItemManager::GetBuff(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddBuff(buff);
			em.DestroyEntity(body2->id);
		}
		else if (item->item_type == 3)//CONSUMABLE
		{
			Consumable* consumable = Wiwa::ItemManager::GetConsumable(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddConsumable(*consumable);
			em.DestroyEntity(body2->id);
		}


		// play audio
		if (GameStateManager::s_CurrentCharacter == STARLORD)
		{
			Audio::PostEvent("vo_starlord_powerup");
		}
		else if (GameStateManager::s_CurrentCharacter == ROCKET)
		{
			Audio::PostEvent("vo_rocket_powerup");
		}
	}
}

bool Wiwa::CharacterInventory::PlayerUpdated()
{
	Character* current_character = Wiwa::GameStateManager::GetPlayerCharacterComp();

	if (lastCharacter.Damage != current_character->Damage)
	{
		lastCharacter.Damage = current_character->Damage;
		return true;
	}
	else if (lastCharacter.MaxHealth != current_character->MaxHealth)
	{
		lastCharacter.MaxHealth = current_character->MaxHealth;
		return true;
	}
	else if (lastCharacter.MaxShield != current_character->MaxShield)
	{
		lastCharacter.MaxShield = current_character->MaxShield;
		return true;
	}
	else if (lastCharacter.Speed != current_character->Speed)
	{
		lastCharacter.Speed = current_character->Speed;
		return true;
	}
	else if (lastCharacter.RateOfFire != current_character->RateOfFire)
	{
		lastCharacter.RateOfFire = current_character->RateOfFire;
		return true;
	}
	else
	{
		return false;
	}
}

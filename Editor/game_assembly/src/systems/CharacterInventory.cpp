#include "wipch.h"
#include "CharacterInventory.h"
#include "Wiwa/game/Items/ItemManager.h"
#include "Wiwa/core/Input.h"
#include "../Components.h"
#include "../components/items/Item.h"

void Wiwa::CharacterInventory::OnUpdate()
{
	if (shopActive)
	{
		ShopElement(currentItem);
	}

	if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
	{
		releasingShop = true;
	}
	if (releasingShop && Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB))
	{
		releasingShop = false;
		shopActive = false;
	}
}

void Wiwa::CharacterInventory::ShopElement(Item* item)
{
	size_t currentTokens = Wiwa::GameStateManager::GetPlayerInventory().GetTokens();

	//ADD ITEM IF TOKENS ARE ENOUGH
	if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
	{
		buyItem = true;
	}
	if (buyItem && Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadA))
	{
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
		

		buyItem = false;
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

		if (item->item_type == 0)//ABILITY
		{
			currentItem = item;
			shopActive = true;
		}
		else if (item->item_type == 1)//PASSIVE
		{
			PassiveSkill* passive = Wiwa::ItemManager::GetPassive(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddPassive(*passive);
			passive->Use();
			em.DestroyEntity(body2->id);
		}
		else if (item->item_type == 2)//BUFF
		{
			currentItem = item;
			shopActive = true;
		}
		else if (item->item_type == 3)//CONSUMABLE
		{
			Consumable* consumable = Wiwa::ItemManager::GetConsumable(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddConsumable(*consumable);
			em.DestroyEntity(body2->id);
		}
	}
}

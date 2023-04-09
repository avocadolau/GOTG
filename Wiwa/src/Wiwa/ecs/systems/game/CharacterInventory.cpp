#include "wipch.h"
#include "CharacterInventory.h"
#include "Wiwa/ecs/components/game/items/Item.h"
#include "Wiwa/game/Items/ItemManager.h"
#include "Wiwa/core/Input.h"

void Wiwa::CharacterInventory::OnUpdate()
{
	if (Wiwa::Input::IsButtonPressed(0, 13))
	{
		Wiwa::GameStateManager::s_PlayerInventory->UseAbility(0);
	}
}

void Wiwa::CharacterInventory::OnCollisionEnter(Object* body1, Object* body2)
{
	if (body1->id == m_EntityId && strcmp(body2->selfTagStr, "ITEM") == 0)
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
			Ability* ability = Wiwa::ItemManager::GetAbility(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddAbility(ability);
		}
		else if (item->item_type == 1)//PASSIVE
		{
			PassiveSkill* passive = Wiwa::ItemManager::GetPassive(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddPassive(*passive);
			passive->Use();
		}
		else if (item->item_type == 2)//BUFF
		{
			Buff* buff = Wiwa::ItemManager::GetBuff(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddBuff(buff);
		}
		else if (item->item_type == 3)//CONSUMABLE
		{
			Consumable* consumable = Wiwa::ItemManager::GetConsumable(item->Name);
			Wiwa::GameStateManager::s_PlayerInventory->AddConsumable(*consumable);
		}
	}
}

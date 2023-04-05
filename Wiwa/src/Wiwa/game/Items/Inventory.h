#pragma once

#include <Wiwa/core/Core.h>
#include "Item.h"

#include <vector>

namespace Wiwa
{
	class JSONDocument;
	class WI_API Inventory
	{
	protected:
		Inventory();
		~Inventory();
	public:
		void Serialize(JSONDocument* doc);
		void Deserialize(JSONDocument* doc);

		void InitGame();
		void AddAbility(Ability* ability);
		void AddBuff(Buff* buff);
		void AddPassive(const PassiveSkill& skill);
		void AddConsumable(const Consumable& consumable);
		void Update();
		
		void UseAbility(size_t index);
		void UseBuff(size_t index);
		void Clear();

	private:
		Ability** m_Abilities;
		Buff** m_Buffs;
		Consumable m_Consumable;
		std::vector<PassiveSkill> m_PassiveSkill;
		int m_Tokens;
		friend class GameStateManager;
	};
}
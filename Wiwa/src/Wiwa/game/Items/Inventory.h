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
		void AddPassive(const PassiveSkill& skill);

		void Update();
		
		void UseAbility(size_t index);

		void Clear();

	private:
		Ability** m_Abilities;
		Buff** m_Buffs;
		std::vector<PassiveSkill> m_PassiveSkill;
		
		friend class GameStateManager;
	};
}
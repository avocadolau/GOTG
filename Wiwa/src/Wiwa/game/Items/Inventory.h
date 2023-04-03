#pragma once

#include <Wiwa/core/Core.h>
#include "Item.h"

#include <vector>

namespace Wiwa
{
	class WI_API Inventory
	{
		void Serialize();
		void Deserialize();



	private:
		Ability* m_Abilities[2];
		std::vector<PassiveSkill> m_PassiveSkill;
	};
}
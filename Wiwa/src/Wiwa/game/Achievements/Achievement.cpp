#include "wipch.h"
#include "Achievement.h"

namespace Wiwa
{
	Achievement::Achievement()
	{
		m_Name = "";
		m_Properties = {};
		m_Unlocked = false;
		m_NumTier = 0;
		m_Tier = 0;
		m_CurrentCost = 0;
		m_Costs = {};
	}

	Achievement::Achievement(std::string name, std::string description, int num_tier, std::vector<int> costs, std::vector<Property> properties)
	{
		m_Name = name;
		m_NumTier = num_tier;
		m_Costs = costs;
		m_Properties = properties;
		m_Unlocked = false;
		m_Tier = 0;
		m_CurrentCost = costs[0];
	}

	void Achievement::UpdateCost()
	{
		m_CurrentCost = m_Costs[m_Tier];
	}

	bool Achievement::Acquire()
	{
		if (m_Tier >= m_NumTier)
		{
			false;
		}
		m_Tier++;
		UpdateCost();
		return true;
	}
}
#include "wipch.h"
#include "Achivement.h"

namespace Wiwa
{
	Achivement::Achivement()
	{
		m_Name = "";
		m_Porperties = {};
		m_Unlocked = false;
		m_MaxCap = 0;
		m_Count = 0;
		m_CurrentCost = 0;
		m_Costs = {};
	}

	Achivement::Achivement(std::string name, std::string description, int max_capacity, std::vector<int> costs, std::vector<Property> properties)
	{
		m_Name = name;
		m_MaxCap = max_capacity;
		m_Costs = costs;
		m_Porperties = properties;
		m_Unlocked = false;
		m_Count = 0;
		m_CurrentCost = costs[0];
	}

	void Achivement::UpdateCost()
	{
		m_CurrentCost = m_Costs[m_Count];
	}

	bool Achivement::Aquire()
	{
		if (m_Count >= m_MaxCap)
		{
			false;
		}
		m_Count++;
		UpdateCost();
		return true;
	}
}
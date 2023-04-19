#pragma once
#include "Property.h"
namespace Wiwa {
	class Achievement {
	public:
		Achievement();
		Achievement(std::string name, std::string description, int max_capacity, std::vector<int> costs, std::vector<Property> properties);
		~Achievement() {};
		void UpdateCost();

		bool Acquire();

		//Achievements names
		std::string m_Name;
		//Achievements names
		std::string m_Description;
		//properties to be able to unlock it
		std::vector<Property> m_Properties;//change
		//check if its unlocked yet
		bool m_Unlocked;
		//max capacity of tiers
		int m_NumTier;
		//current tier
		int m_Tier;
		//current cost of aquirement
		int m_CurrentCost;
		//array of cost as can have many costs for each aquirement
		std::vector<int> m_Costs;
	};
}
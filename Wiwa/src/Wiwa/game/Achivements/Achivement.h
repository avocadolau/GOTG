#pragma once
#include "Property.h"
namespace Wiwa {
	class Achivement {
	public:
		Achivement();
		Achivement(std::string id, int max_capacity, std::vector<int> costs, std::vector<Property> properties);

		void UpdateCost();

		bool Aquire();

		//achivements names
		std::string m_Name;
		//properties to be able to unlock it
		std::vector<Property> m_Porperties;
		//check if its unlocked yet
		bool m_Unlocked;
		//max capacity of aquirements
		int m_MaxCap;
		//number of times the achivement boos t is quired
		int m_Count;
		//current cost of aquirement
		int m_CurrentCost;
		//array of cost as can have many costs for each aquirement
		std::vector<int> m_Costs;
	};
}
#pragma once
#include <Wiwa/core/Resources.h>
#include <string>
#include <map>

namespace Wiwa {
	enum class ActivationRules {
		ACTIVE_IF_GREATER_THAN,
		ACTIVE_IF_LESS_THAN,
		ACTIVE_IF_EQUALS_THAN
	};

	class Property {
	public:
		Property() {

		};
		Property(std::string name, int initialValue, ActivationRules activation, int activationvalue)
		{
			m_Name = name;
			m_InitialValue = initialValue;
			m_Activation = activation;
			m_ActivationValue = activationvalue;
		};
		~Property() {};

		bool IsActive() {
			bool ret = false;
			switch (m_Activation)
			{
			case ActivationRules::ACTIVE_IF_GREATER_THAN:
				ret = m_Value > m_ActivationValue;
				break;
			case ActivationRules::ACTIVE_IF_LESS_THAN:
				ret = m_Value < m_ActivationValue;
				break;
			case ActivationRules::ACTIVE_IF_EQUALS_THAN:
				ret = m_Value == m_ActivationValue;
				break;
			default:
				break;
			}
			return ret;
		}

		std::string m_Name;
		ActivationRules m_Activation;
		int m_Value;
		int m_ActivationValue;
		int m_InitialValue;
	};

	class Achivement {
	public:
		Achivement(std::string id, int max_capacity, std::vector<int> costs, std::vector<Property> properties)
		{
			m_Name = id;
			m_MaxCap = max_capacity;
			m_Costs = costs;
			m_Porperties = properties;
			m_Unlocked = false;
			m_Count = 0;
			m_CurrentCost = costs[0];
		}

		void UpdateCost()
		{
			m_CurrentCost = m_Costs[m_Count];
		}


		bool Aquire()
		{
			if (m_Count >= m_MaxCap)
			{
				false;
			}
			m_Count++;
			UpdateCost();
			return true;
		}
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

	class AchivementManager {
	public:
		AchivementManager() {
			m_Properties = {};
			m_Achivements = {};
		}
		//adds a property
		void DefineProperty(std::string name, int initialValue, ActivationRules activationMode, int value)
		{
			m_Properties[name] = Property(name, initialValue, activationMode, value);
		}
		//adds a Miscellaneous achivement 
		void DefineAchivementAchivement(std::string name, std::vector<Property> properties)
		{
			//m_Achivements[name] = Achivement(name, properties);
		}
		//retruns the value of the property
		int GetValue(std::string name)
		{
			auto item = m_Properties.find(name);

			if (item != m_Properties.end())
				return item->second.m_Value;
			else
				return -1;
		}
		//sets the current value to the property
		void SetValue(std::string name, int value)
		{
			auto item = m_Properties.find(name);

			if (item != m_Properties.end())
				item->second.m_Value = value;
			else
				return;
		}
		//adds a value to the current m_value 
		void AddValue(std::vector<std::string> properties, int value)
		{
			for (int i = 0; i < properties.size(); i++)
			{
				std::string item = properties[i];
				SetValue(item, GetValue(item) + value);
			}
		}

		std::vector<Achivement*> CheckAchivements() {
			std::vector<Achivement*> result;

			for (auto achivement : m_Achivements)
			{
				if (!achivement.second.m_Unlocked)
				{
					int activeProperties = 0;

					for (auto prop : achivement.second.m_Porperties)
					{
						if (prop.IsActive())
						{
							activeProperties++;
						}
					}
					if (achivement.second.m_Porperties.size() == activeProperties)
					{
						result.push_back(&achivement.second);
					}
				}
			}
			return result;
		}
	private:
		//are used to determine if an Achivement is active or not
		std::map<std::string, Property> m_Properties;
		//are used to determine if some extra boost are can be applied
		std::map<std::string, Achivement> m_Achivements;
	};
}
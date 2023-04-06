#pragma once

#include <Wiwa/core/Core.h>

#include <string>

#include <map>

#include "Achivement.h"

namespace Wiwa {
	class JSONDocument;
	class WI_API AchivementsManager {
	private:
		AchivementsManager() = delete;
		~AchivementsManager() = delete;
	public:
		//adds a property
		static void DefineProperty(std::string name, int initialValue, ActivationRules activationMode, int value);
		//adds a Miscellaneous achivement 
		static void DefineAchivement(std::string name, int max_capacity, std::vector<int> costs, std::vector<Property> properties);
		//retruns the value of the property
		static int GetValue(std::string name);
		//sets the current value to the property
		static void SetValue(std::string name, int value);
		//adds a value to the current m_value 
		static void AddValue(std::vector<std::string> properties, int value);
		//pass by all the achivements and checks if they are completed
		static std::vector<Achivement*> CheckAchivements();
		//save the achivements
		static void Serialize(JSONDocument* doc);
		//load achivements
		static void Deserialize(JSONDocument* doc);
		//gets all the properties list (rules that need to be completed before unlocking an achivement)
		static 	std::map<std::string, Property> GetProperties() { return m_Properties; }
		//gets the achivements list
		static 	std::map<std::string, Achivement> GetAchivements() { return m_Achivements; }
	private:
		//are used to determine if an Achivement is active or not
		static std::map<std::string, Property> m_Properties;
		//are used to determine if some extra boost are can be applied
		static std::map<std::string, Achivement> m_Achivements;
	};
}
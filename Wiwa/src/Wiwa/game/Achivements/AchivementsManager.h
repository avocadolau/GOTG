#pragma once
#include <Wiwa/core/Resources.h>
#include <Wiwa/core/Core.h>
#include <string>
#include <map>
#include "Achivement.h"
namespace Wiwa {
	class JSONDocument;
	class WI_API AchivementsManager {
	public:
		AchivementsManager();
		//adds a property
		void DefineProperty(std::string name, int initialValue, ActivationRules activationMode, int value);
		//adds a Miscellaneous achivement 
		void DefineAchivementAchivement(std::string name, int max_capacity, std::vector<int> costs, std::vector<Property> properties);
		//retruns the value of the property
		int GetValue(std::string name);
		//sets the current value to the property
		void SetValue(std::string name, int value);
		//adds a value to the current m_value 
		void AddValue(std::vector<std::string> properties, int value);
		//save the achivements
		void Serialize(JSONDocument* doc);
		//load achivements
		void Deserialize(JSONDocument* doc);

		std::vector<Achivement*> CheckAchivements();
	private:
		//are used to determine if an Achivement is active or not
		std::map<std::string, Property> m_Properties;
		//are used to determine if some extra boost are can be applied
		std::map<std::string, Achivement> m_Achivements;
	};
}
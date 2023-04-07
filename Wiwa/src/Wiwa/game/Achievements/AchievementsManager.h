#pragma once

#include <Wiwa/core/Core.h>

#include <string>

#include <map>

#include "Achievement.h"

namespace Wiwa {
	class JSONDocument;
	class WI_API AchievementsManager {
	public:
		AchievementsManager();
		~AchievementsManager();
	public:
		//adds a property
		static void DefineProperty(std::string name, std::string description, int initialValue, ActivationRules activationMode, int value);
		//adds a Miscellaneous Achievement 
		static void DefineAchievement(std::string name, std::string description, int max_capacity, std::vector<int> costs, std::vector<Property> properties);
		//add property
		static void AddProperty(const Property& prop);
		//add Achievement
		static void AddAchievement(const Achievement& Achievement);
		//retruns the value of the property
		static int GetValue(std::string name);
		//sets the current value to the property
		static void SetValue(std::string name, int value);
		//adds a value to the current m_value 
		static void AddValue(std::vector<std::string> properties, int value);
		//pass by all the Achievements and checks if they are completed
		static std::vector<Achievement*> CheckAchievements();
		//save the Achievements
		static void Serialize(JSONDocument* doc);
		//load Achievements
		static void Deserialize(JSONDocument* doc);
		//gets all the properties list (rules that need to be completed before unlocking an Achievement)
		WI_HARD_INL static 	std::map<std::string, Property> GetProperties() { return m_Properties; }
		//gets the Achievements list
		WI_HARD_INL static 	std::map<std::string, Achievement> GetAchievements() { return m_Achievements; }
		//return property
		static Property* GetProperty(const char* name);
		//returns Achievement
		static Achievement* GetAchievement(const char* name);
	private:
		//are used to determine if an Achievement is active or not
		static std::map<std::string, Property> m_Properties;
		//are used to determine if some extra boost are can be applied
		static std::map<std::string, Achievement> m_Achievements;
	};
}
#include "wipch.h"
#include "AchievementsManager.h"

#include <Wiwa/utilities/json/JSONDocument.h>
#include <rapidjson.h>

namespace Wiwa
{	
	std::map<std::string, Property> Wiwa::AchievementsManager::m_Properties;
	std::map<std::string, Achievement> Wiwa::AchievementsManager::m_Achievements;

	AchievementsManager::AchievementsManager()
	{
	}

	AchievementsManager::~AchievementsManager()
	{
		m_Properties.clear();
		m_Achievements.clear();
	}

	void AchievementsManager::DefineProperty(std::string name, std::string description, int initialValue, ActivationRules activationMode, int value)
	{
		m_Properties[name] = Property(name, description, initialValue, activationMode, value);
	}

	void AchievementsManager::DefineAchievement(std::string name, std::string description, int max_capacity, std::vector<int> costs, std::vector<Property> properties)
	{
		m_Achievements[name] = Achievement(name, description, max_capacity, costs, properties);
	}

	void AchievementsManager::AddProperty(const Property& prop)
	{
		m_Properties[prop.m_Name] =  prop;
	}

	void AchievementsManager::RemoveProperty(const char* prop)
	{
		m_Properties.erase(prop);
	}

	void AchievementsManager::AddAchievement(const Achievement& Achievement)
	{
		m_Achievements[Achievement.m_Name] = Achievement;
	}

	void AchievementsManager::RemoveAchievement(const char* achivement)
	{
		m_Achievements.erase(achivement);
	}

	int AchievementsManager::GetValue(std::string name)
	{
		auto item = m_Properties.find(name);

		if (item != m_Properties.end())
			return item->second.m_Value;
		else
			return -1;
	}

	void AchievementsManager::SetValue(std::string name, int value)
	{
		auto item = m_Properties.find(name);

		if (item != m_Properties.end())
			item->second.m_Value = value;
		else
			return;
	}

	void AchievementsManager::AddValue(std::vector<std::string> properties, int value)
	{
		for (int i = 0; i < properties.size(); i++)
		{
			std::string item = properties[i];
			SetValue(item, GetValue(item) + value);
		}
	}

	std::vector<Achievement*> AchievementsManager::CheckAchievements()
	{
		std::vector<Achievement*> result;

		for (auto Achievement : m_Achievements)
		{
			if (!Achievement.second.m_Unlocked)
			{
				int activeProperties = 0;

				for (auto prop : Achievement.second.m_Properties)
				{
					if (prop.IsActive())
					{
						activeProperties++;
					}
				}
				if (Achievement.second.m_Properties.size() == activeProperties)
				{
					result.push_back(&Achievement.second);
				}
			}
		}
		return result;
	}

	void AchievementsManager::Serialize(JSONDocument* doc)
	{
		JSONValue properties = doc->AddMemberArray("property");
		for (std::map<std::string,Property>::iterator prop = m_Properties.begin(); prop != m_Properties.end(); prop++)
		{
			JSONValue propObj = properties.PushBackObject();
			propObj.AddMember("name", prop->second.m_Name.c_str());
			propObj.AddMember("description", prop->second.m_Description.c_str());
			propObj.AddMember("activation",(int) prop->second.m_Activation);
			propObj.AddMember("activation_value", prop->second.m_ActivationValue);
			propObj.AddMember("initial_value",prop->second.m_InitialValue);
			propObj.AddMember("value",prop->second.m_Value);
		}

		JSONValue Achievements = doc->AddMemberArray("Achievement");

		for (std::map<std::string, Achievement>::iterator Achievement = m_Achievements.begin(); Achievement != m_Achievements.end(); Achievement++)
		{
			JSONValue AchievementsObj = Achievements.PushBackObject();
			AchievementsObj.AddMember("name", Achievement->second.m_Name.c_str());
			AchievementsObj.AddMember("description", Achievement->second.m_Description.c_str());
			AchievementsObj.AddMember("tier", Achievement->second.m_Tier);
			AchievementsObj.AddMember("num_tier", Achievement->second.m_NumTier);
			AchievementsObj.AddMember("current_cost", Achievement->second.m_CurrentCost);
			AchievementsObj.AddMember("unlocked", Achievement->second.m_Unlocked);

			JSONValue costs = AchievementsObj.AddMemberArray("costs");
		
			for (int i  = 0; i < Achievement->second.m_Costs.size(); i++)
			{ 
				costs.PushBack(Achievement->second.m_Costs[i]);
			}

			JSONValue properties = AchievementsObj.AddMemberArray("properties");

			for (const auto& prop : Achievement->second.m_Properties)
			{
				properties.PushBack(prop.m_Name.c_str());
			}
		}
	}

	void AchievementsManager::Deserialize(JSONDocument* doc)
	{
		JSONDocument& document = *doc;
		if (doc->HasMember("property"))
		{
			JSONValue props = document["property"];
			if (props.IsArray())
			{
				for (size_t i = 0; i < props.Size(); i++)
				{
					Wiwa::Property porperty;
					porperty.m_Name= props[i]["name"].as_string();
					porperty.m_Description = props[i]["description"].as_string();
					porperty.m_Activation = static_cast<ActivationRules>(props[i]["activation"].as_int());
					porperty.m_ActivationValue= props[i]["activation_value"].as_int();
					porperty.m_InitialValue = props[i]["initial_value"].as_int();
					porperty.m_Value= props[i]["value"].as_int();
					AddProperty(porperty);
				}
			}
		}
		if (doc->HasMember("Achievement"))
		{
			JSONValue Achievements = document["Achievement"];
			if (Achievements.IsArray())
			{
				for (size_t i = 0; i < Achievements.Size(); i++)
				{
					Wiwa::Achievement Achievement;
					Achievement.m_Name = Achievements[i]["name"].as_string();
					Achievement.m_Description = Achievements[i]["description"].as_string();
					Achievement.m_Tier = Achievements[i]["tier"].as_int();
					Achievement.m_NumTier = Achievements[i]["num_tier"].as_int();
					Achievement.m_CurrentCost = Achievements[i]["current_cost"].as_int();
					Achievement.m_Unlocked = Achievements[i]["unlocked"].as_bool();

					JSONValue costs = Achievements[i]["costs"];
					if (costs.IsArray())
					{
						for (size_t i = 0; i < costs.Size(); i++)
						{
							Achievement.m_Costs.push_back(costs[i].as_int());
						}
					}

					JSONValue props = Achievements[i]["properties"];
					if (props.IsArray())
					{
						for (size_t i = 0; i < props.Size(); i++)
						{
							Achievement.m_Properties.push_back(*GetProperty(props[i].as_string()));
						}
					}

					AddAchievement(Achievement);
				}
			}
		}
	}

	Property* AchievementsManager::GetProperty(const char* name)
	{
		auto item = m_Properties.find(name);
		if (item != m_Properties.end())
			return &item->second;
		else
			return nullptr;
	}

	Achievement* AchievementsManager::GetAchievement(const char* name)
	{
		auto item = m_Achievements.find(name);
		if (item != m_Achievements.end())
			return &item->second;
		else
			return nullptr;
	}


}

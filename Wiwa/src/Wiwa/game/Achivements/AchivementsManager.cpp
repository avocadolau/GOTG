#include "wipch.h"
#include "AchivementsManager.h"

#include <Wiwa/utilities/json/JSONDocument.h>

namespace Wiwa
{	
	std::map<std::string, Property> Wiwa::AchivementsManager::m_Properties;
	std::map<std::string, Achivement> Wiwa::AchivementsManager::m_Achivements;

	void AchivementsManager::DefineProperty(std::string name, std::string description, int initialValue, ActivationRules activationMode, int value)
	{
		m_Properties[name] = Property(name, description, initialValue, activationMode, value);
	}

	void AchivementsManager::DefineAchivement(std::string name, std::string description, int max_capacity, std::vector<int> costs, std::vector<Property> properties)
	{
		m_Achivements[name] = Achivement(name, description, max_capacity, costs, properties);
	}

	void AchivementsManager::AddProperty(const Property& prop)
	{
		m_Properties[prop.m_Name] =  prop;
	}

	void AchivementsManager::AddAchivement(const Achivement& achivement)
	{
		m_Achivements[achivement.m_Name] = achivement;
	}

	int AchivementsManager::GetValue(std::string name)
	{
		auto item = m_Properties.find(name);

		if (item != m_Properties.end())
			return item->second.m_Value;
		else
			return -1;
	}

	void AchivementsManager::SetValue(std::string name, int value)
	{
		auto item = m_Properties.find(name);

		if (item != m_Properties.end())
			item->second.m_Value = value;
		else
			return;
	}

	void AchivementsManager::AddValue(std::vector<std::string> properties, int value)
	{
		for (int i = 0; i < properties.size(); i++)
		{
			std::string item = properties[i];
			SetValue(item, GetValue(item) + value);
		}
	}

	std::vector<Achivement*> AchivementsManager::CheckAchivements()
	{
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

	void AchivementsManager::Serialize(JSONDocument* doc)
	{
	/*	JSONValue properties = doc->AddMemberArray("property");
		for (const auto& prop : m_Properties)
		{
			JSONValue propObj = properties.PushBackObject();
			propObj.AddMember("name", prop.second.m_Name.c_str());
			propObj.AddMember("description", prop.second.m_Description.c_str());
			propObj.AddMember("activation", prop.second.m_Activation);
			propObj.AddMember("activation_value", prop.second.m_ActivationValue);
			propObj.AddMember("initial_value",prop.second.m_InitialValue);
			propObj.AddMember("value",prop.second.m_Value);
		}

		JSONValue achivements = doc->AddMemberArray("achivement");
		for (const auto& achivement : m_Achivements)
		{
			JSONValue achivementsObj = achivements.PushBackObject();
			achivementsObj.AddMember("name", achivement.second.m_Name.c_str());
			achivementsObj.AddMember("description", achivement.second.m_Description.c_str());
			achivementsObj.AddMember("count", achivement.second.m_Count);
			achivementsObj.AddMember("max_cap", achivement.second.m_MaxCap);
			achivementsObj.AddMember("current_cost", achivement.second.m_CurrentCost);
			achivementsObj.AddMember("unlocked", achivement.second.m_Unlocked);
			
			JSONValue costs = achivementsObj.AddMemberArray("costs");

			for (const auto& cost : achivement.second.m_Costs)
			{
				costs.PushBack(cost);
			}

			JSONValue properties = achivementsObj.AddMemberArray("properties");

			for (const auto& prop : achivement.second.m_Porperties)
			{
				properties.PushBack(prop.m_Name);
			}
		}*/
	}

	void AchivementsManager::Deserialize(JSONDocument* doc)
	{
		//JSONDocument& document = *doc;
		//if (doc->HasMember("property"))
		//{
		//	JSONValue props = document["property"];
		//	if (props.IsArray())
		//	{
		//		for (size_t i = 0; i < props.Size(); i++)
		//		{
		//			Wiwa::Property porperty;
		//			porperty.m_Name= props[i]["name"].as_string();
		//			porperty.m_Description = props[i]["description"].as_string();
		//			porperty.m_Activation = props[i]["activation"];
		//			porperty.m_ActivationValue= props[i]["activation_value"].as_int();
		//			porperty.m_InitialValue = props[i]["initial_value"].as_int();
		//			porperty.m_Value= props[i]["value"].as_int();
		//			AddProperty(porperty);
		//		}
		//	}
		//}
		//if (doc->HasMember("achivements"))
		//{
		//	JSONValue achivements = document["property"];
		//	if (achivements.IsArray())
		//	{
		//		for (size_t i = 0; i < achivements.Size(); i++)
		//		{
		//			Wiwa::Achivement achivement;
		//			achivement.m_Name = achivements[i]["name"].as_string();
		//			achivement.m_Description = achivements[i]["description"].as_string();
		//			achivement.m_Count = achivements[i]["count"].as_int();
		//			achivement.m_MaxCap = achivements[i]["max_cap"].as_int();
		//			achivement.m_CurrentCost = achivements[i]["current_cost"].as_int();
		//			achivement.m_Unlocked = achivements[i]["unlocked"].as_bool();

		//			//JSONValue costs = achivements["costs"];
		//			//if (costs.IsArray())
		//			//{
		//			//	for (size_t i = 0; i < costs.Size(); i++)
		//			//	{
		//			//		achivement.m_Costs.push_back(costs[i].as_int());
		//			//	}
		//			//}

		//			//JSONValue props = achivements["properties"];
		//			//if (props.IsArray())
		//			//{
		//			//	for (size_t i = 0; i < props.Size(); i++)
		//			//	{
		//			//		achivement.m_Porperties.push_back(*GetProperty(props[i].as_string()));
		//			//	}
		//			//}

		//			AddAchivement(achivement);
		//		}
		//	}
		//}
	}

	Property* AchivementsManager::GetProperty(const char* name)
	{
		auto item = m_Properties.find(name);
		if (item != m_Properties.end())
			return &item->second;
		else
			return nullptr;
	}

	Achivement* AchivementsManager::GetAchivement(const char* name)
	{
		auto item = m_Achivements.find(name);
		if (item != m_Achivements.end())
			return &item->second;
		else
			return nullptr;
	}


}

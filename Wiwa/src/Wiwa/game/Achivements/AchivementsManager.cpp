#include "wipch.h"
#include "AchivementsManager.h"


namespace Wiwa
{	

	std::map<std::string, Property> Wiwa::AchivementsManager::m_Properties;
	std::map<std::string, Achivement> Wiwa::AchivementsManager::m_Achivements;

	void AchivementsManager::DefineProperty(std::string name, int initialValue, ActivationRules activationMode, int value)
	{
		m_Properties[name] = Property(name, initialValue, activationMode, value);
	}

	void AchivementsManager::DefineAchivement(std::string name, int max_capacity, std::vector<int> costs, std::vector<Property> properties)
	{
		m_Achivements[name] = Achivement(name, max_capacity, costs, properties);
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
	}

	void AchivementsManager::Deserialize(JSONDocument* doc)
	{
	}


}

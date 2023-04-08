#include "AchievementsPanel.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/game/Achievements/Property.h>
#include <Wiwa/game/Achievements/Achievement.h>
#include <Wiwa/game/Achievements/AchievementsManager.h>
#include <cstdlib> 
#define MAX_DESCRIPTION_CHARACTERS 256

AchievementsPanel::AchievementsPanel(EditorLayer* instance)
	: Panel("Achievements", ICON_FK_LIST_UL, instance)
{

}

AchievementsPanel::~AchievementsPanel()
{
}

void AchievementsPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	if (ImGui::CollapsingHeader("Properties"))
	{
        //std::map<std::string, Wiwa::Property> properties = Wiwa::AchievementsManager::GetProperties();
        //std::vector<std::string> keysToRemove; // Store keys to remove from the original map

        //// Create a copy of the map
        //std::map<std::string, Wiwa::Property> copyProperties = properties;

        //// Display existing properties
        //for (std::map<std::string, Wiwa::Property>::iterator item = copyProperties.begin(); item != copyProperties.end(); item++) {
        //    ImGui::Text(item->first.c_str());
        //    ImGui::Separator();
        //    ImGui::Indent();

        //    // Edit name
        //    if (ImGui::InputText("Name", &item->second.m_Name, 56)) {
        //        // Update map with new name
        //        auto tmp = item->second;
        //        keysToRemove.push_back(item->first);
        //        properties.insert(std::make_pair(tmp.m_Name, tmp));
        //    }

        //    // Edit description
        //    if (ImGui::InputText("Description", &item->second.m_Description)) {
        //        properties[item->first].m_Description = item->second.m_Description;
        //    }

        //    // Edit activation rule
        //    int activationIndex = static_cast<int>(item->second.m_Activation);
        //    if (ImGui::Combo("Activation Rule", &activationIndex, "None\0GreaterThan\0LessThan\0EqualTo\0")) {
        //        auto tmp = item->second;
        //        tmp.m_Activation = static_cast<Wiwa::ActivationRules>(activationIndex);
        //        properties[item->first].m_Activation = tmp.m_Activation;
        //    }

        //    // Edit activation value
        //    if (item->second.m_Activation != Wiwa::ActivationRules::NONE) {
        //        if (ImGui::InputInt("Activation Value", &item->second.m_ActivationValue)) {
        //            properties[item->first].m_ActivationValue = std::max(item->second.m_ActivationValue, 0);
        //        }
        //    }

        //    // Edit initial value
        //    if (ImGui::InputInt("Initial Value", &item->second.m_InitialValue)) {
        //        properties[item->first].m_InitialValue = item->second.m_InitialValue;
        //    }

        //    // Edit value
        //    if (ImGui::InputInt("Value", &item->second.m_Value)) {
        //        properties[item->first].m_Value = std::max(item->second.m_Value, 0);
        //    }

        //    ImGui::Separator();
        //    ImGui::Unindent();
        //}

        //// Remove keys from the original map
        //for (auto key : keysToRemove) {
        //    properties.erase(key);
        //}

        // Button to add new property
        if (ImGui::Button("Add Property")) {   
            Wiwa::AchievementsManager::DefineProperty("new property","description",0,Wiwa::ActivationRules::NONE,0);
        }

       
		ImGui::Separator();
		if (ImGui::Button("Save"))
		{
			Wiwa::GameStateManager::SerializeData();
		}
	}
	if (ImGui::CollapsingHeader("Achievements"))
	{
		const std::map<std::string, Wiwa::Achievement>& achievements = Wiwa::AchievementsManager::GetAchievements();

		if (ImGui::Button("add"))
		{

		}


		ImGui::Separator();
		if (ImGui::Button("Save"))
		{
			Wiwa::GameStateManager::SerializeData();
		}
	}
	ImGui::End();
}

void AchievementsPanel::OnEvent(Wiwa::Event& e)
{
}

// Assuming ImGui is properly initialized
void AchievementsPanel::AchievementsPanel::ShowPropertiesTable(const std::map<std::string, Wiwa::Property>& properties)
{
    // Table header
    ImGui::Text("Name");
    ImGui::SameLine(150);
    ImGui::Text("Description");
    ImGui::SameLine(400);
    ImGui::Text("Value");

    // Separator line
    ImGui::Separator();

    // Table rows
    for (const auto& [name, property] : properties)
    {
        ImGui::Text(name.c_str());
        ImGui::SameLine(150);
        ImGui::Text(property.m_Description.c_str());
        ImGui::SameLine(400);

        // Edit property value
        int value = property.m_Value;
        if (ImGui::InputInt("##value", &value))
        {
            // Update property value
            const_cast<Wiwa::Property&>(property).m_Value = value;
        }
    }
}

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
		std::map<std::string, Wiwa::Property> properties = Wiwa::AchievementsManager::GetProperties();
        ImGui::Begin("Property Editor");

        // Display existing properties
        for (auto item = properties.begin(); item != item; item++) {
            ImGui::Text("%s:", item->first.c_str());

            // Edit name
            std::string newName = name;
            if (ImGui::InputText("##Name", &newName)) {
                // Update map with new name
                auto it = properties.find(name);
                if (it != properties.end())
                {
                    auto s = it->second;
                    Wiwa::AchievementsManager::RemoveProperty(it->first.c_str());
                    s.m_Name = newName;
                    Wiwa::AchievementsManager::AddProperty(s);
                }

            }

            // Edit description
            std::string newDesc = item->second.m_Description;
            if (ImGui::InputText("Description", &newDesc)) {
                item->second.m_Description = newDesc;
            }

            // Edit activation rule
            int activationIndex = static_cast<int>(item->second.m_Activation);
            if (ImGui::Combo("Activation Rule", &activationIndex, "None\0GreaterThan\0LessThan\0EqualTo\0")) {
                item->second.m_Activation = static_cast<Wiwa::ActivationRules>(activationIndex);
            }

            // Edit activation value
            if (item->second.m_Activation != Wiwa::ActivationRules::NONE) {
                if (ImGui::InputInt("Activation Value", &item->second.m_ActivationValue)) {
                    item->second.m_ActivationValue = (item->second.m_ActivationValue, 0); // Make sure value is non-negative
                }
            }

            // Edit initial value
            if (ImGui::InputInt("Initial Value", &item->second.m_InitialValue)) {
                item->second.m_Value = item->second.m_InitialValue; // Reset value to initial value if changed
            }

            // Edit value
            if (ImGui::InputInt("Value", &item->second.m_Value)) {
                item->second.m_Value = abs(item->second.m_Value); // Make sure value is non-negative
            }

            ImGui::Separator();
        }

        // Button to add new property
        if (ImGui::Button("Add Property")) {
            //Wiwa::Property newProperty;
            //newProperty.m_Name = "New Property";
            //newProperty.m_Description = "Description";           
            //Wiwa::AchievementsManager::AddProperty(newProperty);
        }

        ImGui::End();
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

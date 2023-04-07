#include "AchievementsPanel.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/game/Achievements/Property.h>
#include <Wiwa/game/Achievements/Achievement.h>
#include <Wiwa/game/Achievements/AchievementsManager.h>
#include <map>
#include <string>

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
		const std::map<std::string, Wiwa::Property>& properties = Wiwa::AchievementsManager::GetProperties();
		if (ImGui::Button("add"))
		{
			ImGui::OpenPopup("new Property");
		}
		if (ImGui::BeginPopup("new Property"))
		{
			ImGui::Text("Alo");
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
			ImGui::OpenPopup("new Achievement");
		}
		if (ImGui::BeginPopup("new Achievement"))
		{
			ImGui::Text("Alo");
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

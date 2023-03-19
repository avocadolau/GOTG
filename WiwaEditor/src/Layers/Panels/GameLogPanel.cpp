#include "GameLogPanel.h"
#include <Wiwa/game/GameStateManager.h>

GameLogPanel::GameLogPanel(EditorLayer* instance)
	: Panel("Game Log", ICON_FK_GAMEPAD, instance)
{
}

GameLogPanel::~GameLogPanel()
{
}

void GameLogPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	ImGui::TextColored(ImVec4(102, 0, 255, 1), "Panel to check information about room");

	DrawStateInfo();
	DrawRoomSpawnersInfo();
	ImGui::End();
}

void GameLogPanel::DrawStateInfo()
{
	const char* roomType = Wiwa::GameStateManager().GetRoomType();
	ImGui::Text("Room type: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 255, 200, 1), roomType);

	const char* roomState = Wiwa::GameStateManager().GetRoomState();
	ImGui::Text("Room state: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 255, 200, 1), roomState);
}

void GameLogPanel::DrawRoomSpawnersInfo()
{
	ImGui::Text("Total spawners: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 0, 200, 1), "%d", Wiwa::GameStateManager::s_TotalSpawners);

	ImGui::Text("Finished spawners: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 0, 0, 1), "%d", Wiwa::GameStateManager::s_SpawnersFinished);
}


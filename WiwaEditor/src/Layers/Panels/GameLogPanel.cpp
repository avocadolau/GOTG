#include "GameLogPanel.h"
#include <Wiwa/game/GameStateManager.h>
#include "../../Utils/EditorUtils.h"

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
	DrawRoomVariables();
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

void GameLogPanel::DrawRoomVariables()
{
	ImGui::Separator();
	ImGui::Text("Rooms to boss %d", Wiwa::GameStateManager::s_RoomsToBoss);
	ImGui::Text("Rooms to shop %d", Wiwa::GameStateManager::s_RoomsToShop);
	ImGui::Text("Rooms to end and return tu hub %d", Wiwa::GameStateManager::s_CurrentRoomsCount);
	ImGui::Separator();
	ImGui::PushID(0);
	ImGui::Text("Battle rooms");
	VectorEdit(Wiwa::GameStateManager::s_CombatRooms);
	ImGui::PopID();
	ImGui::Separator();
	ImGui::PushID(1);
	ImGui::Text("Reward rooms");
	VectorEdit(Wiwa::GameStateManager::s_RewardRooms);
	ImGui::Separator();
	ImGui::PopID();
	ImGui::PushID(2);
	ImGui::Text("Shop rooms");
	VectorEdit(Wiwa::GameStateManager::s_ShopRooms);
	ImGui::PopID();
	ImGui::Separator();

	const char* startCharacter = Wiwa::GameStateManager::s_CurrentCharacter == 0 ? "StarLord" : Wiwa::GameStateManager::s_CurrentCharacter == 1 ? "Rocket" : "None";
	ImGui::Text("Current character %s", startCharacter);
	ImGui::InputInt("Start character", &Wiwa::GameStateManager::s_CurrentCharacter);
	CLAMP(Wiwa::GameStateManager::s_CurrentCharacter, 0, 1);
	if(ImGui::CollapsingHeader("Star Lord default values"))
	{
		ImGui::InputInt("Max health", &Wiwa::GameStateManager::s_CharacterSettings[0].MaxHealth);
		ImGui::Text("Current health %i", Wiwa::GameStateManager::s_CharacterSettings[0].Health);
		ImGui::InputInt("Max Shield", &Wiwa::GameStateManager::s_CharacterSettings[0].MaxShield);
		ImGui::Text("Current Shield %i", Wiwa::GameStateManager::s_CharacterSettings[0].Shield);
		ImGui::InputFloat("Speed", &Wiwa::GameStateManager::s_CharacterSettings[0].Speed);
		ImGui::InputFloat("Dash distance", &Wiwa::GameStateManager::s_CharacterSettings[0].DashDistance);
		ImGui::InputFloat("Dash speed", &Wiwa::GameStateManager::s_CharacterSettings[0].DashSpeed);
		ImGui::InputFloat("Dash cooldown", &Wiwa::GameStateManager::s_CharacterSettings[0].DashCoolDown);
		ImGui::InputFloat("Walk threshold", &Wiwa::GameStateManager::s_CharacterSettings[0].WalkTreshold);
	}

	if (ImGui::CollapsingHeader("Rocket default values"))
	{
		ImGui::InputInt("Max health", &Wiwa::GameStateManager::s_CharacterSettings[1].MaxHealth);
		ImGui::Text("Current health %i", Wiwa::GameStateManager::s_CharacterSettings[1].Health);
		ImGui::InputInt("Max Shield", &Wiwa::GameStateManager::s_CharacterSettings[1].MaxShield);
		ImGui::Text("Current Shield %i", Wiwa::GameStateManager::s_CharacterSettings[1].Shield);
		ImGui::InputFloat("Speed", &Wiwa::GameStateManager::s_CharacterSettings[1].Speed);
		ImGui::InputFloat("Dash distance", &Wiwa::GameStateManager::s_CharacterSettings[1].DashDistance);
		ImGui::InputFloat("Dash speed", &Wiwa::GameStateManager::s_CharacterSettings[1].DashSpeed);
		ImGui::InputFloat("Dash cooldown", &Wiwa::GameStateManager::s_CharacterSettings[1].DashCoolDown);
		ImGui::InputFloat("Walk threshold", &Wiwa::GameStateManager::s_CharacterSettings[1].WalkTreshold);
	}

	ImGui::InputFloat("Gamepad deadzone", &Wiwa::GameStateManager::s_GamepadDeadzone);

	if (ImGui::Button("Save"))
	{
		Wiwa::GameStateManager::SerializeData();
	}
}




#include "EnemyPanel.h"
#include <Wiwa/game/GameStateManager.h>

EnemyPanel::EnemyPanel(EditorLayer* instance)
	: Panel("Enemy Panel", ICON_FK_MAGIC, instance)
{
	Init();
}

EnemyPanel::~EnemyPanel()
{
}

void EnemyPanel::Init()
{
	Wiwa::GameStateManager::GetEnemyManager().DeSerialize();
}

void EnemyPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	DrawTable();

	ImGui::Text("Hard CleanUp: Resets everythin to base level");
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
		Wiwa::GameStateManager::GetEnemyManager().Reset();

	ImGui::SameLine();
	if (ImGui::Button("Load"))
		Wiwa::GameStateManager::GetEnemyManager().DeSerialize();

	ImGui::End();
}

void EnemyPanel::DrawTable()
{
	Wiwa::EnemyManager& manager = Wiwa::GameStateManager::GetEnemyManager();
	std::map<std::pair<int, std::string>, Wiwa::EnemyData>& data = manager.GetData();

	ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV 
		| ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH;

	if (ImGui::TreeNode("Enemies stats"))
	{
		if (ImGui::TreeNodeEx("Melee Phalanx Generic:"))
		{
			if (ImGui::BeginTable("Enemies stats", 9, flags))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Level");
				ImGui::TableSetupColumn("Health");
				ImGui::TableSetupColumn("Damage");
				ImGui::TableSetupColumn("MaxVelocity");
				ImGui::TableSetupColumn("MaxAcceleration");
				ImGui::TableSetupColumn("RateOfFire");
				ImGui::TableSetupColumn("Range");
				ImGui::TableSetupColumn("CreditsDrop");

				ImGui::TableHeadersRow();

				for (int i = 0; i < manager.m_MaxLevel; i++)
				{
					manager.CheckIfHasLevel(i, "MELEE_PHALANX_REDVARIANT");
					Wiwa::EnemyData& value = data.at({ i, "MELEE_PHALANX_REDVARIANT" });

					ImGui::TableNextRow();
					// Name
					ImGui::TableSetColumnIndex(0);
					ImGui::TextUnformatted(value.name.c_str());

					// Level
					ImGui::TableSetColumnIndex(1);
					ImGui::TextUnformatted(std::to_string(value.level).c_str());

					// Health
					ImGui::TableSetColumnIndex(2);
					ImGui::InputInt("health", &value.health);

					// Damage
					ImGui::TableSetColumnIndex(3);
					ImGui::InputInt("damage", &value.damage);

					// Max Velocity
					ImGui::TableSetColumnIndex(4);
					ImGui::TextUnformatted(std::to_string(value.maxVelocity).c_str());

					// Max Acceleration
					ImGui::TableSetColumnIndex(5);
					ImGui::TextUnformatted(std::to_string(value.maxAcceleration).c_str());

					// Rate of fire
					ImGui::TableSetColumnIndex(6);
					ImGui::TextUnformatted(std::to_string(value.rateOfFire).c_str());

					// Range
					ImGui::TableSetColumnIndex(7);
					ImGui::TextUnformatted(std::to_string(value.range).c_str());

					// Credits drop
					ImGui::TableSetColumnIndex(8);
					ImGui::TextUnformatted(std::to_string(value.creditsDrop).c_str());
				}
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	//if (ImGui::BeginTable("Enemies stats", 9))
	//{
	//	ImGui::TableSetupColumn("Name");
	//	ImGui::TableSetupColumn("Level");
	//	ImGui::TableSetupColumn("Health");
	//	ImGui::TableSetupColumn("Damage");
	//	ImGui::TableSetupColumn("MaxVelocity");
	//	ImGui::TableSetupColumn("MaxAcceleration");
	//	ImGui::TableSetupColumn("RateOfFire");
	//	ImGui::TableSetupColumn("Range");
	//	ImGui::TableSetupColumn("CreditsDrop");
	//	
	//	for (const auto& [key, value] : data)
	//	{
	//		ImGui::TableNextRow();
	//		// Name
	//		ImGui::TableSetColumnIndex(0);
	//		ImGui::TextUnformatted(value.name.c_str());

	//		// Level
	//		ImGui::TableSetColumnIndex(1);
	//		ImGui::TextUnformatted(std::to_string(value.level).c_str());

	//		// Health
	//		ImGui::TableSetColumnIndex(2);
	//		ImGui::TextUnformatted(std::to_string(value.health).c_str());

	//		// Damage
	//		ImGui::TableSetColumnIndex(3);
	//		ImGui::TextUnformatted(std::to_string(value.damage).c_str());

	//		// Max Velocity
	//		ImGui::TableSetColumnIndex(4);
	//		ImGui::TextUnformatted(std::to_string(value.maxVelocity).c_str());

	//		// Max Acceleration
	//		ImGui::TableSetColumnIndex(5);
	//		ImGui::TextUnformatted(std::to_string(value.maxAcceleration).c_str());

	//		// Rate of fire
	//		ImGui::TableSetColumnIndex(6);
	//		ImGui::TextUnformatted(std::to_string(value.rateOfFire).c_str());

	//		// Range
	//		ImGui::TableSetColumnIndex(7);
	//		ImGui::TextUnformatted(std::to_string(value.range).c_str());

	//		// Credits drop
	//		ImGui::TableSetColumnIndex(8);
	//		ImGui::TextUnformatted(std::to_string(value.creditsDrop).c_str());
	//	}
	//	ImGui::EndTable();
	//}
	
}

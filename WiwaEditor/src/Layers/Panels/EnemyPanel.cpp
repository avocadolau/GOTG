#include "EnemyPanel.h"
#include <Wiwa/game/GameStateManager.h>

static ImGuiTableFlags s_FlagsEnemyTable = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV
| ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Resizable;

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

	if (ImGui::TreeNode("Enemies stats"))
	{
		DrawEnemyStatsTable("Melee Phalanx Generic", "MELEE_PHALANX_GENERIC");
		DrawEnemyStatsTable("Melee Phalanx Red Variant", "MELEE_PHALANX_REDVARIANT");
		DrawEnemyStatsTable("Melee Phalanx Blue Variant", "MELEE_PHALANX_BLUEVARIANT");

		DrawEnemyStatsTable("Ranged Phalanx Generic", "RANGED_PHALANX_GENERIC");
		DrawEnemyStatsTable("Ranged Phalanx Red Variant", "RANGED_PHALANX_REDVARIANT");
		DrawEnemyStatsTable("Ranged Phalanx Blue Variant", "RANGED_PHALANX_BLUEVARIANT");

		DrawEnemyStatsTable("Sentinel", "SENTINEL");

		DrawEnemyStatsTable("Subjugator", "SUBJUGATOR");
		DrawEnemyStatsTable("Subjugator Chief", "SUBJUGATOR_CHIEF");

		DrawEnemyStatsTable("Ultron", "ULTRON");
		ImGui::TreePop();
	}
}

void EnemyPanel::DrawEnemyStatsTable(const char* label, const char* enemy_str)
{
	Wiwa::EnemyManager& manager = Wiwa::GameStateManager::GetEnemyManager();
	std::map<std::pair<int, std::string>, Wiwa::EnemyData>& data = manager.GetData();

	if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginTable(enemy_str, 9, s_FlagsEnemyTable))
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
				manager.CheckIfHasLevel(i, enemy_str);
				Wiwa::EnemyData& value = data.at({ i, enemy_str });

				ImGui::TableNextRow();
				// Name
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(value.name.c_str());

				// Level
				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted(std::to_string(value.level).c_str());

				// Health
				ImGui::TableSetColumnIndex(2);
				std::string h = "health";
				h = +i;
				ImGui::InputInt(h.c_str(), &value.health);

				// Damage
				ImGui::TableSetColumnIndex(3);
				ImGui::InputInt("damage", &value.damage);

				// Max Velocity
				ImGui::TableSetColumnIndex(4);
				ImGui::InputFloat("maxVelocity", &value.maxVelocity);

				// Max Acceleration
				ImGui::TableSetColumnIndex(5);
				ImGui::InputFloat("maxAcceleration", &value.maxAcceleration);

				// Rate of fire
				ImGui::TableSetColumnIndex(6);
				ImGui::InputFloat("maxAcceleration", &value.rateOfFire);

				// Range
				ImGui::TableSetColumnIndex(7);
				ImGui::InputFloat("maxAcceleration", &value.range);

				// Credits drop
				ImGui::TableSetColumnIndex(8);
				ImGui::InputInt("creditsDrop", &value.creditsDrop);

			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
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
	


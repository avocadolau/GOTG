#include "InventoryPanel.h"
#include <Wiwa/game/Items/ItemManager.h>
#include <Wiwa/game/Items/Inventory.h>

InventoryPanel::InventoryPanel(EditorLayer* instance)
	: Panel("Inventory", ICON_FK_DATABASE, instance)
{
	Wiwa::ItemManager::AddAbility({ "Test" });
	Wiwa::ItemManager::AddAbility({ "Test2" });
	Wiwa::ItemManager::AddAbility({ "Test3" });
}

InventoryPanel::~InventoryPanel()
{
}

void InventoryPanel::Draw()
{
	ImGui::Begin(iconName.c_str());
	int id = 0;
	if (ImGui::CollapsingHeader("Items pool"))
	{
		ImGui::Text("Abilities");
		DrawAbilityPool(id);
		ImGui::Separator();
		ImGui::Text("Passive skill");
		DrawPassivePool(id);
		ImGui::Separator();
		ImGui::Text("Buffs");
		ImGui::Separator();
		ImGui::Text("Consumables");
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Player inventory"))
	{

	}
	ImGui::End();
}

void InventoryPanel::DrawPassivePool(int id)
{
	std::map<std::string, Wiwa::PassiveSkill>& passives = Wiwa::ItemManager::GetSkills();
	
	char* buff;
	for (auto& it : passives)
	{
		ImGui::PushID(id++);
		Wiwa::PassiveSkill* passive = Wiwa::ItemManager::GetPassive(it.first.c_str());
		ImGui::Text(passive->Name.c_str());
		buff = (char*)passive->Description.c_str();
		ImGui::InputTextMultiline("Description", buff, 256);
		passive->Description = buff;

		if (ImGui::Button("Delete"))
		{
			Wiwa::ItemManager::DeletePassive(it.first.c_str());
		}
		ImGui::PopID();
	}
	if (ImGui::Button("Add Passive"))
	{
		ImGui::OpenPopup("Create passive");
	}
	if (ImGui::BeginPopup("Create passive"))
	{
		static char nameBuffer[256];
		ImGui::InputText("Name", nameBuffer, 256);

		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddPassive({ nameBuffer });
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void InventoryPanel::DrawAbilityPool(int id)
{
	std::map<std::string, Wiwa::Ability>& abilities = Wiwa::ItemManager::GetAbilities();
	char* buff;
	for (auto& it : abilities)
	{
		ImGui::PushID(id++);
		Wiwa::Ability* ability = Wiwa::ItemManager::GetAbility(it.first.c_str());
		ImGui::Text(ability->Name.c_str());
		buff = (char*)ability->Description.c_str();
		ImGui::InputTextMultiline("Description", buff, sizeof(buff));
		ability->Description = buff;

		if (ImGui::Button("Delete"))
		{
			Wiwa::ItemManager::DeleteAbility(it.first.c_str());
		}
		ImGui::PopID();
	}
	if (ImGui::Button("Add Ability"))
	{
		ImGui::OpenPopup("Create ability");
	}
	if (ImGui::BeginPopup("Create ability"))
	{
		static char nameBuffer[256];
		ImGui::InputText("Name", nameBuffer, 256);

		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddAbility({ nameBuffer });
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

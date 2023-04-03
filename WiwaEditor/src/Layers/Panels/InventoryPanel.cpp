#include "InventoryPanel.h"
#include <Wiwa/game/Items/ItemManager.h>
#include <Wiwa/game/Items/Inventory.h>
#include <Wiwa/game/GameStateManager.h>

#define MAX_DESCRIPTION_CHARACTERS 256

InventoryPanel::InventoryPanel(EditorLayer* instance)
	: Panel("Inventory", ICON_FK_DATABASE, instance)
{
}

InventoryPanel::~InventoryPanel()
{
}

void InventoryPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
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
		DrawBuffPool(id);
		ImGui::Separator();
		ImGui::Text("Consumables");
		DrawConsumablePool(id);

		if (ImGui::Button("Save"))
		{
			Wiwa::GameStateManager::SerializeData();
		}
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Player inventory"))
	{

	}
	ImGui::End();
}

void InventoryPanel::DrawConsumablePool(int& id)
{
	std::map<std::string, Wiwa::Consumable>& consumables = Wiwa::ItemManager::GetConsumables();
	char* buffer;
	for (auto& it : consumables)
	{
		ImGui::PushID(id++);
		Wiwa::Consumable* consumable = Wiwa::ItemManager::GetConsumable(it.first.c_str());
		ImGui::Text(consumable->Name.c_str());
		buffer = (char*)consumable->Description.c_str();
		ImGui::InputTextMultiline("Description", buffer, MAX_DESCRIPTION_CHARACTERS);
		consumable->Description = buffer;

		if (ImGui::Button("Delete"))
		{
			Wiwa::ItemManager::DeleteConsumable(it.first.c_str());
		}
		ImGui::PopID();
	}
	if (ImGui::Button("Add Consumable"))
	{
		ImGui::OpenPopup("Create consumable");
	}
	if (ImGui::BeginPopup("Create consumable"))
	{
		static char nameBuffer[256];
		ImGui::InputText("Name", nameBuffer, 256);

		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddConsumable({ nameBuffer });
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void InventoryPanel::DrawBuffPool(int& id)
{
	std::map<std::string, Wiwa::Buff>& buffs = Wiwa::ItemManager::GetBuffs();
	char* buffer;
	for (auto& it : buffs)
	{
		ImGui::PushID(id++);
		Wiwa::Buff* buff = Wiwa::ItemManager::GetBuff(it.first.c_str());
		ImGui::Text(buff->Name.c_str());
		buffer = (char*)buff->Description.c_str();
		ImGui::InputTextMultiline("Description", buffer, MAX_DESCRIPTION_CHARACTERS);
		buff->Description = buffer;

		if (ImGui::Button("Delete"))
		{
			Wiwa::ItemManager::DeleteBuff(it.first.c_str());
		}
		ImGui::PopID();
	}
	if (ImGui::Button("Add Buff"))
	{
		ImGui::OpenPopup("Create buff");
	}
	if (ImGui::BeginPopup("Create buff"))
	{
		static char nameBuffer[256];
		ImGui::InputText("Name", nameBuffer, 256);

		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddBuff({ nameBuffer });
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void InventoryPanel::DrawPassivePool(int& id)
{
	std::map<std::string, Wiwa::PassiveSkill>& passives = Wiwa::ItemManager::GetSkills();
	
	char* buff;
	for (auto& it : passives)
	{
		ImGui::PushID(id++);
		Wiwa::PassiveSkill* passive = Wiwa::ItemManager::GetPassive(it.first.c_str());
		ImGui::Text(passive->Name.c_str());
		buff = (char*)passive->Description.c_str();
		ImGui::InputTextMultiline("Description", buff, MAX_DESCRIPTION_CHARACTERS);
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

void InventoryPanel::DrawAbilityPool(int& id)
{
	std::map<std::string, Wiwa::Ability>& abilities = Wiwa::ItemManager::GetAbilities();
	char* buff;
	for (auto& it : abilities)
	{
		ImGui::PushID(id++);
		Wiwa::Ability* ability = Wiwa::ItemManager::GetAbility(it.first.c_str());
		ImGui::Text(ability->Name.c_str());
		buff = (char*)ability->Description.c_str();
		ImGui::InputTextMultiline("Description", buff, MAX_DESCRIPTION_CHARACTERS);
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

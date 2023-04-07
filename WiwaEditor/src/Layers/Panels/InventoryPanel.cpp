#include "InventoryPanel.h"

#include <ranges>
#include <Wiwa/game/Items/ItemManager.h>
#include <Wiwa/game/Items/Inventory.h>
#include <Wiwa/game/GameStateManager.h>
#include "../../Utils/EditorUtils.h"



InventoryPanel::InventoryPanel(EditorLayer* instance)
	: Panel("Inventory", ICON_FK_DATABASE, instance)
{
}

InventoryPanel::~InventoryPanel() = default;

void InventoryPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	if (ImGui::CollapsingHeader("Items pool"))
	{
		int id = 0;
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
		Wiwa::Inventory& playerInventory = Wiwa::GameStateManager::GetPlayerInventory();

		ImGui::Text("Tokens %i", playerInventory.GetTokens());
		
		Wiwa::Ability**& abilities = playerInventory.GetAbilities();
		
		ImGui::Text("Abilities");
		ImGui::Separator();
		ImGui::Indent();
		
		ImGui::Text("Slot 1");
		ImGui::Indent();
		if(abilities[0])
			ImGui::Text("Name %s", abilities[0]->Name.c_str());
		else
			ImGui::Text("Empty slot");
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Slot 2");
		ImGui::Indent();
		if(abilities[1])
			ImGui::Text("Name %s", abilities[1]->Name.c_str());
		else
			ImGui::Text("Empty slot");
		ImGui::Unindent();
		ImGui::Unindent();
		
		ImGui::Separator();
		ImGui::Text("Passives");
		ImGui::Separator();
		ImGui::Indent();
		
		std::vector<Wiwa::PassiveSkill>& passives = playerInventory.GetPassives();
		for (const auto& passive : passives)
		{
			ImGui::Text("Name %s", passive.Name.c_str());
		}
		
		ImGui::Unindent();
		ImGui::Separator();

		Wiwa::Buff**& buffs = playerInventory.GetBuffs();
		
		ImGui::Text("Buffs");
		ImGui::Separator();
		ImGui::Indent();
		
		ImGui::Text("Slot 1");
		ImGui::Indent();
		if(buffs[0])
			ImGui::Text("Name %s", abilities[0]->Name.c_str());
		else
			ImGui::Text("Empty slot");
		ImGui::Unindent();
		ImGui::Separator();
		
		ImGui::Text("Slot 2");
		ImGui::Indent();
		if(buffs[1])
			ImGui::Text("Name %s", abilities[1]->Name.c_str());
		else
			ImGui::Text("Empty slot");
		
		ImGui::Unindent();
	}
	ImGui::End();
}

void InventoryPanel::DrawConsumablePool(int& id)
{
	if (ImGui::Button("Add Consumable"))
	{
		ImGui::OpenPopup("Create consumable");
	}
	if (ImGui::BeginPopup("Create consumable"))
	{
		static std::string name;
		ImGui::InputText("Name", &name);
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddConsumable({ name.c_str() });
			name.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	std::map<std::string, Wiwa::Consumable>& consumables = Wiwa::ItemManager::GetConsumables();
	if (!consumables.empty())
	{
		const char* types[] =
		{
			"Heal",
			"Shield"
		};

		if (ImGui::BeginTable("consumables", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Icon");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Buff %");
			ImGui::TableHeadersRow();

			for (auto& it : consumables)
			{
				Wiwa::Consumable* consumable = Wiwa::ItemManager::GetConsumable(it.first.c_str());

				ImGui::PushID(id++);
				
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(consumable->Name.c_str());
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete"))
				{
					Wiwa::ItemManager::DeleteConsumable(it.first);
				}
				ImGui::PopStyleColor();

				ImGui::TableNextColumn();
				ImGui::Image((ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(consumable->Icon)->GetTextureId(), {64, 64});

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS;
						
						if (ImageExtensionComp(p))
						{
							consumable->Icon = Wiwa::Resources::Load<Wiwa::Image>(p.string().c_str());
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::TableNextColumn();
				const char* currentItem = types[(int)consumable->Type];
				if (ImGui::BeginCombo("##type", currentItem))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = (currentItem == types[i]);
						if (ImGui::Selectable(types[i], isSelected))
						{
							currentItem = types[i];
							consumable->Type = (Wiwa::ConsumableType)(i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Checkbox("Is Ego's help", &consumable->IsEgosHelp);
				HelpMarker("If checked the item will behave like Ego's help");
				
				ImGui::TableNextColumn();
				ImGui::InputInt("##percent", &consumable->BuffPercent);

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	
}

void InventoryPanel::DrawBuffPool(int& id)
{
	std::map<std::string, Wiwa::Buff>& buffs = Wiwa::ItemManager::GetBuffs();
	if (ImGui::Button("Add Buff"))
	{
		ImGui::OpenPopup("Create buff");
	}
	if (ImGui::BeginPopup("Create buff"))
	{
		static std::string name;
		ImGui::InputText("Name", &name);
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddBuff({ name.c_str() });
			name.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (!buffs.empty())
	{
		const char* types[] =
		{
			"Major Victory Shield",
			"Nikki's Touch",
			"Cosmo's Paw",
			"Martinex Thermokinesis",
			"Bugs legs",
			"Charlie 27 fist"
		};

		if (ImGui::BeginTable("buffs", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Description");
			ImGui::TableSetupColumn("Icon");
			ImGui::TableSetupColumn("Buff %");
			ImGui::TableSetupColumn("Duration");
			ImGui::TableSetupColumn("Cooldown");
			ImGui::TableSetupColumn("Price");
			ImGui::TableSetupColumn("Type");
			ImGui::TableHeadersRow();

			for (auto& it : buffs)
			{
				Wiwa::Buff* buff = Wiwa::ItemManager::GetBuff(it.first.c_str());
				ImGui::PushID(id++);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(buff->Name.c_str());

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete"))
				{
					Wiwa::ItemManager::DeleteBuff(it.first.c_str());
				}
				ImGui::PopStyleColor();

				ImGui::TableNextColumn();
				ImGui::InputTextMultiline("Description", &buff->Description, {200, 100});


				ImGui::TableNextColumn();
				ImGui::Image((ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(buff->Icon)->GetTextureId(), { 64, 64 });

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS;

						if (ImageExtensionComp(p))
						{
							buff->Icon = Wiwa::Resources::Load<Wiwa::Image>(p.string().c_str());
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::TableNextColumn();
				ImGui::InputInt("##buffpercent", &buff->BuffPercent);

				ImGui::TableNextColumn();
				ImGui::InputFloat("##duration", &buff->Duration);

				ImGui::TableNextColumn();
				ImGui::InputFloat("##cooldown", &buff->Cooldown);

				ImGui::TableNextColumn();
				ImGui::InputInt("##price", &buff->Price);

				ImGui::TableNextColumn();
				const char* currentBuff = types[(int)buff->buffType];
				if (ImGui::BeginCombo("##type", currentBuff))
				{
					for (int i = 0; i < 6; i++)
					{
						const bool isSelected = (currentBuff == types[i]);
						if (ImGui::Selectable(types[i], isSelected))
						{
							currentBuff = types[i];
							buff->buffType = (Wiwa::BuffType)(i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				
				ImGui::PopID();
			}
			ImGui::EndTable();
		}

	}
}

void InventoryPanel::DrawPassivePool(int& id)
{
	std::map<std::string, Wiwa::PassiveSkill>& passives = Wiwa::ItemManager::GetSkills();
	if (ImGui::Button("Add Passive"))
	{
		ImGui::OpenPopup("Create passive");
	}
	if (ImGui::BeginPopup("Create passive"))
	{
		static std::string name;
		ImGui::InputText("Name", &name);
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddPassive({ name.c_str()});
			name.clear();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	if (!passives.empty())
	{
		const char* types[] =
		{
			"Movement",
			"Rate of Fire",
			"Buff",
			"Attack",
			"Projectile",
			"Health",
			"Range",
			"Shield charge"
		};
		if (ImGui::BeginTable("passives", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Description");
			ImGui::TableSetupColumn("Icon");
			ImGui::TableSetupColumn("Type");
			ImGui::TableHeadersRow();

			for (const auto& key : passives | std::views::keys)
			{
				Wiwa::PassiveSkill* passive = Wiwa::ItemManager::GetPassive(key.c_str());
				ImGui::PushID(id++);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(passive->Name.c_str());

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete"))
				{
					Wiwa::ItemManager::DeletePassive(key);
				}
				ImGui::PopStyleColor();


				ImGui::TableNextColumn();
				ImGui::InputTextMultiline("##description", &passive->Description, {200, 100});

				ImGui::TableNextColumn();
				ImGui::Image((ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(passive->Icon)->GetTextureId(), { 64, 64 });

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS;

						if (ImageExtensionComp(p))
						{
							passive->Icon = Wiwa::Resources::Load<Wiwa::Image>(p.string().c_str());
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::TableNextColumn();
				const char* currentItem = types[(int)passive->PassiveType];
				if (ImGui::BeginCombo("##type", currentItem))
				{
					for (int i = 0; i < 4; i++)
					{
						bool isSelected = (currentItem == types[i]);
						if (ImGui::Selectable(types[i], isSelected))
						{
							currentItem = types[i];
							passive->PassiveType = (Wiwa::PassiveType)(i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	
}

void InventoryPanel::DrawAbilityPool(int& id)
{
	std::map<std::string, Wiwa::Ability>& abilities = Wiwa::ItemManager::GetAbilities();
	if (ImGui::Button("Add Ability"))
	{
		ImGui::OpenPopup("Create ability");
	}
	if (ImGui::BeginPopup("Create ability"))
	{
		static std::string name;
		ImGui::InputText("Name", &name);
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			Wiwa::ItemManager::AddAbility({ name.c_str() });
			name.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (!abilities.empty())
	{
		const char* types[] =
		{
			"Yondu's Fin",
			"Groot's seeds",
			"Phylas Quantum Sword",
			"StarHawks blast",
		};
		if (ImGui::BeginTable("actives", 9, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Description");
			ImGui::TableSetupColumn("Icon");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Damage");
			ImGui::TableSetupColumn("Range");
			ImGui::TableSetupColumn("Area");
			ImGui::TableSetupColumn("Cooldown");
			ImGui::TableSetupColumn("Price");
			ImGui::TableHeadersRow();
			for (auto& it : abilities)
			{
				Wiwa::Ability* ability = Wiwa::ItemManager::GetAbility(it.first.c_str());
				ImGui::PushID(id++);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(ability->Name.c_str());

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete"))
				{
					Wiwa::ItemManager::DeleteAbility(it.first.c_str());
				}
				ImGui::PopStyleColor();

				ImGui::TableNextColumn();
				ImGui::InputTextMultiline("##description", &ability->Description, {200, 100});

				ImGui::TableNextColumn();

				ImGui::Image((ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(ability->Icon)->GetTextureId(), { 64, 64 });

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS;

						if (ImageExtensionComp(p))
						{
							ability->Icon = Wiwa::Resources::Load<Wiwa::Image>(p.string().c_str());
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::TableNextColumn();
				
				const char* currentItem = types[(int)ability->AbilityType];
				if (ImGui::BeginCombo("##type", currentItem))
				{
					for (int i = 0; i < 4; i++)
					{
						bool isSelected = (currentItem == types[i]);
						if (ImGui::Selectable(types[i], isSelected))
						{
							currentItem = types[i];
							ability->AbilityType = (Wiwa::AbilityType)(i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::TableNextColumn();
				ImGui::InputInt("##damage", &ability->Damage);

				ImGui::TableNextColumn();
				ImGui::InputFloat("##range", &ability->Range);

				ImGui::TableNextColumn();
				ImGui::InputFloat("##area", &ability->Area);

				ImGui::TableNextColumn();
				ImGui::InputFloat("##cooldown", &ability->Cooldown);

				ImGui::TableNextColumn();
				ImGui::InputInt("##price", &ability->Price);

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	
}

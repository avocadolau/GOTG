#include <wipch.h>
#include "UIPanel.h"
#include "Wiwa/scene/Scene.h"
#include <Wiwa/Ui/UiManager.h>

#include "imgui.h"

#include <Wiwa/core/Application.h>

using namespace Wiwa;

UIPanel::UIPanel(EditorLayer* instance)
	: Panel("UI", ICON_FK_TELEVISION, instance)
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	/*
	ImGui::Text("UI editor panel");

	if (ImGui::CollapsingHeader("Create UI element"))
	{
		if (ImGui::Button("Create Button"))
		{
			Rect2i* rect = new Rect2i;
			rect->x = 500;
			rect->y = 100;
			rect->width = 200;
			rect->height = 100;

			Wiwa::Scene::m_GuiManager->CreateGuiControl(GuiControlType::BUTTON, 0, &rect, "assets/test.png", nullptr, { 0,0,0,0 });
			if (rect != nullptr)
			{
				UI_elements_rects.push_back(rect);
			}

			delete rect;
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Slider"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Create Checkbox"))
		{

		}
	}

	if (ImGui::CollapsingHeader("Edit UI element"))
	{
		if (Wiwa::GuiManager::controls.size() > 0)
		{
			buttonID = 0;
			sliderID = 0;
			checkboxID = 0;

			for (int i = 0; i < Wiwa::GuiManager::controls.size(); i++)
			{
				ImGui::PushID(i);
				if (Wiwa::GuiManager::controls.at(i)->type == GuiControlType::BUTTON)
				{
					ImGui::Text("Button # %i", buttonID);
					buttonID++;
				}
				if (Wiwa::GuiManager::controls.at(i)->type == GuiControlType::SLIDER)
				{
					ImGui::Text("Slider # %i", sliderID);
					sliderID++;
				}
				if (Wiwa::GuiManager::controls.at(i)->type == GuiControlType::CHECKBOX)
				{
					ImGui::Text("Checkbox # %i", checkboxID);
					checkboxID++;
				}
				ImGui::SameLine();
				if (ImGui::Button("Edit"))
				{
					UI_element_selected = i;
					lastButtonID = buttonID;
					lastSliderID = sliderID;
					lastCheckboxID = checkboxID;
				}
				ImGui::PopID();
			}
		}
		else
		{
			ImGui::Text("	No UI elements created");
		}

		if (UI_element_selected >= 0)
		{
			ImGui::Spacing();
			if (Wiwa::GuiManager::controls.at(UI_element_selected)->type == GuiControlType::BUTTON)
			{
				ImGui::Text("Edit button # %i", lastButtonID);
			}
			if (Wiwa::GuiManager::controls.at(UI_element_selected)->type == GuiControlType::SLIDER)
			{
				ImGui::Text("Edit slider # %i", lastSliderID);
			}
			if (Wiwa::GuiManager::controls.at(UI_element_selected)->type == GuiControlType::CHECKBOX)
			{
				ImGui::Text("Edit checkbox # %i", lastCheckboxID);
			}

			Rect2i rect2;
			rect2.x = Wiwa::GuiManager::controls.at(UI_element_selected)->position.x;
			rect2.y = Wiwa::GuiManager::controls.at(UI_element_selected)->position.y;
			rect2.width = Wiwa::GuiManager::controls.at(UI_element_selected)->position.width;
			rect2.height = Wiwa::GuiManager::controls.at(UI_element_selected)->position.height;

			ImGui::PushID(UI_element_selected);
			ImGui::Text("X: ");
			ImGui::SameLine();
			if (ImGui::DragFloat3("", (float*)&rect2->x, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				Wiwa::GuiManager::controls.at(UI_element_selected)->position.x = rect2->x;
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			ImGui::Text("Y: ");
			ImGui::SameLine();
			if (ImGui::DragFloat3("", (float*)&rect2->y, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				Wiwa::GuiManager::controls.at(UI_element_selected)->position.y = rect2->y;
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			ImGui::Text("Width: ");
			ImGui::SameLine();
			if (ImGui::DragFloat3("", (float*)&rect2->width, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				Wiwa::GuiManager::controls.at(UI_element_selected)->position.width = rect2->width;
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			ImGui::Text("Height: ");
			ImGui::SameLine();
			if (ImGui::DragFloat3("", (float*)&rect2->height, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				Wiwa::GuiManager::controls.at(UI_element_selected)->position.height = rect2->height;
			}
			ImGui::PopID();
		}
	}
	*/

	ImGui::End();
}
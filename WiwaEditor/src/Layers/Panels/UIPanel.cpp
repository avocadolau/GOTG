#include <wipch.h>
#include "UIPanel.h"
#include "Wiwa/scene/Scene.h"
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/scene/SceneManager.h>
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
	Wiwa::GuiManager& m_GuiManager = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

	ImGui::Begin(iconName.c_str(), &active);

	
	ImGui::Text("UI editor panel");

	if (ImGui::CollapsingHeader("Create UI element"))
	{
		if (ImGui::Button("Create Button"))
		{
			Rect2i rect;
			rect.x = 500;
			rect.y = 100;
			rect.width = 200;
			rect.height = 100;
			m_GuiManager.CreateGuiControl(GuiControlType::BUTTON, 0, rect, "assets/test.png", nullptr);
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
		if (m_GuiManager.controls.size() > 0)
		{
			buttonID = 0;
			sliderID = 0;
			checkboxID = 0;

			for (int i = 0; i < m_GuiManager.controls.size(); i++)
			{
				ImGui::PushID(i);
				if (m_GuiManager.controls.at(i)->type == GuiControlType::BUTTON)
				{
					ImGui::Text("Button # %i", buttonID);
					buttonID++;
				}
				if (m_GuiManager.controls.at(i)->type == GuiControlType::SLIDER)
				{
					ImGui::Text("Slider # %i", sliderID);
					sliderID++;
				}
				if (m_GuiManager.controls.at(i)->type == GuiControlType::CHECKBOX)
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
			if (m_GuiManager.controls.at(UI_element_selected)->type == GuiControlType::BUTTON)
			{
				ImGui::Text("Edit button # %i", lastButtonID);
			}
			if (m_GuiManager.controls.at(UI_element_selected)->type == GuiControlType::SLIDER)
			{
				ImGui::Text("Edit slider # %i", lastSliderID);
			}
			if (m_GuiManager.controls.at(UI_element_selected)->type == GuiControlType::CHECKBOX)
			{
				ImGui::Text("Edit checkbox # %i", lastCheckboxID);
			}

			Rect2i rect2;
			rect2.x = m_GuiManager.controls.at(UI_element_selected)->position.x;
			rect2.y = m_GuiManager.controls.at(UI_element_selected)->position.y;
			rect2.width = m_GuiManager.controls.at(UI_element_selected)->position.width;
			rect2.height = m_GuiManager.controls.at(UI_element_selected)->position.height;

			ImGui::PushID(UI_element_selected);
			if (ImGui::DragFloat3("position x", (float*)&rect2.x, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->position.x = rect2.x;
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			if (ImGui::DragFloat3("position y", (float*)&rect2.y, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->position.y = rect2.y;
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			if (ImGui::DragFloat("Width", (float*)&rect2.width, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->position.width = rect2.width;
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			if (ImGui::DragFloat("Height", (float*)&rect2.height, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->position.height = rect2.height;
			}
			ImGui::PopID();
		}
	}
	

	ImGui::End();
}
#include <wipch.h>
#include "UIPanel.h"
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
			m_GuiManager.CreateGuiControl(GuiControlType::BUTTON, 1, rect, "assets/test.png", nullptr, { 0,0,0,0 });
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
				if (m_GuiManager.controls.at(i) != nullptr)
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


						/*startingPosition.x = m_GuiManager.controls.at(UI_element_selected)->GetPosition().x;
						startingPosition.y = m_GuiManager.controls.at(UI_element_selected)->GetPosition().y;
						startingPosition.width = m_GuiManager.controls.at(UI_element_selected)->GetPosition().width;
						startingPosition.height = m_GuiManager.controls.at(UI_element_selected)->GetPosition().height;*/
					}
					ImGui::PopID();
				}
			}
		}
		else
		{
			ImGui::Text("	No UI elements created");
		}

		if (UI_element_selected >= 0 && m_GuiManager.controls.at(UI_element_selected) != nullptr)
		{
			
			ImGui::Spacing();
			if (m_GuiManager.controls.at(UI_element_selected)->ReturnType() == GuiControlType::BUTTON)
			{
				ImGui::Text("Edit button # %i", lastButtonID);
			}
			if (m_GuiManager.controls.at(UI_element_selected)->ReturnType() == GuiControlType::SLIDER)
			{
				ImGui::Text("Edit slider # %i", lastSliderID);
			}
			if (m_GuiManager.controls.at(UI_element_selected)->ReturnType() == GuiControlType::CHECKBOX)
			{
				ImGui::Text("Edit checkbox # %i", lastCheckboxID);
			}

			ImGui::PushID(UI_element_selected);
			if (ImGui::SliderInt("position x", &m_GuiManager.controls.at(UI_element_selected)->position.x, 0.05f, 200.0f, "%.3f", NULL))
			{
				 m_GuiManager.controls.at(UI_element_selected)->SetPosition_x(m_GuiManager.controls.at(UI_element_selected)->position.x);
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			if (ImGui::SliderInt("position y",&m_GuiManager.controls.at(UI_element_selected)->position.y, 0.05f, 200.0f,"%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->SetPosition_y(m_GuiManager.controls.at(UI_element_selected)->position.y);
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			if (ImGui::SliderInt("Width", &m_GuiManager.controls.at(UI_element_selected)->position.width, 0.05f, 200.0f,"%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->SetPosition_width(m_GuiManager.controls.at(UI_element_selected)->position.width);
			}
			ImGui::PopID();

			ImGui::PushID(UI_element_selected);
			if (ImGui::SliderInt("Height", &m_GuiManager.controls.at(UI_element_selected)->position.height, 0.05f, 200.0f,"%.3f", NULL))
			{
				m_GuiManager.controls.at(UI_element_selected)->SetPosition_height(m_GuiManager.controls.at(UI_element_selected)->position.height);
			}
			ImGui::PopID();
		}
	}
	

	ImGui::End();
}
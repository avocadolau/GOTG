#include <wipch.h>
#include "UIPanel.h"
#include "imgui.h"
#include "Time.h"

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

	if (Time::IsPlaying() == false)
	{
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
			else
			{
				ImGui::Text("	No UI elements created");
			}

			if (UI_element_selected >= 0)
			{
				Rect2i rect2;
				rect2.x = m_GuiManager.controls.at(UI_element_selected)->position.x;
				rect2.y = m_GuiManager.controls.at(UI_element_selected)->position.y;
				rect2.width = m_GuiManager.controls.at(UI_element_selected)->position.width;
				rect2.height = m_GuiManager.controls.at(UI_element_selected)->position.height;

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
				if (ImGui::SliderInt("position x", &rect2.x, 0.0f, 1920.0f, "%.3f", NULL))
				{
					m_GuiManager.controls.at(UI_element_selected)->SetPosition_x(rect2.x);
				}
				ImGui::PopID();

				ImGui::PushID(UI_element_selected);
				if (ImGui::SliderInt("position y", &rect2.y, 0.0f, 1080.0f, "%.3f", NULL))
				{
					m_GuiManager.controls.at(UI_element_selected)->SetPosition_y(rect2.y);
				}
				ImGui::PopID();

				ImGui::PushID(UI_element_selected);
				if (ImGui::SliderInt("Width", &rect2.width, 0.0f, 700.0f, "%.3f", NULL))
				{
					m_GuiManager.controls.at(UI_element_selected)->SetPosition_width(rect2.width);
				}
				ImGui::PopID();

				ImGui::PushID(UI_element_selected);
				if (ImGui::SliderInt("Height", &rect2.height, 0.0f, 700.0f, "%.3f", NULL))
				{
					m_GuiManager.controls.at(UI_element_selected)->SetPosition_height(rect2.height);
				}
				ImGui::PopID();
			}
		}
	}
	else
	{
		ImGui::NewLine();
		ImGui::Text("	Buttons cannot be created or edited during play mode.");
	}
	

	ImGui::End();
}
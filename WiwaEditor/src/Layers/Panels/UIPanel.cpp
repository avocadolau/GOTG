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

	if (!Time::IsPlaying())
	{
		const char* items[] = { "Button", "Slider", "CheckBox", "Image","Text"};
		static const char* current_item = NULL;
		if (ImGui::CollapsingHeader("Create UI element"))
		{
			ImGui::Text("UI element type:");

			if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(items[n], is_selected))
					{
						current_item = items[n];
						if (is_selected)
							ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
					}
				}
				ImGui::EndCombo();
			}
		}

		if (current_item == "Button")
		{
			ImGui::Text("UI button creator:");
			ImGui::NewLine();

			ImGui::InputInt2("position", position);
			ImGui::InputInt2("size", size);
			if (ImGui::BeginPopupModal("WARNING"))
			{
				ImGui::Text("Check if all the requirements were filled \nbefore creating the UI element \n");
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();

			}
			Rect2i rect;
			rect.x = position[0];
			rect.y = position[1];
			rect.width = size[0];
			rect.height = size[1];
			AssetContainer(tex_path.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						tex_path = pathS;
						tex2_path = pathS;
					}
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text("Tex path");
			if (ImGui::Button("Create Button"))
			{
				if (Wiwa::FileSystem::Exists(tex_path.c_str()))
				{
					m_GuiManager.CreateGuiControl_Simple(GuiControlType::BUTTON, m_GuiManager.controls.size(), rect, tex_path.c_str(), nullptr);
				}
				else
				{
					ImGui::OpenPopup("WARNING");
				}
			}
		}

		if (current_item == "Slider")
		{
			ImGui::Text("UI slider creator:");
			ImGui::NewLine();

			ImGui::InputInt2("position", position);
			ImGui::InputInt2("size", size);
			if (ImGui::BeginPopupModal("WARNING"))
			{
				ImGui::Text("Check if all the requirements were filled \nbefore creating the UI element \nall elements need 2 textures.");
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();

			}
			Rect2i rect;
			rect.x = position[0];
			rect.y = position[1];
			rect.width = size[0];
			rect.height = size[1];
			AssetContainer(tex_path.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						tex_path = pathS;
					}
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text("Main tex path");
			AssetContainer(tex2_path.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						tex2_path = pathS;
					}
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text("Slider marker tex path");
			if (ImGui::Button("Create Slider"))
			{

				if (Wiwa::FileSystem::Exists(tex_path.c_str()) && Wiwa::FileSystem::Exists(tex2_path.c_str()))
				{
					Rect2i rect2;
					rect2.x = rect.x - (rect.width / 2);
					rect2.y = rect.y;
					rect2.width = (rect.width / 100);
					rect2.height = rect.height;
					m_GuiManager.CreateGuiControl(GuiControlType::SLIDER, m_GuiManager.controls.size(), rect, tex_path.c_str(), tex2_path.c_str(), rect2);
				}
				else
				{
					ImGui::OpenPopup("WARNING");
				}
			}
		}

		if (current_item == "CheckBox")
		{
			ImGui::Text("UI checkbox creator:");
			ImGui::NewLine();

			ImGui::InputInt2("position", position);
			ImGui::InputInt2("size", size);
			if (ImGui::BeginPopupModal("WARNING"))
			{
				ImGui::Text("Check if all the requirements were filled \nbefore creating the UI element \nall elements need 2 textures.");
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();

			}
			Rect2i rect;
			rect.x = position[0];
			rect.y = position[1];
			rect.width = size[0];
			rect.height = size[1];
			AssetContainer(tex_path.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						tex_path = pathS;
					}
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text("Unchecked tex path");
			AssetContainer(tex2_path.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						tex2_path = pathS;
					}
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text("Checked tex path");
			if (ImGui::Button("Create Checkbox"))
			{
				if (Wiwa::FileSystem::Exists(tex_path.c_str()) && Wiwa::FileSystem::Exists(tex2_path.c_str()))
				{
					m_GuiManager.CreateGuiControl_Simple(GuiControlType::CHECKBOX, m_GuiManager.controls.size(), rect, tex_path.c_str(), tex2_path.c_str());
				}
				else
				{
					ImGui::OpenPopup("WARNING");
				}
			}
		}

		if (current_item == "Image")
		{
			ImGui::Text("UI image creator:");
			ImGui::NewLine();

			ImGui::InputInt2("position", position);
			ImGui::InputInt2("size", size);
			if (ImGui::BeginPopupModal("WARNING"))
			{
				ImGui::Text("Check if all the requirements were filled \nbefore creating the UI element \nall elements need 2 textures.");
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();

			}
			Rect2i rect;
			rect.x = position[0];
			rect.y = position[1];
			rect.width = size[0];
			rect.height = size[1];
			AssetContainer(tex_path.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						tex_path = pathS;
						tex2_path = pathS;
					}
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text("Image tex path");
			if (ImGui::Button("Create Image"))
			{
				if (Wiwa::FileSystem::Exists(tex_path.c_str()) && Wiwa::FileSystem::Exists(tex2_path.c_str()))
				{
					m_GuiManager.CreateGuiControl_Simple(GuiControlType::IMAGE, m_GuiManager.controls.size(), rect, tex_path.c_str(), tex2_path.c_str());
				}
				else
				{
					ImGui::OpenPopup("WARNING");
				}
			}
		}

		if (current_item == "Text")
		{
			ImGui::Text("UI text creator:");
			ImGui::NewLine();

			ImGui::InputInt2("position", position);
			ImGui::InputInt2("size", size);
			ImGui::InputText("String", (char*)tex_path.c_str(),64);
			if (ImGui::BeginPopupModal("WARNING"))
			{
				ImGui::Text("Check if all the requirements were filled \nbefore creating the UI element \nall elements need 2 textures.");
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();

			}
			Rect2i rect;
			rect.x = position[0];
			rect.y = position[1];
			rect.width = size[0];
			rect.height = size[1];
			
			if (ImGui::Button("Create Text"))
			{
				if (tex_path.c_str() != nullptr)
				{
					m_GuiManager.CreateGuiControl_Text(GuiControlType::TEXT, m_GuiManager.controls.size(), rect, tex_path.c_str());
				}
				else
				{
					ImGui::OpenPopup("WARNING");
				}
			}
		}

		if (ImGui::CollapsingHeader("Edit UI element"))
		{
			if (m_GuiManager.controls.size() > 0)
			{
				buttonID = 0;
				sliderID = 0;
				checkboxID = 0;
				imageID = 0;
				textID = 0;
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
					if (m_GuiManager.controls.at(i)->type == GuiControlType::IMAGE)
					{
						ImGui::Text("Image # %i", imageID);
						imageID++;
					}
					if (m_GuiManager.controls.at(i)->type == GuiControlType::TEXT)
					{
						ImGui::Text("Text # %i", textID);
						textID++;
					}
					ImGui::SameLine();
					if (ImGui::Button("Edit"))
					{
						UI_element_selected = i;
						lastButtonID = buttonID;
						lastSliderID = sliderID;
						lastCheckboxID = checkboxID;
						lastImageID = imageID;
						lastTextID = textID;
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete"))
					{
						UI_element_selected = -1;
						m_GuiManager.DestroyGuiControl(m_GuiManager.controls.at(i));
					}

					ImGui::SameLine();
					if (ImGui::Button("Active"))
					{
						m_GuiManager.controls.at(i)->SwapActive(Wiwa::Application::Get().GetRenderer2D());
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
				if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::BUTTON)
				{
					ImGui::Text("Edit button # %i", lastButtonID);
				}
				if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::SLIDER)
				{
					ImGui::Text("Edit slider # %i", lastSliderID);
				}
				if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::CHECKBOX)
				{
					ImGui::Text("Edit checkbox # %i", lastCheckboxID);
				}
				if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::IMAGE)
				{
					ImGui::Text("Edit image # %i", lastImageID);
				}
				if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::TEXT)
				{
					ImGui::Text("Edit text # %i", lastTextID);
				}

				ImGui::PushID(UI_element_selected);
				if (ImGui::SliderInt("position x", &rect2.x, 0.0f, 1920.0f, "%.3f", NULL))
				{
					m_GuiManager.controls.at(UI_element_selected)->SetPosition_x(rect2.x);
					if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::SLIDER)
					{
						m_GuiManager.controls.at(UI_element_selected)->SetExtraPosition_x((rect2.x - (rect2.width / 2)));
					}
				}
				ImGui::PopID();

				ImGui::PushID(UI_element_selected);
				if (ImGui::SliderInt("position y", &rect2.y, 0.0f, 1080.0f, "%.3f", NULL))
				{
					m_GuiManager.controls.at(UI_element_selected)->SetPosition_y(rect2.y);
					if (m_GuiManager.controls.at(UI_element_selected)->GetType() == GuiControlType::SLIDER)
					{
						m_GuiManager.controls.at(UI_element_selected)->SetExtraPosition_y(rect2.y);
					}
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

void UIPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<SceneChangeEvent>({ &UIPanel::OnSceneChange, this });

}

bool UIPanel::OnSceneChange(SceneChangeEvent& e)
{
	UI_element_selected = -1;
	return true;
}
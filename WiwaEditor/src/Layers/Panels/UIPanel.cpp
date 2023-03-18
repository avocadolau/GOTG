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

	ImGui::Begin(iconName.c_str(), &active);
	
	ImGui::Text("UI creator panel");
	ImGui::NewLine();
	if (ImGui::CollapsingHeader("Canvas creation"))
	{
		if (ImGui::Button("Create Canvas"))
		{
			Wiwa::SceneManager::getActiveScene()->GetGuiManager().CreateGuiCanvas(Wiwa::SceneManager::getActiveScene()->GetGuiManager().canvas.size(), true);
			//m_GuiManager.CreateGuiControl_Simple(GuiControlType::BUTTON, 0, { 200,200,100,100 }, "assets/hola.png", nullptr, 0,1);

		}
		for (size_t i = 0; i < Wiwa::SceneManager::getActiveScene()->GetGuiManager().canvas.size(); i++)
		{
 			ImGui::PushID(i);
			ImGui::Text("Canvas %i", i);
			ImGui::SameLine();
			if(ImGui::Button("Select canvas"))
			{
				canvasSelected = i;
				Wiwa::SceneManager::getActiveScene()->GetGuiManager().SwapSelectedCanvas(Wiwa::SceneManager::getActiveScene()->GetGuiManager().canvas.at(i));
			}
			ImGui::SameLine();
			if(ImGui::Button("Swap active"))
			{
				Wiwa::SceneManager::getActiveScene()->GetGuiManager().canvas.at(i)->SwapActive();
			}
			ImGui::SameLine();
			if(ImGui::Button("Delete"))
			{
				Wiwa::SceneManager::getActiveScene()->GetGuiManager().DestroyCanvas(Wiwa::SceneManager::getActiveScene()->GetGuiManager().canvas.at(i));
				canvasSelected = -1;
			}
			ImGui::PopID();
		}
	}
	DrawGuiElementSelection();
	
	ImGui::End();
}

void UIPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<SceneChangeEvent>({ &UIPanel::OnSceneChange, this });

}

bool UIPanel::OnSceneChange(SceneChangeEvent& e)
{
	//UI_element_selected = -1;
	return true;
}

void UIPanel::DrawGuiElementSelection()
{
	const char* items[] = {"Button", "Slider", "CheckBox", "Image","Text"};
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
		if (current_item != NULL)
		{
			if (ImGui::CollapsingHeader(current_item))
			{
				DrawGuiElementCreation(current_item);
			}
		}
		else
		{
			TextCentered("Select an item");
		}
	}

}

void UIPanel::DrawGuiElementCreation(const char* current_item)
{
	type = GetSelectedElementType(current_item);

	switch (type)
	{
	case GuiType::BUTTON:
		DrawButtonCreation(canvasSelected, Wiwa::SceneManager::getActiveScene()->GetGuiManager());
		
		break;
	case GuiType::TEXT:
		DrawTextCreation(canvasSelected, Wiwa::SceneManager::getActiveScene()->GetGuiManager());
		break;
	case GuiType::CHECKBOX:
		DrawCheckboxCreation(canvasSelected, Wiwa::SceneManager::getActiveScene()->GetGuiManager());
		break;
	case GuiType::SLIDER:
		DrawSliderCreation(canvasSelected, Wiwa::SceneManager::getActiveScene()->GetGuiManager());
		break;
	case GuiType::IMAGE:
		DrawImageCreation(canvasSelected, Wiwa::SceneManager::getActiveScene()->GetGuiManager());
		break;
	default:
		break;
	}
}

GuiType UIPanel::GetSelectedElementType(const char* current_item)
{
	if (current_item == "Button")
	{
		return GuiType::BUTTON;
	}
	if (current_item == "Slider")
	{
		return GuiType::SLIDER;
	}
	if (current_item == "CheckBox")
	{
		return GuiType::CHECKBOX;
	}
	if (current_item == "Image")
	{
		return GuiType::IMAGE;
	}
	if (current_item == "Text")
	{
		return GuiType::TEXT;
	}

	return GuiType::TEXT;
}



void UIPanel::DrawButtonCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager)
{
	ImGui::PushID("CreateButton");
	if (canvas_id > -1)ImGui::Text("Canvas: %i", canvas_id);
	if (canvas_id < 0)ImGui::Text("Please select a canvas");
	ImGui::InputFloat2("Position", position);
	ImGui::InputFloat2("Size", size);
	AssetContainer(pathForAsset.c_str());
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
				pathForAsset = pathS;
			}
		}

		ImGui::EndDragDropTarget();
	}
	
	const char* items[] = { "Play","Change Scene","Quit"};
	static const char* current_item = NULL;
	ImGui::Text("Callback type:");

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
	if (current_item != NULL)
	{
		if (current_item == "Change Scene")
		{
			callbackID = 0;
		}
		else if (current_item == "Play")
		{
			callbackID = 1;
		}
		else if (current_item == "Quit")
		{
			callbackID = 2;
		}
	}
	
	if (ImGui::Button("Create button"))
	{
		Wiwa::Rect2i rect;
		rect.x = position[0];
		rect.y = position[1];
		rect.height = size[0];
		rect.width = size[1];
		if(canvas_id > -1) m_GuiManager.CreateGuiControl_Simple(GuiControlType::BUTTON, m_GuiManager.canvas.at(canvas_id)->controls.size(), rect, pathForAsset.c_str(), nullptr, canvas_id, callbackID);
	}

	ImGui::PopID();
}
void UIPanel::DrawSliderCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager)
{
	ImGui::PushID("CreateSlider");
	if (canvas_id > -1)ImGui::Text("Canvas: %i", canvas_id);
	if (canvas_id < 0)ImGui::Text("Please select a canvas");
	ImGui::InputFloat2("Position", position);
	ImGui::InputFloat2("Size", size);
	AssetContainer(pathForAsset.c_str());
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
				pathForAsset = pathS;
			}
		}

		ImGui::EndDragDropTarget();
	}
	AssetContainer(pathForExtraAsset.c_str());
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
				pathForExtraAsset = pathS;
			}
		}

		ImGui::EndDragDropTarget();
	}
	const char* items[] = { "Play","Change Scene","Quit" };
	static const char* current_item = NULL;
	ImGui::Text("Callback type:");

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
	if (current_item != NULL)
	{
		if (current_item == "Change Scene")
		{
			callbackID = 0;
		}
		else if (current_item == "Play")
		{
			callbackID = 1;
		}
		else if (current_item == "Quit")
		{
			callbackID = 2;
		}
	}

	if (ImGui::Button("Create slider"))
	{
		Wiwa::Rect2i rect;
		rect.x = position[0];
		rect.y = position[1];
		rect.height = size[0];
		rect.width = size[1];
		Wiwa::Rect2i rect2;
		rect2.x = rect.x - (rect.width / 2);
		rect2.y = rect.y;
		rect2.width = (rect.width / 100);
		rect2.height = rect.height;
		if (canvas_id > -1) m_GuiManager.CreateGuiControl(GuiControlType::SLIDER, m_GuiManager.canvas.at(canvas_id)->controls.size(), rect, pathForAsset.c_str(), pathForExtraAsset.c_str(),rect2, canvas_id, callbackID);
	}

	ImGui::PopID();
}
void UIPanel::DrawImageCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager)
{
	ImGui::PushID("CreateImage");
	if (canvas_id > -1)ImGui::Text("Canvas: %i", canvas_id);
	if (canvas_id < 0)ImGui::Text("Please select a canvas");
	ImGui::InputFloat2("Position", position);
	ImGui::InputFloat2("Size", size);
	AssetContainer(pathForAsset.c_str());
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
				pathForAsset = pathS;
			}
		}

		ImGui::EndDragDropTarget();
	}

	const char* items[] = { "Play","Change Scene","Quit" };
	static const char* current_item = NULL;
	ImGui::Text("Callback type:");

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
	if (current_item != NULL)
	{
		if (current_item == "Change Scene")
		{
			callbackID = 0;
		}
		else if (current_item == "Play")
		{
			callbackID = 1;
		}
		else if (current_item == "Quit")
		{
			callbackID = 2;
		}
	}

	if (ImGui::Button("Create Image"))
	{
		Wiwa::Rect2i rect;
		rect.x = position[0];
		rect.y = position[1];
		rect.height = size[0];
		rect.width = size[1];
		if (canvas_id > -1) m_GuiManager.CreateGuiControl_Simple(GuiControlType::IMAGE, m_GuiManager.canvas.at(canvas_id)->controls.size(), rect, pathForAsset.c_str(), nullptr, canvas_id, callbackID);
	}

	ImGui::PopID();
}
void UIPanel::DrawTextCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager)
{

	ImGui::PushID("CreateText");
	if (canvas_id > -1)ImGui::Text("Canvas: %i", canvas_id);
	if (canvas_id < 0)ImGui::Text("Please select a canvas");
	ImGui::InputFloat2("position", position);
	ImGui::InputFloat2("size", size);
	ImGui::InputText("String", (char*)pathForAsset.c_str(),64);

	if (ImGui::Button("Create Text"))
	{
		Wiwa::Rect2i rect;
		rect.x = position[0];
		rect.y = position[1];
		rect.height = size[0];
		rect.width = size[1];
		if (canvas_id > -1) m_GuiManager.CreateGuiControl_Text(GuiControlType::TEXT, m_GuiManager.canvas.at(canvas_id)->controls.size(), rect, pathForAsset.c_str(),canvas_id,callbackID);
	}
	ImGui::PopID();
}
void UIPanel::DrawCheckboxCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager)
{
	ImGui::PushID("CreateCheckbox");
	if (canvas_id > -1)ImGui::Text("Canvas: %i", canvas_id);
	if (canvas_id < 0)ImGui::Text("Please select a canvas");
	ImGui::InputFloat2("Position", position);
	ImGui::InputFloat2("Size", size);
	AssetContainer(pathForAsset.c_str());
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
				pathForAsset = pathS;
			}
		}

		ImGui::EndDragDropTarget();
	}
	AssetContainer(pathForExtraAsset.c_str());
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
				pathForExtraAsset = pathS;
			}
		}

		ImGui::EndDragDropTarget();
	}
	const char* items[] = { "Play","Change Scene","Quit" };
	static const char* current_item = NULL;
	ImGui::Text("Callback type:");

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
	if (current_item != NULL)
	{
		if (current_item == "Change Scene")
		{
			callbackID = 0;
		}
		else if (current_item == "Play")
		{
			callbackID = 1;
		}
		else if (current_item == "Quit")
		{
			callbackID = 2;
		}
	}

	if (ImGui::Button("Create checkbox"))
	{
		Wiwa::Rect2i rect;
		rect.x = position[0];
		rect.y = position[1];
		rect.height = size[0];
		rect.width = size[1];
		if (canvas_id > -1) m_GuiManager.CreateGuiControl_Simple(GuiControlType::CHECKBOX, m_GuiManager.canvas.at(canvas_id)->controls.size(), rect, pathForAsset.c_str(), pathForExtraAsset.c_str(), canvas_id, callbackID);
	}

	ImGui::PopID();
}
#include <wipch.h>
#include "UIEditorPanel.h"
#include <unordered_map>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/scripting/ScriptEngine.h>
#include <Wiwa/core/Application.h>
UIEditorPanel::UIEditorPanel(EditorLayer* instance)
	: Panel("UI editor", ICON_FK_MAGIC, instance)
{
	elementSelected = -1;
}

UIEditorPanel::~UIEditorPanel()
{

}

void UIEditorPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	ImGui::Text("UI editor panel");
	GetSelectedCanvas();
	if (ImGui::CollapsingHeader("Edit canvas"))
	{
		
		if(canvasSelectedID < 0)ImGui::Text("Select a canvas to edit");
		if (canvasSelectedID > -1) {
			ImGui::Text("Canvas: %i", canvasSelectedID);
			ImGui::NewLine();
			DrawCanvasItems();
			
		}
		
	}
	ImGui::End();
}

void UIEditorPanel::GetSelectedCanvas()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

	for (size_t i = 0; i < gm.canvas.size(); i++)
	{
		if (gm.canvas.at(i)->selected)
		{
			canvasSelectedID = (int)i;
		}
	}

	if (gm.canvas.size() <= 0) canvasSelectedID = -1;
}

void UIEditorPanel::DrawCanvasItems()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	Wiwa::GuiCanvas* canvas = gm.canvas.at(canvasSelectedID);
	size_t controls_size = canvas->controls.size();
	for (size_t i = 0; i < controls_size; i++)
	{
		ImGui::Text(canvas->controls.at(i)->name.c_str());
		ImGui::SameLine();
		ImGui::Text("%i", canvas->controls.at(i)->id);
		ImGui::SameLine();
		ImGui::PushID(i);
		if (ImGui::Button("Edit"))
		{
			elementSelected = (int)i;
			SetInitialValues(canvas->controls.at(i));
		}
		ImGui::SameLine();
		if (ImGui::Button("Activate"))
		{
			canvas->controls.at(i)->SwapActive(Wiwa::Application::Get().GetRenderer2D());
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			gm.DestroyGuiControl(canvas->controls.at(i), canvas);
			elementSelected = -1;
		}
		ImGui::PopID();
	}
	if(elementSelected > -1)OpenEditGuiControl(canvas->controls.at(elementSelected));
	
	
}

void UIEditorPanel::SetInitialValues(Wiwa::GuiControl* control)
{
	pos[0] = control->position.x;
	pos[1] = control->position.y;
	size[0] = control->position.width;
	size[1] = control->position.height;
	originPos[0] = control->texturePosition.x;
	originPos[1] = control->texturePosition.y;
	originSize[0] = control->texturePosition.width;
	originSize[1] = control->texturePosition.height;
	callbackID = control->callbackID;
	audioEventForButton = control->audioEventForButton;
	if (control->type == Wiwa::GuiControlType::SLIDER || control->type == Wiwa::GuiControlType::BAR)
	{
		extraOriginPos[0] = control->extraTexturePosition.x;
		extraOriginPos[1] = control->extraTexturePosition.y;
		extraOriginSize[0] = control->extraTexturePosition.width;
		extraOriginSize[1] = control->extraTexturePosition.height;
	}
	pathForAsset = Wiwa::Resources::getResourcePathById<Wiwa::Image>(control->textId1);
	pathForExtraAsset = Wiwa::Resources::getResourcePathById<Wiwa::Image>(control->textId2);
}
void UIEditorPanel::OpenEditGuiControl(Wiwa::GuiControl* control)
{
	if (ImGui::CollapsingHeader("Edit control"))
	{
		ImGui::Text(control->name.c_str());
		ImGui::SameLine();
		ImGui::Text("%i", control->id);
		ImGui::NewLine();
		ImGui::InputInt2("position",pos);
		ImGui::InputInt2("size", size);
		ImGui::InputInt2("origin position", originPos);
		ImGui::InputInt2("origin size", originSize);
		if (control->type == Wiwa::GuiControlType::SLIDER || control->type == Wiwa::GuiControlType::BAR)
		{
			ImGui::InputInt2("origin position slider", extraOriginPos);
			ImGui::InputInt2("origin size slider", extraOriginSize);
		}
		if (control->type != Wiwa::GuiControlType::TEXT || control->type != Wiwa::GuiControlType::BAR)
		{

			Wiwa::Application& app = Wiwa::Application::Get();
			if (callbackID != WI_INVALID_INDEX)
			{
				size_t cbcount = app.GetCallbacksCount();

				if (cbcount > 0) {
					Wiwa::Callback* current_cb = app.getCallbackAt(callbackID);

					ImGui::Text("Callback type:");

					if (ImGui::BeginCombo("##combo", current_cb->getName().c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (size_t n = 0; n < cbcount; n++)
						{
							bool is_selected = n == callbackID; // You can store your selection however you want, outside or inside your objects
							current_cb = app.getCallbackAt(n);
							switch (control->type)
							{
							case Wiwa::GuiControlType::BUTTON:
								if (current_cb->getParamCount() == 0) {
									if (ImGui::Selectable(current_cb->getName().c_str(), is_selected))
									{
										callbackID = n;
										if (is_selected)
											ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
									}
								}
								break;
							case Wiwa::GuiControlType::CHECKBOX:
								if (current_cb->getParamCount() == 1) {
									if (current_cb->getParamAt(0)->hash == (size_t)TypeHash::Bool) {
										if (ImGui::Selectable(current_cb->getName().c_str(), is_selected))
										{
											callbackID = n;
											if (is_selected)
												ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
										}
									}
								}
								break;
							case Wiwa::GuiControlType::SLIDER:
								if (current_cb->getParamCount() == 1) {
									if (current_cb->getParamAt(0)->hash == (size_t)TypeHash::Float) {
										if (ImGui::Selectable(current_cb->getName().c_str(), is_selected))
										{
											callbackID = n;
											if (is_selected)
												ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
										}
									}
								}
								break;
							case Wiwa::GuiControlType::IMAGE:
								if (current_cb->getParamCount() == 0) {
									if (ImGui::Selectable(current_cb->getName().c_str(), is_selected))
									{
										callbackID = n;
										if (is_selected)
											ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
									}
								}
								break;
							default:
								break;
							}
						}
						ImGui::EndCombo();
					}
				}
			}


			 
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
			if (control->type == Wiwa::GuiControlType::CHECKBOX || control->type == Wiwa::GuiControlType::SLIDER)
			{
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
			}
			
			ImGui::InputText("Audio event name", (char*)audioEventForButton.c_str(), 64);
			
		}
		
		if (ImGui::Button("Update"))
		{
			UpdateElements(control);
		}
		
	}
}

void UIEditorPanel::VectorEdit(std::vector<Wiwa::Rect2i> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Animation frame: %d", i);
		ImGui::InputInt2("Anim position", &(list.at(i).x, list.at(i).y));
		ImGui::InputInt2("Anim size", &(list.at(i).width, list.at(i).height));
		ImGui::PopID();
	}
	if (ImGui::Button("+"))
	{
		list.push_back({ 0,0,0,0 });
	}
	ImGui::SameLine();
	if (ImGui::Button("-"))
	{
		list.erase(list.begin() + list.size() - 1);
	}

	animationRects = list;
}

void UIEditorPanel::UpdateElements(Wiwa::GuiControl* control)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
	Wiwa::Vector2i vector = { pos[0],pos[1] };
	Wiwa::Size2i size_ = { size[0],size[1] };
	Wiwa::Rect2i originTexRect = { originPos[0],originPos[1],originSize[0],originSize[1] };
	Wiwa::Rect2i extraOriginTexRect = { extraOriginPos[0],extraOriginPos[1],extraOriginSize[0],extraOriginSize[1] };
	control->position.x = pos[0];
	control->position.y = pos[1];
	control->position.width = size[0];
	control->position.height = size[1];
	control->audioEventForButton = audioEventForButton.c_str();
	control->positionsForAnimations = animationRects;
	if (callbackID != WI_INVALID_INDEX)
	{
		control->callbackID = callbackID;
		control->callback = Wiwa::Application::Get().getCallbackAt(callbackID);
	}
	if(control->type != Wiwa::GuiControlType::TEXT)r2d.UpdateInstancedQuadTexClip(Wiwa::SceneManager::getActiveScene(), control->id_quad_normal, control->texture->GetSize(), originTexRect);
	r2d.UpdateInstancedQuadTexSize(Wiwa::SceneManager::getActiveScene(), control->id_quad_normal, { pos[0], pos[1] }, size_, Wiwa::Renderer2D::Pivot::UPLEFT);
	if (control->type != Wiwa::GuiControlType::TEXT)
	{
		control->textId1 = Wiwa::Resources::Load<Wiwa::Image>(pathForAsset.c_str());
		control->texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(control->textId1);
		r2d.UpdateInstancedQuadTexTexture(Wiwa::SceneManager::getActiveScene(), control->id_quad_normal, control->texture->GetTextureId());
		if (control->type == Wiwa::GuiControlType::SLIDER || control->type == Wiwa::GuiControlType::BAR)
		{
			control->textId2 = Wiwa::Resources::Load<Wiwa::Image>(pathForExtraAsset.c_str());
			control->extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(control->textId2);
			r2d.UpdateInstancedQuadTexTexture(Wiwa::SceneManager::getActiveScene(), control->id_quad_extra, control->extraTexture->GetTextureId());
		}
	}
}

void UIEditorPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>({ &UIEditorPanel::OnSceneChange, this });

}
bool UIEditorPanel::OnSceneChange(Wiwa::SceneChangeEvent& e)
{
	elementSelected = -1;
	pos[0] = 0; pos[1] = 0;
	size[0] = 0; size[1] = 0;
	originPos[0] = 0; originPos[1] = 0;
	originSize[0] = 0; originSize[1] = 0;
	extraOriginPos[0] = 0; extraOriginPos[1] = 0;
	extraOriginSize[0] = 0; extraOriginSize[1] = 0;
	callbackID = WI_INVALID_INDEX;
	pathForAsset = "";
	pathForExtraAsset = "";
	audioEventForButton = "";
	return true;
}
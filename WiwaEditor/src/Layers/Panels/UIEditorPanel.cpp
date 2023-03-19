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
			canvasSelectedID = i;
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
		if (ImGui::Button("Edit"))
		{
			elementSelected = i;
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
	}
	if(elementSelected > -1)OpenEditGuiControl(canvas->controls.at(elementSelected));
	
	
}

void UIEditorPanel::SetInitialValues(Wiwa::GuiControl* control)
{
	pos[0] = control->position.x;
	pos[1] = control->position.y;
	size[0] = control->position.width;
	size[1] = control->position.height;
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
		ImGui::InputFloat2("position",pos);
		ImGui::InputFloat2("size", size);
		if (control->type != Wiwa::GuiControlType::TEXT)
		{
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
		}
		
		UpdateElements(control);
		
	}
}

void UIEditorPanel::UpdateElements(Wiwa::GuiControl* control)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	Wiwa::Vector2i vector = { pos[0],pos[1] };
	Wiwa::Size2i size_ = { size[0],size[1] };
	control->position.x = pos[0];
	control->position.y = pos[1];
	control->position.width = size[0];
	control->position.height = size[1];
	r2d.UpdateInstancedQuadTexPosition(Wiwa::SceneManager::getActiveScene(), control->id_quad_normal, vector, Wiwa::Renderer2D::Pivot::UPLEFT);
	r2d.UpdateInstancedQuadTexSize(Wiwa::SceneManager::getActiveScene(), control->id_quad_normal, size_);
	if (control->type != Wiwa::GuiControlType::TEXT)
	{
		control->textId1 = Wiwa::Resources::Load<Wiwa::Image>(pathForAsset.c_str());
		control->texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(control->textId1);
		r2d.UpdateInstancedQuadTexTexture(Wiwa::SceneManager::getActiveScene(), control->id_quad_normal, control->texture->GetTextureId());
		if (control->type == Wiwa::GuiControlType::SLIDER)
		{
			control->textId2 = Wiwa::Resources::Load<Wiwa::Image>(pathForExtraAsset.c_str());
			control->extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(control->textId2);
			r2d.UpdateInstancedQuadTexTexture(Wiwa::SceneManager::getActiveScene(), control->id_quad_extra, control->extraTexture->GetTextureId());
		}
	}
}
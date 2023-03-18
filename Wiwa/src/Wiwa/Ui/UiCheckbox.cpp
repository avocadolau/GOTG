#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiCheckbox.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiCheckbox::GuiCheckbox(Scene* scene, unsigned int id, Rect2i bounds,const char* path, const char* extraPath, size_t callbackID) : GuiControl(scene, GuiControlType::CHECKBOX, id)
	{
		this->position = bounds;
		name = "Checkbox";
		m_Scene = scene;

		if (path != "") {
			textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
			texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		}

		if (extraPath != "") {
			textId2 = Wiwa::Resources::Load<Wiwa::Image>(extraPath);
			extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);
		}

		this->callbackID = callbackID;
		if (callbackID != WI_INVALID_INDEX)
			callback = Wiwa::Application::Get().getCallbackAt(callbackID);
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		//We create only once the texture
		//id_quad_disabled = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_focused = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_pressed = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_selected = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);

		canClick = true;
	}

	GuiCheckbox::~GuiCheckbox()
	{

	}

	bool GuiCheckbox::Update()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		if (state != GuiControlState::DISABLED)
		{
			// L14: TODO 3_D: Update the state of the GUiButton according to the mouse position
			float mouseX, mouseY;
			mouseX = Wiwa::Input::GetMouseX();
			mouseY = Wiwa::Input::GetMouseY();


			if ((mouseX > position.x && mouseX < (position.x + position.width)) &&
				(mouseY > position.y && mouseY < position.y + position.height))
			{
				state = GuiControlState::FOCUSED;

				if (Wiwa::Input::IsMouseButtonPressed(0) && refreshTime > 60)
				{
					state = GuiControlState::PRESSED;
					checked = !checked;
					void* params[] = { &checked };
					callback->Execute(params);
					refreshTime = 0;
				}
				else
				{
					state = GuiControlState::NORMAL;
				}
			}
		}

		SwapTexture();
		refreshTime++;

		return false;
	}

	bool GuiCheckbox::Draw(Renderer2D* render)
	{
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			render->UpdateInstancedQuad(m_Scene, id_quad_normal, newPosition, newSize, color);		} break;
		

		case GuiControlState::NORMAL:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			render->UpdateInstancedQuad(m_Scene, id_quad_normal, newPosition, newSize, color);		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			render->UpdateInstancedQuad(m_Scene, id_quad_normal, newPosition, newSize, color);		} break;
		case GuiControlState::PRESSED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			render->UpdateInstancedQuad(m_Scene, id_quad_normal, newPosition, newSize, color);		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			render->UpdateInstancedQuad(m_Scene, id_quad_normal, newPosition, newSize, color);		} break;
		default:
			break;
		}

		return false;
	}
	bool GuiCheckbox::SwapTexture()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		if (checked)
		{
			texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);
		}
		else
		{
			texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		}
		r2d.RemoveInstance(m_Scene, id_quad_normal);
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);

		return true;
	}
}
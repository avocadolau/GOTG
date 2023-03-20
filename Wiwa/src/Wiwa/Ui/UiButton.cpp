#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiButton.h"
#include <Wiwa/utilities/Log.h>
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiButton::GuiButton(Scene* scene, unsigned int id, Rect2i bounds,const char* path, const char* extraPath, size_t callbackID, Rect2i boundsOriginTex) : GuiControl(scene, GuiControlType::BUTTON, id)
	{
		this->position = bounds;
		texturePosition = boundsOriginTex;
		name = "Button";
		m_Scene = scene;
		active = true;
		if (path != "") {
			textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
			texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		}
		//textId2 = Wiwa::Resources::Load<Wiwa::Image>(extraPath);
		//extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);
		this->callbackID = callbackID;
		if(callbackID != WI_INVALID_INDEX)
			callback = Wiwa::Application::Get().getCallbackAt(callbackID);

		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, texturePosition, Wiwa::Renderer2D::Pivot::UPLEFT);
		
		
		
		
		state = GuiControlState::NORMAL;
		canClick = true;
	}

	GuiButton::~GuiButton()
	{

	}

	bool GuiButton::Update()
	{
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
				if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = GuiControlState::PRESSED;
					clicked = true;
				}
				
				if (Wiwa::Input::IsMouseButtonReleased(0) && clicked)
				{
					clicked = false;
					if(callback)
						callback->Execute();
				}
			}
			else
			{
				state = GuiControlState::NORMAL;
			}

		}
		SwapTexture();

		return false;
	}

	bool GuiButton::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		//Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;

			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;

		case GuiControlState::NORMAL:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;

			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;
		case GuiControlState::PRESSED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;

			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;

			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;
		default:
			break;
		}

		return false;
	}

	bool GuiButton::SwapTexture()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		Color4f color = { 1.0f,1.0f,1.0f,1.0f };
		switch (state)
		{
		case Wiwa::GuiControlState::DISABLED:
			break;
		case Wiwa::GuiControlState::NORMAL:
			color = { 1.0f,1.0f,1.0f,1.0f };
			break;
		case Wiwa::GuiControlState::FOCUSED:
			color = { 0.6f, 0.6f, 0.6f, 1.0f };
			break;
		case Wiwa::GuiControlState::PRESSED:
			color = { 0.1f, 0.1f, 0.1f, 1.0f };
			break;
		case Wiwa::GuiControlState::SELECTED:
			color = { 0.6f, 0.6f, 0.6f, 1.0f };
			break;
		default:
			break;
		}

		r2d.UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);

		return true;
	}
}
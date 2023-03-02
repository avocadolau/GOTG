#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiImage.h"
#include <Wiwa/utilities/Log.h>
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiImage::GuiImage(Scene* scene, unsigned int id, Rect2i bounds, const char* path) : GuiControl(scene, GuiControlType::IMAGE, id)
	{
		this->position = bounds;
		this->texture = texture;
		textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		//id_quad_disabled = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_focused = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_pressed = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_selected = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);

		state = GuiControlState::NORMAL;
		canClick = true;
	}

	GuiImage::~GuiImage()
	{

	}

	bool GuiImage::Update()
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
				}

			}
			else
			{
				state = GuiControlState::NORMAL;
			}

		}

		return false;
	}

	bool GuiImage::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::CENTER);		} break;

		case GuiControlState::NORMAL:
		{
			Wiwa::Vector2i newPosition = Wiwa::Vector2i{ position.x,position.y };
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);

		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{

			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::CENTER);		} break;
		case GuiControlState::PRESSED:
		{

			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::CENTER);		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::CENTER);		}break;
		default:
			break;
		}

		return false;
	}
}
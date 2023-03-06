#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiButton.h"
#include <Wiwa/utilities/Log.h>
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiButton::GuiButton(Scene* scene, unsigned int id, Rect2i bounds,const char* path, const char* extraPath) : GuiControl(scene, GuiControlType::BUTTON, id)
	{
		this->position = bounds;
		
		textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);

		//textId2 = Wiwa::Resources::Load<Wiwa::Image>(extraPath);
		//extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);

		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		//id_quad_disabled = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), {position.x,position.y}, {position.width,position.height}, Wiwa::Renderer2D::Pivot::CENTER);
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_focused = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_pressed = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_selected = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
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
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;

		case GuiControlState::NORMAL:
		{
			Wiwa::Vector2i newPosition = Wiwa::Vector2i{ position.x,position.y };
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, {position.x,position.y}, Wiwa::Renderer2D::Pivot::UPLEFT);

		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;
		case GuiControlState::PRESSED:
		{
			
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);		}break;
		default:
			break;
		}

		return false;
	}

	bool GuiButton::SwapTexture()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		if (state == GuiControlState::DISABLED)
		{
			const Color4f color = { 0.1f, 0.1f, 0.1f, 0.5f };

			Rect2i rect;
			rect.x = texture->GetSize().x;
			rect.y = texture->GetSize().y;
			rect.width = texture->GetSize().w;
			rect.height = texture->GetSize().h;

			r2d.RemoveInstance(m_Scene, id_quad_normal);
			id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, color, rect, Wiwa::Renderer2D::Pivot::UPLEFT);
		}
		if (state == GuiControlState::NORMAL)
		{
			Rect2i rect;
			rect.x = texture->GetSize().x;
			rect.y = texture->GetSize().y;
			rect.width = texture->GetSize().w;
			rect.height = texture->GetSize().h;

			r2d.RemoveInstance(m_Scene, id_quad_normal);
			id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Color::WHITE, rect, Wiwa::Renderer2D::Pivot::UPLEFT);
		}
		if (state == GuiControlState::FOCUSED)
		{
			const Color4f color = { 0.6f, 0.6f, 0.6f, 1.0f };

			Rect2i rect;
			rect.x = texture->GetSize().x;
			rect.y = texture->GetSize().y;
			rect.width = texture->GetSize().w;
			rect.height = texture->GetSize().h;

			r2d.RemoveInstance(m_Scene, id_quad_normal);
			id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, color, rect, Wiwa::Renderer2D::Pivot::UPLEFT);
		}
		if (state == GuiControlState::PRESSED)
		{
			const Color4f color = { 0.1f, 0.1f, 0.1f, 1.0f };

			Rect2i rect;
			rect.x = texture->GetSize().x;
			rect.y = texture->GetSize().y;
			rect.width = texture->GetSize().w;
			rect.height = texture->GetSize().h;

			r2d.RemoveInstance(m_Scene, id_quad_normal);
			id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, color, rect, Wiwa::Renderer2D::Pivot::UPLEFT);
		}
		if (state == GuiControlState::SELECTED)
		{
			const Color4f color = { 0.6f, 0.6f, 0.6f, 1.0f };

			Rect2i rect;
			rect.x = texture->GetSize().x;
			rect.y = texture->GetSize().y;
			rect.width = texture->GetSize().w;
			rect.height = texture->GetSize().h;

			r2d.RemoveInstance(m_Scene, id_quad_normal);
			id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, color, rect, Wiwa::Renderer2D::Pivot::UPLEFT);
		}

		return true;
	}
}
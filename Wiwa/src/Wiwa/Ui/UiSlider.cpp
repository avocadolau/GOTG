#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiSlider.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiSlider::GuiSlider(Scene* scene, unsigned int id, Rect2i bounds, Rect2i sliderBounds, const char* path, const char* slider_path, int callbackID) : GuiControl(scene, GuiControlType::SLIDER, id)
	{
		this->position = bounds;
		this->extraPosition = sliderBounds;
		name = "Slider";
		m_Scene = scene;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		this->callbackID = callbackID;
		callback = Wiwa::Application::Get().getCallbackAt(callbackID);
		textId2 = Wiwa::Resources::Load<Wiwa::Image>(slider_path);
		extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);
		id_quad_extra = r2d.CreateInstancedQuadTex(m_Scene, extraTexture->GetTextureId(), extraTexture->GetSize(), { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::CENTER);


		textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		//id_quad_disabled = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_focused = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_pressed = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		//id_quad_selected = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);

		
		state = GuiControlState::NORMAL;
		//canClick = true;
	}

	GuiSlider::~GuiSlider()
	{

	}

	bool GuiSlider::Update()
	{
		if (state != GuiControlState::DISABLED)
		{
			if (canClick == true)
			{
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
						extraPosition.x = mouseX;
					}
					else
					{
						state = GuiControlState::NORMAL;
						/*NotifyObserver();*/
					}
				}
			}
		}

		return false;
	}

	bool GuiSlider::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			//render->DrawTexture2(textureForSlider, extraPosition.x, extraPosition.y, NULL); <-- Old way to do it (example)
			render->UpdateInstancedQuad(m_Scene, id_quad_extra, { extraPosition.x - (position.width / 2),extraPosition.y }, {extraPosition.width,extraPosition.height},color);
			render->UpdateInstancedQuad(m_Scene, id_quad_normal, { position.x,position.y }, {position.width,position.height},color);

			
		} break;

		case GuiControlState::NORMAL:
		{
			render->UpdateInstancedQuad(m_Scene, id_quad_extra, { extraPosition.x - (position.width / 2),extraPosition.y }, { extraPosition.width,extraPosition.height }, color);

			render->UpdateInstancedQuad(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, color);

		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			render->UpdateInstancedQuad(m_Scene, id_quad_extra, { extraPosition.x - (position.width / 2),extraPosition.y }, { extraPosition.width,extraPosition.height }, color);

			render->UpdateInstancedQuad(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, color);

		} break;
		case GuiControlState::PRESSED:
		{
			render->UpdateInstancedQuad(m_Scene, id_quad_extra, { extraPosition.x - (position.width / 2),extraPosition.y }, { extraPosition.width,extraPosition.height }, color);


			render->UpdateInstancedQuad(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, color);


		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			render->UpdateInstancedQuad(m_Scene, id_quad_extra, { extraPosition.x - (position.width / 2),extraPosition.y }, { extraPosition.width,extraPosition.height }, color);


			render->UpdateInstancedQuad(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, color);

		}break;
		default:
			break;
		}

		return false;
	}
}
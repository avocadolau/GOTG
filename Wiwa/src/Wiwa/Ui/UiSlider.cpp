#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiSlider.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/audio/Audio.h>

namespace Wiwa
{
	GuiSlider::GuiSlider(Scene* scene, unsigned int id, Rect2i bounds, Rect2i sliderBounds, const char* path, const char* slider_path, size_t callbackID, Rect2i boundsOriginTex, Rect2i sliderOriginTex, const char* audioEventName) : GuiControl(scene, GuiControlType::SLIDER, id)
	{
		this->position = bounds;
		this->extraPosition = sliderBounds;
		texturePosition = boundsOriginTex;
		extraTexturePosition = sliderOriginTex;
		name = "Slider";
		m_Scene = scene;
		active = true;
		text = "none";
		audioEventForButton = audioEventName;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		this->callbackID = callbackID;
		if (callbackID != WI_INVALID_INDEX)
			callback = Wiwa::Application::Get().getCallbackAt(callbackID);
		textId2 = Wiwa::Resources::Load<Wiwa::Image>(slider_path);
		extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);


		textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, texturePosition, Wiwa::Renderer2D::Pivot::UPLEFT);
		id_quad_extra = r2d.CreateInstancedQuadTex(m_Scene, extraTexture->GetTextureId(), extraTexture->GetSize(), { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, extraTexturePosition, Wiwa::Renderer2D::Pivot::UPLEFT);
		
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
				}
				if (state == GuiControlState::FOCUSED)
				{
					/*if (Wiwa::Input::IsMouseButtonPressed(0))
					{
						state = GuiControlState::PRESSED;
						extraPosition.width = (int)mouseX - extraPosition.x;
						clicked = true;
					}

					if (Wiwa::Input::IsMouseButtonReleased(0) && clicked)
					{
						clicked = false;
						value = (((float)extraPosition.width / (float)position.width) * 100);
						void* params[] = { &value };
						callback->Execute(params);
					}*/

					//RIGHT
					if (Wiwa::Input::IsButtonPressed(0, 12))
					{
						extraPosition.width += 1;
						if (extraPosition.width >= position.width)
						{
							extraPosition.width = position.width;
						}

					}
					//LEFT
					if (Wiwa::Input::IsButtonPressed(0, 14))
					{
						extraPosition.width -= 1;
						if (extraPosition.width <= 0)
						{
							extraPosition.width = 0;
						}

					}
					if (Wiwa::Input::IsButtonPressed(0, 0))
					{
						state = GuiControlState::PRESSED;
						clicked = true;
					}
					if (clicked && Wiwa::Input::IsButtonReleased(0, 0))
					{
						clicked = false;
						value = (((float)extraPosition.width / (float)position.width) * 100);
						void* params[] = { &value };
						if (Audio::FindEvent(audioEventForButton.c_str()) != Audio::INVALID_ID)
						{
							Audio::PostEvent(audioEventForButton.c_str());
						}
						if(callback)
							callback->Execute(params);
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
		Color4f color = { 1.0f,1.0f,1.0f,1.0f };

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			//render->DrawTexture2(textureForSlider, extraPosition.x, extraPosition.y, NULL); <-- Old way to do it (example)
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_extra, { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::UPLEFT);

			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);

			
		} break;

		case GuiControlState::NORMAL:
		{
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_extra, { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			color = { 1.0f,1.0f,1.0f,1.0f };
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);


		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_extra, { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			color = { 0.6f, 0.6f, 0.6f, 1.0f };
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);


		} break;
		case GuiControlState::PRESSED:
		{
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_extra, { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::UPLEFT);

			color = { 0.1f, 0.1f, 0.1f, 1.0f };
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);


		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_extra, { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			color = { 0.6f, 0.6f, 0.6f, 1.0f };
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);


		}break;
		default:
			break;
		}

		return false;
	}
}
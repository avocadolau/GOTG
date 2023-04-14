#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiCheckbox.h"
#include <Wiwa/audio/Audio.h>
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiCheckbox::GuiCheckbox(Scene* scene, unsigned int id, Rect2i bounds,const char* path, size_t callbackID, Rect2i boundsOriginTex, const char* audioEventName, bool active, bool animated, std::vector<Rect2i> animationRects) : GuiControl(scene, GuiControlType::CHECKBOX, id)
	{
		this->position = bounds;
		texturePosition = boundsOriginTex;
		name = "Checkbox";
		m_Scene = scene;
		text = "none";
		this->active = active;
		audioEventForButton = audioEventName;
		
		if (path != "") {
			textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
			texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);
		}

		this->callbackID = callbackID;
		if (callbackID != WI_INVALID_INDEX)
			callback = Wiwa::Application::Get().getCallbackAt(callbackID);
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, texturePosition, Wiwa::Renderer2D::Pivot::UPLEFT);
		if (!active)
		{
			r2d.DisableInstance(m_Scene, id_quad_normal);

		}
		canClick = true;

		framesAnimation = 0;
		animatedControl = animated;
		positionsForAnimations = animationRects;
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
			}

			if (state == GuiControlState::FOCUSED)
			{
				/*if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = GuiControlState::PRESSED;
					clicked = true;
				}

				if (Wiwa::Input::IsMouseButtonReleased(0) && clicked)
				{
					checked = !checked;
					clicked = false;
					void* params[] = { &checked };
					callback->Execute(params);
				}*/
				if (Wiwa::Input::IsButtonPressed(0, 0))
				{
					state = GuiControlState::PRESSED;
					clicked = true;
				}
				if (clicked && Wiwa::Input::IsButtonReleased(0,0))
				{
					clicked = false;
					checked = !checked;
					if (Audio::FindEvent(audioEventForButton.c_str()) != Audio::INVALID_ID)
					{
						Audio::PostEvent(audioEventForButton.c_str());
					}
					void* params[] = { &checked };
					if (callback)
						callback->Execute(params);
				}
				
				
			}
			if (checked)
			{
				SetNextFrame(1, &r2d);
			}
			else if(!checked)
			{
				SetNextFrame(0, &r2d);
			}

		}

		
		
		return false;
	}

	bool GuiCheckbox::Draw(Renderer2D* render)
	{
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();
		Color4f color = { 1.0f,1.0f,1.0f,1.0f };

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
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;


		case GuiControlState::NORMAL:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			color = { 1.0f,1.0f,1.0f,1.0f };
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			color = { 0.6f, 0.6f, 0.6f, 1.0f };

			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;
		case GuiControlState::PRESSED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			color = { 0.1f, 0.1f, 0.1f, 1.0f };

			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			Vector2i newPosition;
			newPosition.x = this->position.x;
			newPosition.y = this->position.y;
			Size2i newSize;
			newSize.w = this->position.width;
			newSize.h = this->position.height;
			color = { 0.6f, 0.6f, 0.6f, 1.0f };

			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, newPosition, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;
		default:
			break;
		}
		//HandleAnim(render);
		return false;
	}
}
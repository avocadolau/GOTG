#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiButton.h"
#include <Wiwa/utilities/Log.h>
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/audio/Audio.h>

namespace Wiwa
{
	GuiButton::GuiButton(Scene* scene, unsigned int id, Rect2i bounds,const char* path, const char* extraPath, size_t callbackID, Rect2i boundsOriginTex, const char* audioEventName, bool active,bool animated, float animFrames, std::vector<Rect2i> animationRects) : GuiControl(scene, GuiControlType::BUTTON, id)
	{
		this->position = bounds;
		texturePosition = boundsOriginTex;
		name = "Button";
		m_Scene = scene;
		this->active = active;
		text = "none";
		audioEventForButton = audioEventName;
		
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
		if (!active)
		{
			r2d.DisableInstance(m_Scene, id_quad_normal);

		}
		
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
			
			float mouseX, mouseY;
			mouseX = Wiwa::Input::GetMouseX();
			mouseY = Wiwa::Input::GetMouseY();
			if ((mouseX > position.x && mouseX < (position.x + position.width)) &&
				(mouseY > position.y && mouseY < position.y + position.height))
			{
				if (state == GuiControlState::NORMAL)
				{
					//Audio::PostEvent("on_hover");
				}

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
					clicked = false;
					Audio::PostEvent("character_selection");
					if (callback)
						callback->Execute();
				}*/
				if(Wiwa::Input::IsButtonPressed(0, 0))
				{
					state = GuiControlState::PRESSED;
					clicked = true;
				} 
				if (clicked && Wiwa::Input::IsButtonReleased(0,0))
				{
					clicked = false;
					if (Audio::FindEvent(audioEventForButton.c_str()) != Audio::INVALID_ID)
					{
						Audio::PostEvent(audioEventForButton.c_str());
					}
					if (callback)
						callback->Execute();
				}
			}
		}
		return false;
	}

	bool GuiButton::Draw(Renderer2D* render)
	{
		Color4f color = { 1.0f,1.0f,1.0f,1.0f };
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;

		case GuiControlState::NORMAL:
		{
			color = { 1.0f,1.0f,1.0f,1.0f };
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			color = { 0.6f, 0.6f, 0.6f, 1.0f };
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;
		case GuiControlState::PRESSED:
		{
			color = { 0.1f, 0.1f, 0.1f, 1.0f };
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			color = { 0.6f, 0.6f, 0.6f, 1.0f };
			render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render->UpdateInstancedQuadTexColor(m_Scene, id_quad_normal, color);
		} break;
		default:
			break;
		}
		HandleAnim(render);
		return false;
	}
}
#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiSlider.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiSlider::GuiSlider(unsigned int id, Rect2i bounds, Rect2i sliderBounds, const char* path, const char* slider_path) : GuiControl(GuiControlType::SLIDER, id)
	{
		this->position = bounds;
		this->extraPosition = sliderBounds;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		ResourceId imgid = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgid);
		r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);

		ResourceId imgid2 = Wiwa::Resources::Load<Wiwa::Image>(slider_path);
		textureForSlider = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgid2);
		r2d.CreateInstancedQuadTex(textureForSlider->GetTextureId(), textureForSlider->GetSize(), { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, Wiwa::Renderer2D::Pivot::CENTER);


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
					//extraBounds.x = mouseX;
					if (Wiwa::Input::IsMouseButtonPressed(0))
					{
						state = GuiControlState::PRESSED;
						extraPosition.x = extraPosition.x;
						//cout << "Pressed " << endl;
						//NotifyObserver();
					}
					else if (Wiwa::Input::IsMouseButtonPressed(0))
					{
						state = GuiControlState::SELECTED;
						//cout << "Selected " << endl;
						extraPosition.x = mouseX;
						/*NotifyObserver();*/
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
		Vector2i positionForUpdate_1;
		Vector2i positionForUpdate_2;

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			//render->DrawTexture2(textureForSlider, extraPosition.x, extraPosition.y, NULL); <-- Old way to do it (example)

			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(textureForSlider->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		case GuiControlState::NORMAL:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(textureForSlider->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(textureForSlider->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;
		case GuiControlState::PRESSED:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(textureForSlider->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(textureForSlider->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		}break;
		default:
			break;
		}

		return false;
	}
}
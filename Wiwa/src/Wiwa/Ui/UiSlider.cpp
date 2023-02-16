#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiSlider.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiSlider::GuiSlider(unsigned int id, Rect2i bounds, Rect2i sliderBounds, Image* texture, Image* sliderTexture) : GuiControl(GuiControlType::SLIDER, id)
	{
		this->position = bounds;
		this->extraPosition = sliderBounds;
		this->texture = texture;
		this->textureForSlider = sliderTexture;
		//canClick = true;
	}

	GuiSlider::~GuiSlider()
	{

	}

	bool GuiSlider::Update(float dt)
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
		Wiwa::Renderer2D& r2d_2 = Wiwa::Application::Get().GetRenderer2D();
		Vector2i positionForUpdate_1;
		Vector2i positionForUpdate_2;

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			//render->DrawTexture2(textureForSlider, extraPosition.x, extraPosition.y, NULL); <-- Old way to do it (example)

			r2d_1.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), position, texture->GetSize(), color, section, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			r2d_2.CreateInstancedQuadTex(textureForSlider->GetTextureId(), textureForSlider->GetSize(), extraPosition, textureForSlider->GetSize(), color, extraSection, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		case GuiControlState::NORMAL:
		{
			r2d_1.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), position, texture->GetSize(), color, section, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			r2d_2.CreateInstancedQuadTex(textureForSlider->GetTextureId(), textureForSlider->GetSize(), extraPosition, textureForSlider->GetSize(), color, extraSection, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			r2d_1.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), position, texture->GetSize(), color, section, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			r2d_2.CreateInstancedQuadTex(textureForSlider->GetTextureId(), textureForSlider->GetSize(), extraPosition, textureForSlider->GetSize(), color, extraSection, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;
		case GuiControlState::PRESSED:
		{
			r2d_1.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), position, texture->GetSize(), color, section, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			r2d_2.CreateInstancedQuadTex(textureForSlider->GetTextureId(), textureForSlider->GetSize(), extraPosition, textureForSlider->GetSize(), color, extraSection, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			r2d_1.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), position, texture->GetSize(), color, section, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_1.x = position.x;
			positionForUpdate_1.y = position.y;
			positionForUpdate_1.w = position.width;
			positionForUpdate_1.h = position.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

			r2d_2.CreateInstancedQuadTex(textureForSlider->GetTextureId(), textureForSlider->GetSize(), extraPosition, textureForSlider->GetSize(), color, extraSection, Wiwa::Renderer2D::Pivot::UPLEFT);
			positionForUpdate_2.x = extraPosition.x;
			positionForUpdate_2.y = extraPosition.y;
			positionForUpdate_2.w = extraPosition.width;
			positionForUpdate_2.h = extraPosition.height;
			r2d_1.UpdateInstancedQuadTex(texture->GetTextureId(), positionForUpdate_2, Wiwa::Renderer2D::Pivot::UPLEFT);
		}break;
		default:
			break;
		}

		return false;
	}
}
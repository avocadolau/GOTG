#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiSlider.h"


namespace Wiwa
{
	GuiSlider::GuiSlider(unsigned int id, Rect2i bounds, Rect2i sliderBounds, Image* texture, Image* sliderTexture) : GuiControl(GuiControlType::SLIDER, id)
	{
		this->bounds = bounds;
		this->extraBounds = sliderBounds;
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

				if ((mouseX > bounds.x && mouseX < (bounds.x + bounds.width)) &&
					(mouseY > bounds.y && mouseY < bounds.y + bounds.height))
				{
					state = GuiControlState::FOCUSED;
					//extraBounds.x = mouseX;
					if (Wiwa::Input::IsMouseButtonPressed(0))
					{
						state = GuiControlState::PRESSED;
						extraBounds.x = extraBounds.x;
						//cout << "Pressed " << endl;
						//NotifyObserver();
					}
					else if (Wiwa::Input::IsMouseButtonPressed(0))
					{
						state = GuiControlState::SELECTED;
						//cout << "Selected " << endl;
						extraBounds.x = mouseX;
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
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL);
			render->DrawTexture2(textureForSlider, extraBounds.x, extraBounds.y, NULL);
		} break;

		case GuiControlState::NORMAL:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL);
			render->DrawTexture2(textureForSlider, extraBounds.x, extraBounds.y, NULL);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL);
			render->DrawTexture2(textureForSlider, extraBounds.x, extraBounds.y, NULL);
		} break;
		case GuiControlState::PRESSED:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL);
			render->DrawTexture2(textureForSlider, extraBounds.x, extraBounds.y, NULL);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL);
			render->DrawTexture2(textureForSlider, extraBounds.x, extraBounds.y, NULL);
		}break;
		default:
			break;
		}

		return false;
	}
}
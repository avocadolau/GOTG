#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiCheckbox.h"

namespace Wiwa
{
	GuiCheckbox::GuiCheckbox(unsigned int id, Rect2i bounds) : GuiControl(GuiControlType::BUTTON, id)
	{
		this->bounds = bounds;

		canClick = true;
	}

	GuiCheckbox::~GuiCheckbox()
	{

	}

	bool GuiCheckbox::Update(float dt)
	{
		if (state != GuiControlState::DISABLED)
		{
			// L14: TODO 3_D: Update the state of the GUiButton according to the mouse position
			float mouseX, mouseY;
			mouseX = Wiwa::Input::GetMouseX();
			mouseY = Wiwa::Input::GetMouseY();

			if ((mouseX > bounds.x && mouseX < (bounds.x + bounds.width)) &&
				(mouseY > bounds.y && mouseY < bounds.y + bounds.height))
			{
				state = GuiControlState::FOCUSED;

				if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = GuiControlState::PRESSED;
					//cout << "Pressed " << endl;
					//NotifyObserver();
				}
				else if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = GuiControlState::SELECTED;
					//cout << "Selected " << endl;
					//NotifyObserver();
				}
				else
				{
					state = GuiControlState::NORMAL;
				}
			}

		}

		return false;
	}

	bool GuiCheckbox::Draw(Renderer2D* render)
	{

		// Draw the right button depending on state
		switch (state)
		{

		case GuiControlState::DISABLED:
		{

			render->CreateInstancedQuadTex(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;

		case GuiControlState::NORMAL:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;
		case GuiControlState::PRESSED:
		{

			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			render->DrawTexture2(texture, bounds.x, bounds.y, NULL); //<--Usar esto
		}break;
		default:
			break;
		}

		return false;
	}
}
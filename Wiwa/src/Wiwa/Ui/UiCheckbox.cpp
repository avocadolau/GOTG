#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiCheckbox.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	GuiCheckbox::GuiCheckbox(unsigned int id, Rect2i bounds,const char* path, const char* extraPath) : GuiControl(GuiControlType::BUTTON, id)
	{
		this->position = bounds;

		imgid_nonChecked = Wiwa::Resources::Load<Wiwa::Image>(path);
		imgid2_checked = Wiwa::Resources::Load<Wiwa::Image>(extraPath);

		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgid_nonChecked);
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		//We create only once the texture
		id_quad = r2d.CreateInstancedQuadTex(texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		
		canClick = true;
	}

	GuiCheckbox::~GuiCheckbox()
	{

	}

	bool GuiCheckbox::Update()
	{
		if (state != GuiControlState::DISABLED)
		{
			// L14: TODO 3_D: Update the state of the GUiButton according to the mouse position
			float mouseX, mouseY;
			mouseX = Wiwa::Input::GetMouseX();
			mouseY = Wiwa::Input::GetMouseY();

			if (checked)
			{
				texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgid_nonChecked);
			}
			else
			{
				texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgid2_checked);
			}

			if ((mouseX > position.x && mouseX < (position.x + position.width)) &&
				(mouseY > position.y && mouseY < position.y + position.height))
			{
				state = GuiControlState::FOCUSED;

				if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = GuiControlState::PRESSED;
					//cout << "Pressed " << endl;
					//NotifyObserver();
					checked = !checked;
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
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture2(texture, position.x, position.y, NULL); <-- Old way to do it (example)
			
			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		case GuiControlState::NORMAL:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;
		case GuiControlState::PRESSED:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
		} break;

		/******/

		case GuiControlState::SELECTED:
		{
			r2d_1.UpdateInstancedQuadTex(id_quad, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
		}break;
		default:
			break;
		}

		return false;
	}
}
#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiCanvas.h"
#include "Wiwa/ui/UiManager.h"
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
namespace Wiwa
{
	GuiCanvas::GuiCanvas(Scene* scene, unsigned int id,bool active)
	{
		this->id = id;
		this->active = active;
		selected = false;
	}

	GuiCanvas::~GuiCanvas()
	{

	}

	bool GuiCanvas::Update()
	{
		InputController();

		return true;
	}

	void GuiCanvas::SwapActive()
	{
		active = !active;
		for (size_t i = 0; i < controls.size(); i++)
		{
			controls.at(i)->SwapActive(Wiwa::Application::Get().GetRenderer2D());
		}
	}

	void GuiCanvas::SelectElement(size_t id)
	{
		for (size_t i = 0; i < controlsForSelection.size(); i++)
		{
			if (i == id)
			{
				controlsForSelection.at(i)->state = Wiwa::GuiControlState::FOCUSED;
			}
			else
			{
				controlsForSelection.at(i)->state = Wiwa::GuiControlState::NORMAL;
			}
		}
		
	}

	void GuiCanvas::InputController()
	{

		if (Wiwa::Input::IsButtonPressed(0, 13))
		{
			DpadUp = true;
		}
		if (Wiwa::Input::IsButtonPressed(0, 11))
		{
			DpadDown = true;
		}
		if (Wiwa::Input::IsButtonReleased(0, 13) && DpadUp)
		{
			DpadUp = false;
			idGuiSelected++;

			if (idGuiSelected >= controlsForSelection.size())
			{
				idGuiSelected = 0;
			}
		}
		if (Wiwa::Input::IsButtonReleased(0, 11) && DpadDown)
		{
			DpadDown = false;
			idGuiSelected--;
			if (idGuiSelected <= -1)
			{
				idGuiSelected = controlsForSelection.size() - 1;
			}
		}
		if (idGuiSelected > -1 && idGuiSelected < controlsForSelection.size())
		{
			SelectElement(idGuiSelected);

		}


	}
}
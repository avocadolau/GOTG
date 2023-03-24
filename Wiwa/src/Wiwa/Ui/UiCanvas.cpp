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

	void GuiCanvas::SwapActive()
	{
		active = !active;
		for (size_t i = 0; i < controls.size(); i++)
		{
			this->controls.at(i)->SwapActive(Wiwa::Application::Get().GetRenderer2D());
		}
	}

	void GuiCanvas::SelectElement(size_t id)
	{
		for (size_t i = 0; i < controls.size(); i++)
		{
			controls.at(i)->state = Wiwa::GuiControlState::NORMAL;
		}
		controls.at(id)->state = Wiwa::GuiControlState::FOCUSED;
	}
}
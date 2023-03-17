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
	}
}
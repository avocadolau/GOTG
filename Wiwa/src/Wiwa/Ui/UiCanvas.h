#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>


namespace Wiwa
{
	class WI_API GuiCanvas
	{
	public:

		GuiCanvas(Scene* scene, unsigned int id,bool active);
		 ~GuiCanvas();

		 std::vector<GuiControl*> controls;
		 std::vector<GuiControl*> controlsToDestroy;


		 void SwapActive();
		 bool active;

		 int id;
	};
}
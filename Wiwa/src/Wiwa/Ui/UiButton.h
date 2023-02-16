#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>


namespace Wiwa
{
	class WI_API GuiButton
	{
	public:

		GuiButton(unsigned int id, Rect2i bounds, const char* text, Image* texture);
		virtual ~GuiButton();

		bool Update(float dt);
		bool Draw(Renderer2D* render);

		bool canClick = true;
	};
}


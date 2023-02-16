#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>

namespace Wiwa
{
	class WI_API GuiCheckbox : public GuiControl
	{
	public:

		GuiCheckbox(unsigned int id, Rect2i bounds);
		virtual ~GuiCheckbox();

		bool Update(float dt);
		bool Draw(Renderer2D* render);

		bool canClick = true;
	};
}
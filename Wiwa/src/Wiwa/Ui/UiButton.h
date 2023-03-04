#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>


namespace Wiwa
{
	class WI_API GuiButton : public Wiwa::GuiControl
	{
	public:

		GuiButton(Scene* scene, unsigned int id, Rect2i bounds, const char* path, const char* extraPath);
		virtual ~GuiButton();

		bool Update();
		bool Draw(Renderer2D* render);

		bool SwapTexture();

		bool canClick = true;

	};
}


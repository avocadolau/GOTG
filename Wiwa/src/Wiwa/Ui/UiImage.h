#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>


namespace Wiwa
{
	class WI_API GuiImage : public Wiwa::GuiControl
	{
	public:

		GuiImage(Scene* scene, unsigned int id, Rect2i bounds, const char* path, size_t callbackID, Rect2i boundsOriginTex);
		virtual ~GuiImage();

		bool Update();
		bool Draw(Renderer2D* render);

		bool canClick = true;

		
	};
}

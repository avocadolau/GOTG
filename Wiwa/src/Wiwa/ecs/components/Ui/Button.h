#pragma once

#include <Wiwa.h>
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	struct WI_API Button
	{
		bool canClick;

		ResourceId elementImageid;
		Size2i elementImageSize;
		Vector2i elementPosition;
		Rect2i elementImageSection;

		int state = 1; // 1 == NORMAL
	};
}


REFLECTION_BEGIN(Wiwa::Button)
	REFLECT_MEMBER(canClick)
	REFLECT_MEMBER(elementImageid)
	REFLECT_MEMBER(elementImageSize)
	REFLECT_MEMBER(elementPosition)
	REFLECT_MEMBER(elementImageSection)
	REFLECT_MEMBER(state)
REFLECTION_END;
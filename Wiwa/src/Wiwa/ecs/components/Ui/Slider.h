#pragma once

#include <Wiwa.h>
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Renderer2D.h>

namespace Wiwa
{
	struct WI_API Slider
	{
		bool canClick;
		ResourceId elementImageid;
		Size2i elementImageSize;
		Vector2i elementPosition;
		Rect2i elementImageSection;

		ResourceId movingElementImageid;
		Size2i movingElementImageSize;
		Vector2i movingElementPosition;
		Rect2i movingElementImageSection;

		int SliderLevel;
		int state = 1; // 1 == NORMAL

	};
}


REFLECTION_BEGIN(Wiwa::Slider)
	REFLECT_MEMBER(canClick)
	REFLECT_MEMBER(elementImageid)
	REFLECT_MEMBER(elementImageSize)
	REFLECT_MEMBER(elementPosition)
	REFLECT_MEMBER(elementImageSection)
	REFLECT_MEMBER(movingElementImageid)
	REFLECT_MEMBER(movingElementImageSize)
	REFLECT_MEMBER(movingElementPosition)
	REFLECT_MEMBER(movingElementImageSection)
	REFLECT_MEMBER(SliderLevel)
	REFLECT_MEMBER(state)
REFLECTION_END;
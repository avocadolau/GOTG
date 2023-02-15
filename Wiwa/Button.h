#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {
	struct WI_API Button {
		ButtonState button_state;
		Rect2i bounds;
		ResourceId button_resource;
		char button_text[32]; //STILL NEED TO IMPLEMENT TEXT
		bool active;
	};

	enum ButtonState {
		DEFAULT,
		HOVER,
		CLICKED,
	};
}

REFLECTION_BEGIN(Wiwa::Button)
REFLECT_MEMBER(button_state)
REFLECT_MEMBER(bounds)
REFLECT_MEMBER(button_text)
REFLECT_MEMBER(active)
REFLECTION_END;
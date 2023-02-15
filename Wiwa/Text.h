#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/math/Math.h>

namespace Wiwa {
	struct WI_API Text {
		Rect2i bounds;
		char text[32];
		bool active;
	};
}

REFLECTION_BEGIN(Wiwa::Text)
REFLECT_MEMBER(bounds)
REFLECT_MEMBER(text)
REFLECT_MEMBER(active)
REFLECTION_END;
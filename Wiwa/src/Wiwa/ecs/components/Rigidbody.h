#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa {
	struct WI_API Rigidbody {
		int x;
	};
}

REFLECTION_BEGIN(Wiwa::Rigidbody)
	REFLECT_MEMBER(x)
REFLECTION_END;
#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa {
	struct WI_API AgentAI {
		float speed;
	};
}

REFLECTION_BEGIN(Wiwa::AgentAI)
REFLECT_MEMBER(speed)
REFLECTION_END;
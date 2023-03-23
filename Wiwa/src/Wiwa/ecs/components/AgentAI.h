#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa {
	struct WI_API AgentAI {
		float speed;
		glm::vec3 target;
		//bool hasArrived;
	};
}

REFLECTION_BEGIN(Wiwa::AgentAI)
	REFLECT_MEMBER(speed)
	REFLECT_MEMBER(target)
REFLECTION_END;
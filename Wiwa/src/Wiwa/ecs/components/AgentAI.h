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
		bool hasArrived;
		bool hasPath;
		float angularSpeed;
	};
}

WI_REFLECTION_BEGIN(Wiwa::AgentAI)
	WI_REFLECT_MEMBER(speed)
	WI_REFLECT_MEMBER(target)
	WI_REFLECT_MEMBER(hasArrived)
	WI_REFLECT_MEMBER(hasPath)
	WI_REFLECT_MEMBER(angularSpeed)
WI_REFLECTION_END;
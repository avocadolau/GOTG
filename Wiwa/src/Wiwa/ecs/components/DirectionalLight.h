#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>


namespace Wiwa {
	struct WI_API DirectionalLight {
		glm::vec3 Color;
		float AmbientIntensity;
		float DiffuseIntensity;
		glm::vec3 Direction;
	};
}

REFLECTION_BEGIN(Wiwa::DirectionalLight)
	REFLECT_MEMBER(Color)
	REFLECT_MEMBER(AmbientIntensity)
	REFLECT_MEMBER(DiffuseIntensity)
	REFLECT_MEMBER(Direction)
REFLECTION_END;

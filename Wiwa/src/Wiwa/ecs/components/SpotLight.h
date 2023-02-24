#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>


namespace Wiwa {
	struct WI_API SpotLight {
		glm::vec3 Color;
		float AmbientIntensity;
		float DiffuseIntensity;
		float Constant;
		float Linear;
		float Exp;
		glm::vec3 Direction;
		float Cutoff;
	};
}

REFLECTION_BEGIN(Wiwa::SpotLight)
	REFLECT_MEMBER(Color)
	REFLECT_MEMBER(AmbientIntensity)
	REFLECT_MEMBER(DiffuseIntensity)
	REFLECT_MEMBER(Constant)
	REFLECT_MEMBER(Linear)
	REFLECT_MEMBER(Exp)
	REFLECT_MEMBER(Direction)
	REFLECT_MEMBER(Cutoff)
REFLECTION_END;
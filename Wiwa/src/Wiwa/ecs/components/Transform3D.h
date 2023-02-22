#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>

#include <glm/mat4x4.hpp>

namespace Wiwa {
	struct WI_API Transform3D {
		// Read-only world position
		glm::vec3 position;
		// Read-only world rotation
		glm::vec3 rotation;
		// Read-only world scale
		glm::vec3 scale;

		// Local position
		glm::vec3 localPosition;
		// Local rotation
		glm::vec3 localRotation;
		// Local scale
		glm::vec3 localScale;

		// World matrix
		glm::mat4 worldMatrix;

		// Local matrix
		glm::mat4 localMatrix;
	};
}

REFLECTION_BEGIN(Wiwa::Transform3D)
	REFLECT_MEMBER(position)
	REFLECT_MEMBER(rotation)
	REFLECT_MEMBER(scale)
	REFLECT_MEMBER(localPosition)
	REFLECT_MEMBER(localRotation)
	REFLECT_MEMBER(localScale)
	REFLECT_MEMBER(worldMatrix)
REFLECTION_END;
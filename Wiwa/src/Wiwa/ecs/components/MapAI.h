#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>
#define MAP_TILES_MAX_SIZE 5000

namespace Wiwa {
	struct WI_API MapAI {
		glm::vec3 startingPosition;
		int width;
		int	height;
		float tileWidth; // Cannot be less than 1!
		float tileHeight;// Cannot be less than 1!
		unsigned char map[MAP_TILES_MAX_SIZE];
	};
}

REFLECTION_BEGIN(Wiwa::MapAI)
REFLECT_MEMBER(startingPosition)
REFLECT_MEMBER(width)
REFLECT_MEMBER(height)
REFLECT_MEMBER(tileWidth)
REFLECT_MEMBER(tileHeight)
REFLECT_MEMBER(map)
REFLECTION_END;
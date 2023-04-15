#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API AINavMesh
	{
		char filePath[128];
	};
}

REFLECTION_BEGIN(Wiwa::AINavMesh)
REFLECT_MEMBER(filePath)
REFLECTION_END;
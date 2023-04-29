#pragma once
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa
{
	struct VentRotatorData
	{
		float Max;
		float Min;
		float Velocity;
	};
}

REFLECTION_BEGIN(Wiwa::VentRotatorData)
	REFLECT_MEMBER(Max)
	REFLECT_MEMBER(Min)
	REFLECT_MEMBER(Velocity)
REFLECTION_END;
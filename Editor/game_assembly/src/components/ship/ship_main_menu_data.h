#pragma once
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa
{
	struct ShipMainMenuData
	{
		float VelocityMove;
		float VelocityRot;
		float PanVelocity;
	};
}

REFLECTION_BEGIN(Wiwa::ShipMainMenuData)
	REFLECT_MEMBER(VelocityMove)
	REFLECT_MEMBER(VelocityRot)
	REFLECT_MEMBER(PanVelocity)
REFLECTION_END;

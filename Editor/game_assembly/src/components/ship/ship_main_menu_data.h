#pragma once
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa
{
	struct ShipMainMenuData
	{
		float VelocityMove;
		float VelocityRot;
		float PanVelocity;
		bool PanToCamera;
		float PanRotationVelocity;
	};
}

REFLECTION_BEGIN(Wiwa::ShipMainMenuData)
	REFLECT_MEMBER(VelocityMove)
	REFLECT_MEMBER(VelocityRot)
	REFLECT_MEMBER(PanVelocity)
	REFLECT_MEMBER(PanToCamera)
	REFLECT_MEMBER(PanRotationVelocity)
REFLECTION_END;

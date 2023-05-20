#pragma once
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa
{
	struct PlayerCameraData
	{
		float MaxHUBFOV;
		float MinHUBFOV;
		float HubMultiplier;

		float MaxCombatFOV;
		float MinCombatFOV;
		float CombatVelocity;
	};
}

REFLECTION_BEGIN(Wiwa::PlayerCameraData)
	REFLECT_MEMBER(MaxHUBFOV)
	REFLECT_MEMBER(MinHUBFOV)
	REFLECT_MEMBER(HubMultiplier)
	REFLECT_MEMBER(MaxCombatFOV)
	REFLECT_MEMBER(MinCombatFOV)
	REFLECT_MEMBER(CombatVelocity)
REFLECTION_END;
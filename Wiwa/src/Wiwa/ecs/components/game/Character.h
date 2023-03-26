#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Character {
		int MaxHealth;
		int Health;
		int MaxShield;
		int Shield;
		int Damage;
		
		float RateOfFire;
		float Speed;
		float DashDistance;
		float DashSpeed;
		float DashCooldown;
		float WalkTreshold;
	};
}

REFLECTION_BEGIN(Wiwa::Character)
	REFLECT_MEMBER(MaxHealth)
	REFLECT_MEMBER(Health)
	REFLECT_MEMBER(MaxShield)
	REFLECT_MEMBER(Shield)
	REFLECT_MEMBER(Damage)
	REFLECT_MEMBER(RateOfFire)
	REFLECT_MEMBER(DashDistance)
	REFLECT_MEMBER(DashSpeed)
	REFLECT_MEMBER(DashCooldown)
	REFLECT_MEMBER(WalkTreshold)
REFLECTION_END;
#pragma once

#include <string>

#include <Wiwa/core/Resources.h>

namespace Wiwa
{
	enum class ConsumableType : uint16_t
	{
		HEAL,
		SHIELD
	};

	struct Ability
	{
		std::string Name;
		std::string Description;

		ResourceId Icon;

		int Damage;
		float Range;
		float Area;
		float Cooldown;

		int Price;

		Ability() = default;
		Ability(const char* name)
			: Name(name),
			  Icon(0),
			  Damage(0),
			  Range(0.f),
			  Area(0.f),
			  Cooldown(0.f),
			  Price(0)
		{}
	};

	struct PassiveSkill
	{
		std::string Name;
		std::string Description;

		ResourceId Icon;


		PassiveSkill() = default;
		PassiveSkill(const char* name)
			: Name(name), Icon(0) {}
	};

	struct Buff
	{
		std::string Name;
		std::string Description;

		ResourceId Icon;

		int BuffPercent;
		float Duration;
		float Cooldown;
		int Price;

		Buff() = default;
		Buff(const char* name)
			: Name(name),
			  Icon(0),
			  BuffPercent(0),
			  Duration(0.f),
			  Cooldown(0.f),
			  Price(0)
		{}
	};

	struct Consumable
	{
		std::string Name;

		ResourceId Icon;

		ConsumableType Type;

		int BuffPercent;

		Consumable() = default;
		Consumable(const char* name)
			: Name(name),
			  Type(ConsumableType::HEAL),
			  Icon(0),
			  BuffPercent(0)
		{}
	};
}
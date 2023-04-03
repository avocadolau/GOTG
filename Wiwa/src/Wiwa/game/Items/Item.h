#pragma once

#include <string>

namespace Wiwa
{
	struct Ability
	{
		std::string Name;
		std::string Description;

		Ability() = default;
		Ability(const char* name)
			: Name(name) {}
	};

	struct PassiveSkill
	{
		std::string Name;
		std::string Description;

		PassiveSkill() = default;
		PassiveSkill(const char* name)
			: Name(name) {}
	};

	struct Buff
	{
		std::string Name;
		std::string Description;

		Buff() = default;
		Buff(const char* name)
			: Name(name) {}
	};

	struct Consumable
	{
		std::string Name;
		std::string Description;

		Consumable() = default;
		Consumable(const char* name)
			: Name(name) {}
	};
}
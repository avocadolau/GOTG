#pragma once

#include <string>

#include <Wiwa/core/Resources.h>

namespace Wiwa
{
	enum class ConsumableType
	{
		HEAL,
		SHIELD 
	};
	enum class AbilityType
	{
		YONDUS_SEEDS = BIT(0),
		GROOTS_SEEDS = BIT(1),
		PHYLAS_QUANTUM_SWORD = BIT(2),
		STARHAWKS_BLAST = BIT(3)
	};
	enum class BuffType
	{
		MAJOR_VICTORY_SHIELD = BIT(0),
		NIKKIS_TOUCH = BIT(1),
		COSMOS_PAW = BIT(2),
		MARTINEX_THERMOKINESIS = BIT(3),
		BUGS_LEGS = BIT(4),
		CHARLIE27_FIST = BIT(5)
	};
	// Defines which attributes will be modified via the passive
	enum class PassiveType
	{
		MOVEMENT = BIT(0),
		ROF = BIT(1),
		BUFF = BIT(2),
		ATTACK = BIT(3)
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
		float CurrentTime;
		int Price;

		AbilityType abilityType;

		Ability() = default;
		Ability(const char* name)
			: Name(name),
			  abilityType(AbilityType::YONDUS_SEEDS),
			  Icon(0),
			  Damage(0),
			  Range(0.f),
			  Area(0.f),
			  Cooldown(0.f),
			  Price(0),
			  CurrentTime(0.f)
		{}

		void Use();
	};


	struct PassiveSkill
	{
		std::string Name;
		std::string Description;

		ResourceId Icon;

		PassiveType passiveType;

		PassiveSkill() = default;
		PassiveSkill(const PassiveSkill& passive)
		{
			this->Name = passive.Name;
			this->Description = passive.Description;
			this->Icon = passive.Icon;
			this->passiveType = passive.passiveType;
		}
		PassiveSkill(const char* name)
			: Name(name),
			  passiveType(PassiveType::ATTACK),
		      Icon(0)
		{}

		void Use();
	};

	struct Buff
	{
		std::string Name;
		std::string Description;

		ResourceId Icon;
		
		BuffType buffType;

		int BuffPercent;
		float Duration;
		float Cooldown;
		float CurrentTime;
		float CoolDownTimer;
		int Price;

		Buff() = default;
		Buff(const char* name)
			: Name(name),
			  Icon(0),
			  buffType(BuffType::MAJOR_VICTORY_SHIELD),
			  BuffPercent(0),
			  Duration(0.f),
			  Cooldown(0.f),
			  Price(0),
		      CurrentTime(0.f),
              CoolDownTimer(0.f)
		{}

		void Use();
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
			  Icon(0),
			  Type(ConsumableType::HEAL),
			  BuffPercent(0)
		{}

		void Use();
	};
}
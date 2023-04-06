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
		YONDUS_SEEDS,
		GROOTS_SEEDS,
		PHYLAS_QUANTUM_SWORD,
		STARHAWKS_BLAST
	};
	enum class BuffType
	{
		MAJOR_VICTORY_SHIELD,
		NIKKIS_TOUCH,
		COSMOS_PAW,
		MARTINEX_THERMOKINESIS,
		BUGS_LEGS,
		CHARLIE27_FIST
	};
	// Defines which attributes will be modified via the passive
	enum class PassiveType
	{
		MOVEMENT,
		ROF,
		BUFF,
		ATTACK
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
		Ability(const Ability& ability)
		{
			this->Name = ability.Name;
			this->Description = ability.Description;
			this->Icon = ability.Icon;
			this->Damage = ability.Damage;
			this->Range = ability.Range;
			this->Cooldown = ability.Cooldown;
			this->CurrentTime = ability.CurrentTime;
			this->Price = ability.Price;
			
		}
		
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
		Buff(const Buff& buff)
		{
			this->Name = buff.Name;
			this->Icon = buff.Icon;
			this->buffType = buff.buffType;
			this->BuffPercent = buff.BuffPercent;
			this->Duration = buff.Duration;
			this->Cooldown = buff.Cooldown;
			this->Price = buff.Price;
			this->CurrentTime = buff.CurrentTime;
			this->CoolDownTimer = buff.CoolDownTimer;
		}
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
		Consumable(const Consumable& consumable)
		{
			this->Name = consumable.Name;
			this->Icon = consumable.Icon;
			this->Type = consumable.Type;
			this->BuffPercent = consumable.BuffPercent;
		}
		Consumable(const char* name)
			: Name(name),
			  Icon(0),
			  Type(ConsumableType::HEAL),
			  BuffPercent(0)
		{}

		void Use();
	};
}
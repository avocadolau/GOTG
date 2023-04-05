#pragma once
#include <Wiwa/core/Core.h>

#include "Item.h"

#include <map>

namespace Wiwa
{
	class JSONDocument;
	//This class let's the game know which items are available during the run
	class WI_API ItemManager
	{
	private:
		ItemManager() = delete;
		~ItemManager() = delete;
	public:

		static void AddAbility(const Ability& ability);
		static void DeleteAbility(const std::string& name);

		static void AddPassive(const PassiveSkill& passive);
		static void DeletePassive(const std::string& name);

		static void AddBuff(const Buff& buff);
		static void DeleteBuff(const std::string& name);

		static void AddConsumable(const Consumable& consumable);
		static void DeleteConsumable(const std::string& name);

		static void Serialize(JSONDocument* doc);
		static void Deserialize(JSONDocument* doc);

		static Ability* GetAbility(const char* name);
		static PassiveSkill* GetPassive(const char* name);
		static Buff* GetBuff(const char* name);
		static Consumable* GetConsumable(const char* name);

		WI_HARD_INL static std::map<std::string, Ability>& GetAbilities() { return m_AbilityPool; }
		WI_HARD_INL static std::map<std::string, PassiveSkill>& GetSkills() { return m_PassiveSkillPool; }
		WI_HARD_INL static std::map<std::string, Buff>& GetBuffs() { return m_BuffPool; }
		WI_HARD_INL static std::map<std::string, Consumable>& GetConsumables() { return m_ConsumablePool; }

	private:
		static std::map<std::string, Ability> m_AbilityPool;
		static std::map<std::string, PassiveSkill> m_PassiveSkillPool;
		static std::map<std::string, Buff> m_BuffPool;
		static std::map<std::string, Consumable> m_ConsumablePool;
	};
}
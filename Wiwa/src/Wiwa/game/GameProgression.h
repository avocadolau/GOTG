#pragma once

#include <Wiwa/core/Core.h>

#include <vector>

namespace Wiwa
{
	struct GameData
	{
		int m_EnemiesKilled;
		int m_KilledUltronCount;
		int m_ItemsBought;
	};

	class JSONDocument;
	class WI_API GameProgression
	{
	protected:
		GameProgression();
		~GameProgression();
	public:
		void Serialize(JSONDocument* doc);
		void Deserialize(JSONDocument* doc);

		//Functions for incrementation of data
		void IncrementEnemiesKilled() { m_PlayerData.m_EnemiesKilled++; }
		void IncrementUltronKills() { m_PlayerData.m_KilledUltronCount++; }
		void IncrementItemsBought() { m_PlayerData.m_ItemsBought++; }

		void InitGame();
		void Update();
		void Clear();
	private:
		GameData m_PlayerData;
		friend class GameStateManager;
	};
}
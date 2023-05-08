#pragma once
#include "BossUltronBase.h"

namespace Wiwa {
	class BossUltronSecondPhaseState : public BossUltronBaseState {
		enum class SecondPhaseState 
		{
			MOVE_CENTER,
			REGENERATE,
			END_STATE
		};

	public:
		BossUltronSecondPhaseState();
		~BossUltronSecondPhaseState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

		void FillPremadePositionAfterRegen(BossUltron* enemy, std::vector<glm::vec3>& vec);
		glm::vec3 GetNewPositionAfterRegen();

	private: 
		SecondPhaseState m_SecondPhaseState;

		std::vector<glm::vec3> m_AfterRegenPosition;

		bool m_SpawnEnemies = false;
		float m_TimerSecondPhase = 0.0f;
		float m_TimerHealing = 0.0f;
		const char* m_WallPrefabPath;
		const char* m_EnemySpawnerPath;
	};
}
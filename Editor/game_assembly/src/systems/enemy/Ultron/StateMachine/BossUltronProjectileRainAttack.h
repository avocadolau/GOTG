#pragma once
#include "BossUltronBase.h"

#define RAIN_RANGE 3.0f
#define TIME_BETWEEN_PROJECTILES 2.0f

namespace Wiwa {
	class BossUltronProjectileRainAttackState : public BossUltronBaseState {
		enum class ProjectileRainState {
			//MOVE_CENTER,
			PREPARE_RAIN,
			RAIN_ATTACK,
			END_STATE,
		};
	public:
		BossUltronProjectileRainAttackState();
		~BossUltronProjectileRainAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		//void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;
		//void FillPremadePositionAfterRain(BossUltron* enemy, std::vector<glm::vec3>& vec);
		//glm::vec3 GetNewPositionAfterLaser();

	private:

		/*bool SpawnProjectileRain(BossUltron* enemy, const glm::vec3& bull_dir);*/

		glm::vec3 GetRandomPositionInRange(const glm::vec3& position, float range);

		void SpawnThunderStorm(BossUltron* enemy, glm::vec3 thunderPosition, const glm::vec3& bull_dir);

		void SetupThunderMark(BossUltron* enemy, int id, glm::vec3 pos, const char* path);

		glm::vec3 RandomPointInHexagon();

		ProjectileRainState m_RainState;
		std::vector<glm::vec3> m_AfterRainPosition;

		float m_TimerRain;
		float m_TimerThundersSpawn = 0.0f;
		float m_TimerThundersMark = 0.0f;
		float m_TimerThundersMarkDelete = 0.0f;

		bool m_RandomPositionSetted = false;
		int m_RainProjectileCounter;

		//temporal
		EntityId m_ThunderMarkId1;


		const char* m_ThunderMarkPath;

		//temporal
		glm::vec3 m_ThunderPos1 = glm::vec3(0.0f, 0.0f, 0.0f);

	};
}

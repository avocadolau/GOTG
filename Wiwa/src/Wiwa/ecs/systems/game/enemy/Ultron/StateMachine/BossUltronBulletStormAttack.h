#pragma once
#include "BossUltronBase.h"

#define NUMBER_OF_ROUNDS 3

namespace Wiwa {
	class BossUltronBulletStormAttackState : public BossUltronBaseState {
	public:
		BossUltronBulletStormAttackState();
		~BossUltronBulletStormAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

		//glm::vec3 CalculateForward(const Transform3D& t3d);
	private:

		void SpawnBullet(BossUltron* enemy, const glm::vec3& bull_dir);

		void SpawnFirstPattern(BossUltron* enemy);
		void SpawnSecondPattern(BossUltron* enemy);
		void SpawnThirdPattern(BossUltron* enemy);

	

		float m_TimerRoundCooldown = 0.0f;
		int m_RoundCounter = 0;
	};
}

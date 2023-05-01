#pragma once
#include "BossUltronBase.h"

#define NUMBER_OF_ROUNDS 3
#define TIMER_ATTACK 2.0f

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

		void SpawnBullet(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir);

		void SpawnFirstPattern(BossUltron* enemy);
		void SpawnSecondPattern(BossUltron* enemy);
		void SpawnThirdPattern(BossUltron* enemy);	

		void SelectRandomAttack(BossUltron* enemy);

		bool IsSecondPatternFinished();
		bool IsThirdPatternFinished();

		float m_TimerRoundCooldown = 0.0f;
		int m_RoundCounter = 0;
		int m_SelectRandomAttack = 0;
		bool m_IsAttackSelected = false;

		//Second Pattern Attack
		float m_SecondPatternAttackTimer = 0.0f;
		bool m_SecondPatternEnabled = false;
		float m_SecondPatternBulletcounter = 0.0f;
		int m_SecondPatternCounter = 0;

		//Third Pattern Attack
		float m_ThirdPatternAttackTimer = 0.0f;
		bool m_ThirdPatternEnabled = false;
		float m_ThirdPatternBulletcounter = 0.0f;
		int m_ThirdPatternCounter = 0;
	};
}

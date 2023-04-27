#pragma once
#include "BossUltronBase.h"

namespace Wiwa {
	class BossUltronLaserBeamAttackState : public BossUltronBaseState {
	public:
		BossUltronLaserBeamAttackState();
		~BossUltronLaserBeamAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

	private:

		EntityId SpawnLaserBeam(BossUltron* enemy, const glm::vec3& bull_dir);

		glm::vec3 CalculateForward(const Transform3D& t3d);

		float m_Timer;
		bool shootLaser;

		glm::vec3 m_LaserAttackPosition = { 0.f,0.f,0.f };
	};
}

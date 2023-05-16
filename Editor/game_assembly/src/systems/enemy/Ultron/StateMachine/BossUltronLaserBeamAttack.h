#pragma once
#include "BossUltronBase.h"

namespace Wiwa {
	class BossUltronLaserBeamAttackState : public BossUltronBaseState {
		enum class LaserState {
			MOVE_CENTER,
			PREPARE_LASER,
			LASER_ATTACK,
			END_STATE,
		};
	public:
		BossUltronLaserBeamAttackState();
		~BossUltronLaserBeamAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;
		void FillPremadePositionAfterLaser(BossUltron* enemy, std::vector<glm::vec3>& vec);
		glm::vec3 GetNewPositionAfterLaser();
		
	private:

		EntityId SpawnLaserBeam(BossUltron* enemy, const glm::vec3& bull_dir);

		glm::vec3 CalculateForward(const Transform3D& t3d);

		LaserState laserState;
		std::vector<glm::vec3> m_AfterLaserBeamPosition;
		float m_TimerLaser;
	};
}

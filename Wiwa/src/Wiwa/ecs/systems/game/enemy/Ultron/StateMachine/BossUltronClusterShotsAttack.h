#pragma once
#include "BossUltronBase.h"
//#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

#define CLUSTER_BULLET_LIFETIME 5.0f

namespace Wiwa {
	class BossUltronClusterShotsAttackState : public BossUltronBaseState {
	public:
		BossUltronClusterShotsAttackState();
		~BossUltronClusterShotsAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;		

	private:

		EntityId* SpawnClusterBullet(BossUltron* enemy, const glm::vec3& bull_dir);

		glm::vec3 CalculateForward(const Transform3D& t3d);

		float m_TimerBetweenBullet = 0.0f;
		
	};
}

#pragma once
#include "BossUltronBase.h"
//#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

namespace Wiwa {
	class BossUltronClusterShotsAttackState : public BossUltronBaseState {
	public:
		BossUltronClusterShotsAttackState();
		~BossUltronClusterShotsAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

		void SpawnBullet(BossUltron* enemy, const glm::vec3& bull_dir, EntityId clusterBullet);
		EntityId SpawnClusterBullet(BossUltron* enemy, const glm::vec3& bull_dir);

		void BlowClusterBullet(EntityId bulletId, BossUltron* enemy);

	private:

		float m_TimerClusterBullet01Lifetime = -1.0f;
		float m_TimerClusterBullet02Lifetime = -1.0f;
		
		EntityId m_ClusterBullet01Id;
		EntityId m_ClusterBullet02Id;
	};
}

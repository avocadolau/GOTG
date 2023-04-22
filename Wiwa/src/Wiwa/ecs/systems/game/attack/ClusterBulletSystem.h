#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

//#include "Wiwa/ecs/components/game/wave/Wave.h"

//#define CLUSTER_BULLET_LIFETIME 5.0f
//#define CLUSTER_BULLET_DAMAGE 30

namespace Wiwa {
	class WI_API ClusterBulletSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_BulletIt;
	public:
		ClusterBulletSystem();
		~ClusterBulletSystem();

		void OnAwake() override;

		void OnInit() override;

		void InitClusterBullet();

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void SpawnBullet(const glm::vec3& bull_dir, EntityId clusterBullet);
		void BlowClusterBullet01(EntityId bulletId);
		void BlowClusterBullet02(EntityId bulletId);

		bool OnEnabledFromPool();
		bool OnDisabledFromPool();
	};
}

REGISTER_SYSTEM(Wiwa::ClusterBulletSystem);
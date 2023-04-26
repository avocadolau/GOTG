#pragma once
#include "../../System.h"
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API EnemySystem : public System {
	public:
		EntityManager::ComponentIterator m_EnemyIt;
		EntityManager::ComponentIterator m_StatsIt;
		EntityManager::ComponentIterator m_ColliderIt;
		EntityManager::ComponentIterator m_AgentIt;
		EntityManager::ComponentIterator m_NavAgentIt;
		EntityManager::ComponentIterator m_TransformIt;

		EntityManager::ComponentIterator m_PlayerTransformIt;
		EntityManager::ComponentIterator m_PlayerStatsIt;
		SystemHash m_PhysicsSystemHash;

		EntityId m_PlayerId;
		EntityId m_WaveId = -1;
		bool m_WasSpawnedBySpawner = false;

		float previousSpeed = 0.0f;

		float timerSlow = 0.0f;
	public:
		EnemySystem();
		~EnemySystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		virtual void ReceiveDamage(int damage);

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;

		void LookAt(const glm::vec3& target_look);

		void RotateTo(const glm::vec3& target);
	};
}

REGISTER_SYSTEM(Wiwa::EnemySystem);
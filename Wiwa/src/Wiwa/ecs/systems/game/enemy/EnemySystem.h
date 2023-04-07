#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>
#include "Wiwa/ecs/components/game/wave/WaveSpawner.h"
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/AgentAISystem.h>
#include "Wiwa/scripting/embed_functions/embed_audio_functions.h"
#include "Wiwa/scripting/ScriptEngine.h"
#include "Wiwa/AI/AIMapGeneration.h"

namespace Wiwa {
	class WI_API EnemySystem : public System {
	public:
		EntityManager::ComponentIterator m_EnemyIt;
		EntityManager::ComponentIterator m_StatsIt;
		EntityManager::ComponentIterator m_ColliderIt;
		EntityManager::ComponentIterator m_AgentIt;
		EntityManager::ComponentIterator m_TransformIt;

		EntityManager::ComponentIterator m_PlayerTransformIt;
		EntityManager::ComponentIterator m_PlayerStatsIt;
		EntityId m_PlayerId;
		bool m_WasSpawnedBySpawner = false;
	public:
		EnemySystem();
		~EnemySystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		virtual void ReceiveDamage(int damage);

		void ChasePlayer();

		bool GoToPosition(glm::vec3 targetedPosition);

		void RotateTo(const glm::vec3& target);
		//void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::EnemySystem);
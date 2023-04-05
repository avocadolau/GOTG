#include <wipch.h>
#include "EnemySystem.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/systems/AgentAISystem.h>

namespace Wiwa
{
	struct BulletComponent
	{
		float Velocity;
		float LifeTime;
		int Damage;
		glm::vec3 Direction;
	};

	Wiwa::EnemySystem::EnemySystem()
	{
		m_EnemyIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_StatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_ColliderIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AgentIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		m_PlayerTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerStatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerId = -1;
	}

	EnemySystem::~EnemySystem()
	{
	}

	void EnemySystem::OnAwake()
	{
	}

	void EnemySystem::OnInit()
	{
		m_EnemyIt = GetComponentIterator<Enemy>();
		m_StatsIt = GetComponentIterator<Character>();
		m_ColliderIt = GetComponentIterator<CollisionBody>();
		m_AgentIt = GetComponentIterator<AgentAI>();
		m_TransformIt = GetComponentIterator<Transform3D>();

		m_PlayerId = Wiwa::GameStateManager::s_PlayerId;
		m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
		m_PlayerStatsIt = GetComponentIterator<Character>(m_PlayerId);

		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);
		transform->localPosition.y = 0.0f;

		self->currentRotation = transform->localRotation;
	}

	void EnemySystem::OnUpdate()
	{
		if (Input::IsKeyPressed(Wiwa::Key::M))
		{
			Character* stats = GetComponentByIterator<Character>(m_StatsIt);
			stats->Health = 0;
		}
	}

	void EnemySystem::OnDestroy()
	{
	}

	void EnemySystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId && strcmp(body2->selfTagStr,"BULLET") == 0)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			BulletComponent* bullet = em.GetComponent<BulletComponent>(body2->id);
			ReceiveDamage(bullet->Damage);
		}
	}

	void EnemySystem::ReceiveDamage(int damage)
	{
		Character* statsSelf = GetComponentByIterator<Character>(m_StatsIt);
		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);

		statsSelf->Health = statsSelf->Health - damage;
		if (statsSelf->Health <= 0)
		{
			self->hasFinished = true;
		}
	}

	void EnemySystem::ChasePlayer()
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		Transform3D* playerTr = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);
		Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(m_EntityId);

		if (agentPtr)
		{
			agentPtr->CreatePath(playerTr->localPosition);
		}
		//RotateTo(playerTr.Position,  enemy, entityId);                   
	}

	void EnemySystem::RotateTo(const glm::vec3& target)
	{
		float angle = glm::degrees(std::atan2(target.x, target.z));
		
		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);

		self->currentRotation.y = Wiwa::Math::LerpAngle(self->currentRotation.y, angle, 10);

		if (self->currentRotation.y >= 360.0f)
			self->currentRotation.y = 0.0f;

		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);

		transform->localRotation.y = self->currentRotation.y;
	}
}
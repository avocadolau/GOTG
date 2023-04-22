#include <wipch.h>
#include "SentinelAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "Wiwa/ecs/components/game/attack/SentinelExplosion.h"

namespace Wiwa
{
	SentinelAttackState::SentinelAttackState()
	{
		m_TimerExplosion = 0.0f;
		prepareAttack = false;
	}

	SentinelAttackState::~SentinelAttackState()
	{

	}

	void SentinelAttackState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
	}

	void SentinelAttackState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		float dist2Player = glm::distance(selfTr->localPosition, playerTr->localPosition);
		int distPath = aiSystem->GetPathSize();
		
		aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });
		
		if (dist2Player >= enemy->m_RangeOfExplosion)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}

		if (dist2Player < enemy->m_RangeOfExplosion)
		{
			stats->Speed = stats->Speed * 0.5f;
			prepareAttack = true;
		}

		if (prepareAttack)
		{
		
			float dt = Time::GetDeltaTime() * 0.001;

			m_TimerExplosion += dt;

			if (m_TimerExplosion >= 4.0f)
			{
				enemy->SwitchState(enemy->m_DeathState);
			}
		}
	}

	void SentinelAttackState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelAttackState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}
}
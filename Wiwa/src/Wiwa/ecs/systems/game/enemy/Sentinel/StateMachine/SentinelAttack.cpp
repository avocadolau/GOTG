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

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		
		if (glm::distance(selfTr->localPosition, playerTr->localPosition) > enemy->m_RangeOfExplosion)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}

		if (glm::distance(selfTr->localPosition, playerTr->localPosition) < enemy->m_RangeOfExplosion)
		{
			prepareAttack = true;
		}

		if (prepareAttack)
		{
		
			float dt = Time::GetDeltaTime() * 0.001;

			m_TimerExplosion += dt;

			if (m_TimerExplosion >= 3.0f)
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
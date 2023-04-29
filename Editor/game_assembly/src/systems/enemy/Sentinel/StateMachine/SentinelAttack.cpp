#include <wipch.h>
#include "SentinelAttack.h"
#include "../EnemySentinel.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "../../../attack/SentinelExplosionSystem.h"

#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/Character.h>

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

		animator->PlayAnimation("atackdeath", false);
	}

	void SentinelAttackState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		/*if (glm::distance(selfTr->localPosition, playerTr->localPosition) < 2.0f)
		{
			navAgent->stoppingDistance = 1.0f;
			navAgentPtr->StopAgent();
		}*/
		
		//if (glm::distance(selfTr->localPosition, playerTr->localPosition) > enemy->m_RangeOfExplosion)
		//{
		//	/*if (prepareAttack == false)
		//	{
		//		enemy->SwitchState(enemy->m_ChasingState);
		//	}*/

		//	enemy->SwitchState(enemy->m_ChasingState);
		//}

		//if (prepareAttack == false)
		//{
		//	if (glm::distance(selfTr->localPosition, playerTr->localPosition) <= enemy->m_RangeOfExplosion)
		//	{
		//		prepareAttack = true;
		//	}
		//}
		//
		//if (prepareAttack)
		//{
		//
		//	float dt = Time::GetDeltaTimeSeconds();

		//	m_TimerExplosion += dt;

		//	if (m_TimerExplosion >= 3.0f)
		//	{
		//		enemy->SwitchState(enemy->m_DeathState);
		//	}
		//}

		//-----------------------------------------------

		if (glm::distance(selfTr->localPosition, playerTr->localPosition) <= 1.0f)
		{
			enemy->SwitchState(enemy->m_DeathState);
		}

		/*animator->PlayAnimation("atackdeath", false);*/

		float dt = Time::GetDeltaTimeSeconds();

		m_TimerExplosion += dt;

		if (m_TimerExplosion >= 3.0f)
		{
			enemy->SwitchState(enemy->m_DeathState);
		}

		//-------------------------------------------------

		/*if (glm::distance(selfTr->localPosition, playerTr->localPosition) <= enemy->m_RangeOfExplosion)
		{
			float dt = Time::GetDeltaTimeSeconds();

			m_TimerExplosion += dt;

			if (m_TimerExplosion >= 3.0f)
			{
				enemy->SwitchState(enemy->m_DeathState);
			}
		}
		if (glm::distance(selfTr->localPosition, playerTr->localPosition) <= enemy->m_RangeOfExplosion)
		{
			float dt = Time::GetDeltaTimeSeconds();

			m_TimerExplosion += dt;

			if (m_TimerExplosion >= 3.0f)
			{
				enemy->SwitchState(enemy->m_DeathState);
			}
		}*/
		//-----------------------------
		//if (glm::distance(selfTr->localPosition, playerTr->localPosition) < enemy->m_RangeOfExplosion)
		//{
		//	/*navAgentPtr->StopAgent();*/

		//	animator->PlayAnimation("atackdeath", false);

		//	if (animator->HasFinished())
		//	{
		//		enemy->SwitchState(enemy->m_DeathState);
		//	}
		//}

	}

	void SentinelAttackState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelAttackState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{
		/*std::string playerStr = "PLAYER";
		if (body1->id == enemy->GetEntity() && playerStr == body2->selfTagStr)
		{
			WI_INFO("COLLISION SENTINEL");
			enemy->SwitchState(enemy->m_DeathState);
		}*/
	}
}
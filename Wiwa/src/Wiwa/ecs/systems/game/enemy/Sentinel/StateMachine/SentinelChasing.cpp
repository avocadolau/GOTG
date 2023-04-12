#include <wipch.h>
#include "SentinelChasing.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>

namespace Wiwa
{
	SentinelChasingState::SentinelChasingState()
	{

	}

	SentinelChasingState::~SentinelChasingState()
	{

	}

	void SentinelChasingState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		animator->Blend("walk", true, 0.2f);

		enemy->m_Timer = 0;
		enemy->ChasePlayer();
	}

	void SentinelChasingState::UpdateState(EnemySentinel* enemy)
	{
		m_ChasingTimer += Time::GetDeltaTime();

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);

		if (m_ChasingTimer > 1000.0f && distance <= 50.0f)
		{
			m_ChasingTimer = 0.0f;
			enemy->ChasePlayer();
		}

		if (animator->HasFinished())//&& !IsPlaying(enemy->GetEntity())
		{
			PlaySound(ScriptEngine::CreateString("melee_moving"), enemy->m_PlayerId);
		}

		if (agentPtr)
		{
			agentPtr->AllowRotationByTile();
			if (glm::distance(selfTr->localPosition, playerTr->localPosition) < 4.0f)
			{
				agentPtr->DisableRotationByTile();
				enemy->SwitchState(enemy->m_AttackingState);
			}
		}

	}

	void SentinelChasingState::ExitState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
	}

	void SentinelChasingState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}
}
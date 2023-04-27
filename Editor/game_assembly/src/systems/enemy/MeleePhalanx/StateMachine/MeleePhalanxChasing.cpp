#include <wipch.h>
#include "MeleePhalanxChasing.h"
#include "../EnemyMeleePhalanx.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	MeleePhalanxChasingState::MeleePhalanxChasingState()
	{

	}

	MeleePhalanxChasingState::~MeleePhalanxChasingState()
	{

	}

	void MeleePhalanxChasingState::EnterState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		animator->Blend("walk",true,0.2f);

		enemy->m_Timer = 0;
	}
	
	void MeleePhalanxChasingState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		m_ChasingTimer += Time::GetDeltaTime();

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);

		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		if (agent)
		{
			Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			agent->SetDestination(playerTr->localPosition);
		}
		
		if (animator->HasFinished() )//&& !IsPlaying(enemy->GetEntity())
		{
			
		}


		if (glm::distance(selfTr->localPosition, playerTr->localPosition) < 4.0f)
		{
			enemy->SwitchState(enemy->m_AttackingState);
		}
	}
	
	void MeleePhalanxChasingState::ExitState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//animator->Blend("idle", true, true, 0.5f);
		//animator->Pause();
	}
	
	void MeleePhalanxChasingState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}
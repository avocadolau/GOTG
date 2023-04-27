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

		float distance = glm::distance(selfTr->localPosition, playerTr->localPosition);

		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		if (agent)
		{
			Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

			glm::vec3 offset = { 1.0f,0.0f,1.0f };
			offset.x = playerTr->localPosition.x + 1.1f;
			offset.z = playerTr->localPosition.z + 1.1f;

			agent->SetDestination(offset);
		}
		
		if (animator->HasFinished() )//&& !IsPlaying(enemy->GetEntity())
		{
			
		}

		
		if (distance < 4.0f && distance > 1.0f)
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
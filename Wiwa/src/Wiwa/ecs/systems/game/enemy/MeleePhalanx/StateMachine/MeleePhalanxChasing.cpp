#include <wipch.h>
#include "MeleePhalanxChasing.h"
#include <Wiwa/ecs/systems/game/enemy/MeleePhalanx/EnemyMeleePhalanx.h>

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

		animator->Blend("run",true,0.2f);

		//PlaySound(ScriptEngine::CreateString("melee_moving"), enemy->m_PlayerId);
		enemy->m_Timer = 0;
	}
	
	void MeleePhalanxChasingState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		m_ChasingTimer += Time::GetDeltaTime();

		if (m_ChasingTimer > 100.0f)
		{
			m_ChasingTimer = 0.0f;
			enemy->ChasePlayer();

		}

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());

		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		
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
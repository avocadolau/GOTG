#include <wipch.h>
#include "MeleePhalanxAttack.h"
#include "../EnemyMeleePhalanx.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/Character.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/audio/Audio.h>

namespace Wiwa
{
	MeleePhalanxAttackState::MeleePhalanxAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	MeleePhalanxAttackState::~MeleePhalanxAttackState()
	{
	}

	void MeleePhalanxAttackState::EnterState(EnemyMeleePhalanx *enemy)
	{
		enemy->m_Timer = 0;
		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem *animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		m_TimerAttackCooldown = 0.0f;

		animator->Blend("atack", false, 0.2f);

		NavAgent *navAgent = (NavAgent *)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent)
		{
			navAgent->autoRotate = false;
		}

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();

		GenerateAttack(enemy);
	}

	void MeleePhalanxAttackState::UpdateState(EnemyMeleePhalanx *enemy)
	{
		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem *animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Transform3D *playerTr = (Transform3D *)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D *selfTr = (Transform3D *)em.GetComponentByIterator(enemy->m_TransformIt);

		enemy->LookAt(playerTr->localPosition);

		m_TimerAttackCooldown += Time::GetDeltaTime(); // This is in milliseconds

		if (glm::distance(selfTr->localPosition, playerTr->localPosition) > 3.0f) // animation->HasFinished()
		{
			m_TimerAttackCooldown = 0.0f;
			enemy->SwitchState(enemy->m_ChasingState);
		}
		else if (m_TimerAttackCooldown > 2000.0f)
		{
			GenerateAttack(enemy);
			// Reset the timer after generating the attack
			m_TimerAttackCooldown = 0.0f;
		}
	}

	void MeleePhalanxAttackState::ExitState(EnemyMeleePhalanx *enemy)
	{
		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();
		NavAgent *navAgent = (NavAgent *)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent)
		{
			navAgent->autoRotate = true;
		}
	}

	void MeleePhalanxAttackState::OnCollisionEnter(EnemyMeleePhalanx *enemy, const Object *body1, const Object *body2)
	{
	}

	void MeleePhalanxAttackState::GenerateAttack(EnemyMeleePhalanx *enemy)
	{

		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();

		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());

		Transform3D *playerTr = (Transform3D *)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D *selfTr = (Transform3D *)em.GetComponentByIterator(enemy->m_TransformIt);

		Character *selfStats = (Character *)em.GetComponentByIterator(enemy->m_StatsIt);

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);
		if (selfStats != nullptr)
		{
			if (distance <= 3.0f)
			{
				animator->Blend("atack", false, 0.2f);
				GameStateManager::DamagePlayer(selfStats->Damage);
				EntityId pe_hurt = em.GetChildByName(enemy->m_PlayerId, "PE_Hurt");
				audio->PlayAudio("melee_attack");
			}
		}
	}
}
#include <wipch.h>
#include "MeleePhalanxAttack.h"
#include "../EnemyMeleePhalanx.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
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
		Wiwa::OzzAnimationSystem*animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		m_TimerAttackCooldown = 0.0f;

		NavAgent *navAgent = (NavAgent *)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent)
		{
			navAgent->autoRotate = false;
		}

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();
		m_SoundCurrentTime = 0.0f;
		//GenerateAttack(enemy);
	}

	void MeleePhalanxAttackState::UpdateState(EnemyMeleePhalanx *enemy)
	{
		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem*animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		Transform3D *playerTr = (Transform3D *)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D *selfTr = (Transform3D *)em.GetComponentByIterator(enemy->m_TransformIt);
		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);

		enemy->LookAt(playerTr->localPosition, 30.f);
		float distance = glm::distance(selfTr->localPosition, playerTr->localPosition);

		if (distance > 4.0f && !m_PlaySound) // animation->HasFinished()
		{
			m_TimerAttackCooldown = 0.0f;
			enemy->SwitchState(enemy->m_ChasingState);
		}
		else if (m_TimerAttackCooldown > 1.0f / stats->rateOfFire)
		{
			GenerateAttack(enemy);
			// Reset the timer after generating the attack
			m_TimerAttackCooldown = 0.0f;
		}
		m_SoundCurrentTime += Time::GetDeltaTimeSeconds();
		//WI_INFO("m_SoundCurrentTime: {}", m_SoundCurrentTime);
		//WI_INFO("m_PlaySound: {}", m_PlaySound);

		if (m_SoundCurrentTime >= m_SoundTimer && m_PlaySound)
		{	
			//WI_INFO("attack!!!!!!!!!!!!!!!!!!!: {}", m_PlaySound);
			// make damage here when the animation is hitting
			EnemyData* selfStats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);
			if (selfStats != nullptr)
			{
				//WI_INFO("Melee damage: {}", selfStats->damage);
				animator->PlayAnimation("walk");
				animator->PlayAnimation("atack");
				GameStateManager::DamagePlayer(selfStats->damage);
			}
			
			audio->PlayAudio("melee_attack");
			m_PlaySound = false;
			m_SoundCurrentTime = 0.0f;
		}

		m_TimerAttackCooldown += Time::GetDeltaTimeSeconds(); // This is in milliseconds
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
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());

		Transform3D *playerTr = (Transform3D *)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D *selfTr = (Transform3D *)em.GetComponentByIterator(enemy->m_TransformIt);

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);
		if (distance <= 3.0f)
		{
			animator->PlayAnimation("walk");
			animator->PlayAnimation("atack");			
	
			EntityId pe_hurt = em.GetChildByName(enemy->m_PlayerId, "PE_Hurt");
			m_SoundCurrentTime = m_SoundTimer;
			m_PlaySound = true;
		}
	}
}
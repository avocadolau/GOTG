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
		m_Enemy = nullptr;
	}

	MeleePhalanxAttackState::~MeleePhalanxAttackState()
	{
	}

	void MeleePhalanxAttackState::EnterState(EnemyMeleePhalanx *enemy)
	{
		m_Enemy = enemy;

		enemy->m_Timer = 0;
		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();

		NavAgent *navAgent = (NavAgent *)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent)
		{
			navAgent->autoRotate = false;
		}

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();
		m_SoundCurrentTime = 0.0f;

		Wiwa::OzzAnimationSystem* animatorSys = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::OzzAnimator* animator = animatorSys->getAnimator();
		OzzAnimation* animation = animator->getAnimationByName("atack");
		animation->addKeyAction({ &MeleePhalanxAttackState::GenerateAttack, this }, 0.6f);

		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);
		animator->PlayAnimation("atack");
		animator->getActiveAnimation()->setPlaybackSpeed(stats->rateOfFire);
	}

	void MeleePhalanxAttackState::UpdateState(EnemyMeleePhalanx *enemy)
	{
		Wiwa::EntityManager &em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		Transform3D *playerTr = (Transform3D *)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D *selfTr = (Transform3D *)em.GetComponentByIterator(enemy->m_TransformIt);
		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);

		enemy->LookAt(playerTr->localPosition, 30.f);
		float distance = glm::distance(selfTr->localPosition, playerTr->localPosition);
		bool hasFinished = animator->getAnimator()->getActiveAnimation()->HasFinished();

		if (distance > 4.0f && hasFinished)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
		else if (hasFinished)
		{
			animator->PlayAnimation("atack");
			animator->getAnimator()->getActiveAnimation()->setPlaybackSpeed(stats->rateOfFire);
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

		m_Enemy = nullptr;

		Wiwa::OzzAnimationSystem* animatorSys = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::OzzAnimator* animator = animatorSys->getAnimator();
		OzzAnimation* animation = animator->getAnimationByName("atack");
		animation->removeKeyAction({ &MeleePhalanxAttackState::GenerateAttack, this });
	}

	void MeleePhalanxAttackState::OnCollisionEnter(EnemyMeleePhalanx *enemy, const Object *body1, const Object *body2)
	{
	}

	void MeleePhalanxAttackState::GenerateAttack()
	{
		if (m_Enemy == nullptr)
			return;

		Wiwa::EntityManager &em = m_Enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(m_Enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(m_Enemy->GetEntity());

		Transform3D *playerTr = (Transform3D *)em.GetComponentByIterator(m_Enemy->m_PlayerTransformIt);
		Transform3D *selfTr = (Transform3D *)em.GetComponentByIterator(m_Enemy->m_TransformIt);
		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(m_Enemy->m_StatsIt);

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);
		if (stats != nullptr && distance <= 3.0f)
		{
			GameStateManager::DamagePlayer(stats->damage);
			audio->PlayAudio("melee_attack");
		}
	}
}
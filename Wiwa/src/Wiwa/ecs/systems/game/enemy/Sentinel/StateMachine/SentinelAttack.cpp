#include <wipch.h>
#include "SentinelAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>

namespace Wiwa
{
	SentinelAttackState::SentinelAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	SentinelAttackState::~SentinelAttackState()
	{

	}

	void SentinelAttackState::EnterState(EnemySentinel* enemy)
	{
		enemy->m_Timer = 0;
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		m_TimerAttackCooldown = 0.0f;
		//animator->PlayAnimation("atack", false);
		animator->Blend("atack", false, 0.2f);
		PlaySound(ScriptEngine::CreateString("melee_attack"), enemy->m_PlayerId);

		GenerateAttack(enemy);
	}

	void SentinelAttackState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());

		m_TimerAttackCooldown += Time::GetDeltaTime(); // This is in milliseconds

		if (glm::distance(selfTr->localPosition, playerTr->localPosition) > 4.0f) // animation->HasFinished()
		{
			m_TimerAttackCooldown = 0.0f;
			enemy->SwitchState(enemy->m_ChasingState);
		}
		else if (m_TimerAttackCooldown > 2000.0f)
		{
			animator->Blend("atack", false, 0.2f);
			PlaySound(ScriptEngine::CreateString("melee_heavy_attack"), enemy->m_PlayerId);
			GenerateAttack(enemy);

			// Reset the timer after generating the attack
			m_TimerAttackCooldown = 0.0f;
		}
	
		aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });

	}

	void SentinelAttackState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelAttackState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}

	void SentinelAttackState::GenerateAttack(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::ParticleManager& pm = enemy->getScene().GetParticleManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		Character* selfStats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);

		ParticleManager& pman = enemy->getScene().GetParticleManager();

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);
		if (selfStats != nullptr)
		{
			if (distance <= 3.0f)
			{
				GameStateManager::DamagePlayer(selfStats->Damage);
				EntityId pe_hurt = em.GetChildByName(enemy->m_PlayerId, "PE_Hurt");
				pman.EmitBatch(pe_hurt);
			}
		}
	}
}
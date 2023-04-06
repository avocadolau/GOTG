#include <wipch.h>
#include "MeleePhalanxAttack.h"
#include <Wiwa/ecs/systems/game/enemy/MeleePhalanx/EnemyMeleePhalanx.h>

namespace Wiwa
{
	MeleePhalanxAttackState::MeleePhalanxAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	MeleePhalanxAttackState::~MeleePhalanxAttackState()
	{

	}

	void MeleePhalanxAttackState::EnterState(EnemyMeleePhalanx* enemy)
	{
		enemy->m_Timer = 0;
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		m_TimerAttackCooldown = 0.0f;
		//animator->PlayAnimation("atack", false);
		animator->Blend("atack", false, 0.2f);
		//PlaySound(ScriptEngine::CreateString("melee_heavy_attack"), enemy->m_PlayerId);
	
		GenerateAttack(enemy);
		//enemy->ChasePlayer();
	}
	
	void MeleePhalanxAttackState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
	
		/*if ()
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}*/
		
		//if (animator->HasFinished())
		//{
			m_TimerAttackCooldown += Time::GetDeltaTime(); // This is in milliseconds
			//WI_INFO(" Cooldown before: {}", m_TimerAttackCooldown);
			if (glm::distance(selfTr->localPosition, playerTr->localPosition) > 4.0f)
			{
				m_TimerAttackCooldown = 0.0f;
				enemy->SwitchState(enemy->m_ChasingState);
			}
			else if (m_TimerAttackCooldown > 2000.0f)
			{					
				animator->Blend("atack", false, 0.2f);
				//PlaySound(ScriptEngine::CreateString("melee_heavy_attack"), enemy->m_PlayerId);
				GenerateAttack(enemy);

				// Reset the timer after generating the attack
				m_TimerAttackCooldown = 0.0f;

				//WI_INFO(" Cooldown after: {}", m_TimerAttackCooldown);
			}
		//}

			//if (animator->HasFinished() && animator->GetCurrentAnimName() == "atack")
			//{
			//	GenerateAttack(enemy);
			//}

		aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });

		/*if (m_TimerAttackCooldown > 500.0f)
		{
			WI_INFO("Attack melee!");
			animator->PlayAnimation("idle", false);
			animator->PlayAnimation("atack", false);
			GenerateAttack(enemy);
			m_TimerAttackCooldown = 0.0f;
		}*/
		
	}
	
	void MeleePhalanxAttackState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}
	
	void MeleePhalanxAttackState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}

	void MeleePhalanxAttackState::GenerateAttack(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::ParticleManager& pm = enemy->getScene().GetParticleManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();


		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Character* playerStats = (Character*)em.GetComponentByIterator(enemy->m_PlayerStatsIt);
		Character* selfStats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);

		EntityId player = em.GetEntityByName("Player");

		float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);
		if (playerStats != nullptr && selfStats != nullptr)
		{
			if (distance <= 3.0f)
			{
				EntityId pe_hurt = em.GetChildByName(player,"PE_Hurt");
				pman.EmitBatch(player);
				playerStats->Health -= selfStats->Damage;
			}
		}		
	}
}
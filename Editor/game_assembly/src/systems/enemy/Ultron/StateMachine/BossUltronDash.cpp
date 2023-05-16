#include <wipch.h>
#include "BossUltronDash.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <cmath>
#include "../../../attack/UltronSmashExplosionSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include "../../../../components/attack/Explosion.h"

#define TIMER_GO_UPWARDS 0.01f
#define TIMER_GO_DOWNWARDS 0.01f

namespace Wiwa
{
	BossUltronDashState::BossUltronDashState()
	{
		m_TimerToRotate = 0.0f;
		m_TimerDash = 0.0f;
		lifetimeDash = 0.0f;
		damageDash = 0;
		m_MoveUpwardsCounter = 0.0f;
		initiateDash = false;
		playerDistance = glm::vec3(0.0f);
		interpolatedDashDistance = glm::vec3(0.0f);

		m_PreSmashMarkPath = "assets/vfx/prefabs/vfx_finals/Smash/p_boss_smash_01_marker.wiprefab";
	}

	BossUltronDashState::~BossUltronDashState()
	{

	}

	void BossUltronDashState::EnterState(BossUltron* enemy)
	{
		enemy->m_Timer = 0.0f;
		m_TimerToRotate = 0.0f;
		m_TimerDash = 0.0f;
		m_TimerToStopDash = 0.0f;
		m_TimerAfterDash = 0.0f;
		playerDistance = glm::vec3(0.0f);
		interpolatedDashDistance = glm::vec3(0.0f);
		m_MoveUpwardsCounter = 0.0f;
		m_State = DashState::DASH_INIT;
	}

	void BossUltronDashState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);

		switch (m_State)
		{
		case Wiwa::BossUltronDashState::DashState::DASH_INIT:
		{
			agent->StopAgent();
			navAgent->autoRotate = false;

			m_TimerToRotate += Time::GetDeltaTimeSeconds();

			playerDistance = playerTr->localPosition;

			enemy->LookAt(playerTr->localPosition, 80.0f);

			if (m_TimerToRotate >= 1.6f) //Timer to look at player
			{
				m_TimerToRotate = 0.0f;
				m_MoveUpwardsCounter = 0.0f;
				m_TimerToStopDash = 0.0f;
				m_UltronJump = false;
				agent->StopAgent();
				agent->RemoveAgent();

				playerDistance.x = playerDistance.x + 2.0f;
				playerDistance.z = playerDistance.z + 2.0f;

				m_State = DashState::DASH_GO_UP;
			}
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_GO_UP:
		{
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(em.GetComponentIterator<Transform3D>(enemy->GetEntity()));

			m_TimerDash += Time::GetDeltaTimeSeconds();
			m_TimerToStopDash += Time::GetDeltaTimeSeconds();

			if (m_TimerDash >= TIMER_GO_UPWARDS)
			{
				m_TimerDash = 0.0f;
				m_MoveUpwardsCounter = m_MoveUpwardsCounter + 1.0f; //Offset for the Smash
				selfTr->localPosition.y = m_MoveUpwardsCounter;
			}

			if (m_TimerToStopDash >= 1.2f) //Timer for the Ultron to go up
			{
				/*agent->RegisterWithCrowd();
				agent->SetPosition(playerDistance);
				agent->StopAgent();
				agent->RemoveAgent();*/

				selfTr->localPosition.x = playerDistance.x;
				selfTr->localPosition.z = playerDistance.z;

				m_PreSmashMarkId = em.LoadPrefab(m_PreSmashMarkPath);
				Transform3D* thunderMarkTr = em.GetComponent<Transform3D>(m_PreSmashMarkId);

				thunderMarkTr->localPosition.x = playerDistance.x;
				thunderMarkTr->localPosition.y = 0.1f;
				thunderMarkTr->localPosition.z = playerDistance.z;

				WI_INFO("INTERPOLATION DONE");
				m_State = DashState::DASH_GO_DOWN;
			}
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_GO_DOWN:
		{
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(em.GetComponentIterator<Transform3D>(enemy->GetEntity()));

			m_TimerDash += Time::GetDeltaTimeSeconds();
			
			if (m_TimerDash >= TIMER_GO_DOWNWARDS)
			{
				m_TimerDash = 0.0f;
				m_MoveUpwardsCounter = m_MoveUpwardsCounter - 1.0f;
				selfTr->localPosition.y = m_MoveUpwardsCounter;
			}

			if (selfTr->localPosition.y <= 0.05f)
			{
				WI_INFO("INTERPOLATION DONE");
				m_State = DashState::DASH_EXPLOSION;
			}
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_EXPLOSION:
		{
			em.DestroyEntity(m_PreSmashMarkId);

			agent->RegisterWithCrowd();
			agent->SetPosition(playerDistance);
			agent->StopAgent();

			Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

			SpawnExplosionAfterDash(enemy, selfTr);

			/*float center_x = 0.0f;
			float center_z = 0.0f;
			float radius = 8.0f;
			float angle_increment = 1.0f;
			float current_angle = 0.0f;
			int numberOfExplosion = 6;

			for (int i = 0; i < numberOfExplosion; i++)
			{
				float x_pos = center_x + radius * cos(current_angle);
				float z_pos = center_z + radius * sin(current_angle);

				SpawnExplosionAfterDash(enemy, selfTr, x_pos, z_pos);

				current_angle += angle_increment;
			}*/

			navAgent->autoRotate = true;

			m_State = DashState::DASH_FINISH;
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_FINISH:
		{
			m_TimerAfterDash += Time::GetDeltaTimeSeconds();

			if (m_TimerAfterDash >= 1.0f)
			{
				WI_INFO("Dash DONE");
				m_TimerAfterDash = 0.0f;
				enemy->SwitchState(enemy->m_MovementState);
			}
		}
		break;
		default:
			break;
		}
	}

	void BossUltronDashState::ExitState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
	}

	void BossUltronDashState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
		
	}

	void BossUltronDashState::SpawnExplosionAfterDash(BossUltron* enemy, Transform3D* selfTransform)
	{
		/*if (GameStateManager::s_PoolManager->s_ExplosiveBarrel->getCountDisabled() <= 0)
			return;*/

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newExplosionId = EntityManager::INVALID_INDEX;
		newExplosionId = GameStateManager::s_PoolManager->s_SmashExplosionPool->GetFromPool();

		if (newExplosionId == EntityManager::INVALID_INDEX)
			return;

		UltronSmashExplosionSystem* explosionSys = entityManager.GetSystem<UltronSmashExplosionSystem>(newExplosionId);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newExplosionId);
		physSys->DeleteBody();

		// Set intial positions
		//Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_TransformIt); //Transform
		Transform3D* explosiveBarrelTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newExplosionId));
		Explosion* explosion = (Explosion*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Explosion>(newExplosionId));
		explosion->isFromPool = true;

		if (!explosiveBarrelTr)
			return;

		explosiveBarrelTr->localPosition.x = Math::GetWorldPosition(selfTransform->worldMatrix).x;
		explosiveBarrelTr->localPosition.y = Math::GetWorldPosition(selfTransform->worldMatrix).y;
		explosiveBarrelTr->localPosition.z = Math::GetWorldPosition(selfTransform->worldMatrix).z;
		
		physSys->CreateBody();

		explosionSys->EnableExplosion();

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		EntityId p_explosion = em.LoadPrefab("assets/vfx/prefabs/vfx_finals/boss_Ultron/p_boss_smash.wiprefab");

		if (p_explosion != EntityManager::INVALID_INDEX)
		{
			Transform3D* p_exT = em.GetComponent<Transform3D>(p_explosion);
			if (p_exT != nullptr)
			{
				p_exT->localPosition = explosiveBarrelTr->localPosition;
			}
		}
	}
}
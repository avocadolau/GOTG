#include <wipch.h>
#include "BossUltronDash.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <cmath>
#include "../../../attack/UltronSmashExplosionSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
	BossUltronDashState::BossUltronDashState()
	{
		m_TimerToRotate = 0.0f;
		m_TimerDash = 0.0f;
		lifetimeDash = 0.0f;
		damageDash = 0;
		initiateDash = false;
		playerDistance = glm::vec3(0.0f);
		interpolatedDashDistance = glm::vec3(0.0f);
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
		m_State = DashState::DASH_INIT;
		/*animator->Blend("walk", true, 0.2f);*/
		/*enemy->m_DashTimer = 0;*/
	}

	void BossUltronDashState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);

		//switch (m_State)
		//{
		//case Wiwa::BossUltronDashState::DashState::DASH_INIT:
		//{
		//	agent->StopAgent();
		//	navAgent->autoRotate = false;

		//	m_TimerToRotate += Time::GetDeltaTimeSeconds();

		//	playerDistance = playerTr->localPosition;

		//	enemy->LookAt(playerTr->localPosition, 80.0f);
		//	
		//	if (m_TimerToRotate >= 2.0f)
		//	{
		//		m_TimerToRotate = 0.0f;
		//		interpolationValue = 0.0f;
		//		m_TimerToStopDash = 0.0f;
		//		m_State = DashState::DASH_PLAYING;
		//	}
		//}
		//	break;
		//case Wiwa::BossUltronDashState::DashState::DASH_PLAYING:
		//{
		//	m_TimerDash += Time::GetDeltaTimeSeconds();
		//	m_TimerToStopDash += Time::GetDeltaTimeSeconds();

		//	if (m_TimerDash >= 0.6f && interpolationValue < 0.95f)
		//	{
		//		m_TimerDash = 0.0f;
		//		interpolationValue = interpolationValue + 0.05;
		//	}

		//	interpolatedDashDistance = Math::InterpolateTwoV3(enemy->GetTransform()->localPosition, playerDistance, interpolationValue);

		//	agent->SetPosition(interpolatedDashDistance);

		//	if (m_TimerToStopDash >= 1.5f)
		//	{
		//		WI_INFO("INTERPOLATION DONE");
		//		m_State = DashState::DASH_STOP;
		//	}
		//}
		//	break;
		//case Wiwa::BossUltronDashState::DashState::DASH_STOP:
		//{
		//	Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//	Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		//	float center_x = 0.0f; // center x position of the circle
		//	float center_z = 0.0f; // center z position of the circle
		//	float radius = 8.0f; // radius of the circle
		//	float angle_increment = 1.0f; // angle increment between consecutive explosions
		//	float current_angle = 0.0f; // current angle
		//	int numberOfExplosion = 6;

		//	for (int i = 0; i < numberOfExplosion; i++) 
		//	{
		//		float x_pos = center_x + radius * cos(current_angle); // calculate x position of current explosion
		//		float z_pos = center_z + radius * sin(current_angle); // calculate z position of current explosion

		//		SpawnExplosionAfterDash(enemy, selfTr, x_pos, z_pos);

		//		current_angle += angle_increment; // increment angle for next explosion
		//	}

		//	/*SpawnExplosionAfterDash(enemy, selfTr);*/

		//	agent->StopAgent();
		//	navAgent->autoRotate = true;
		//
		//	m_State = DashState::DASH_COOLDOWN;
		//}
		//	break;
		//case Wiwa::BossUltronDashState::DashState::DASH_COOLDOWN:
		//{
		//	m_TimerAfterDash += Time::GetDeltaTimeSeconds();

		//	if (m_TimerAfterDash >= 2.0f)
		//	{
		//		WI_INFO("Dash DONE");
		//		m_TimerAfterDash = 0.0f;
		//		enemy->SwitchState(enemy->m_MovementState);
		//	}
		//}
		//	break;
		//default:
		//	break;
		//}

		//-----------------------------------------------------------------------------------------

		switch (m_State)
		{
		case Wiwa::BossUltronDashState::DashState::DASH_INIT:
		{
			agent->StopAgent();
			navAgent->autoRotate = false;

			m_TimerToRotate += Time::GetDeltaTimeSeconds();

			playerDistance = playerTr->localPosition;

			enemy->LookAt(playerTr->localPosition, 80.0f);

			if (m_TimerToRotate >= 2.0f)
			{
				m_TimerToRotate = 0.0f;
				interpolationValue = 0.0f;
				m_TimerToStopDash = 0.0f;
				m_UltronJump = false;
				m_State = DashState::DASH_PLAYING;
			}
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_PLAYING:
		{
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(em.GetComponentIterator<Transform3D>(enemy->GetEntity()));

			m_TimerDash += Time::GetDeltaTimeSeconds();
			m_TimerToStopDash += Time::GetDeltaTimeSeconds();

			if (m_TimerDash >= 0.6f && interpolationValue < 0.95f)
			{
				m_TimerDash = 0.0f;
				interpolationValue = interpolationValue + 0.05;
			}

			if (m_UltronJump == false)
			{
				glm::vec3 bossUltronToTheSky = glm::vec3(0.0f, 100.0f, 0.0f);

				agent->StopAgent();

				agent->RemoveAgent();

				selfTr->localPosition.x = 100.0f;

				m_UltronJump = true;
			}

			if (m_TimerToStopDash >= 2.0f)
			{
				agent->RegisterWithCrowd();
				agent->SetPosition(playerDistance);
				agent->StopAgent();

				WI_INFO("INTERPOLATION DONE");
				m_State = DashState::DASH_STOP;
			}
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_STOP:
		{
			Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

			float center_x = 0.0f; // center x position of the circle
			float center_z = 0.0f; // center z position of the circle
			float radius = 8.0f; // radius of the circle
			float angle_increment = 1.0f; // angle increment between consecutive explosions
			float current_angle = 0.0f; // current angle
			int numberOfExplosion = 6;

			for (int i = 0; i < numberOfExplosion; i++)
			{
				float x_pos = center_x + radius * cos(current_angle); // calculate x position of current explosion
				float z_pos = center_z + radius * sin(current_angle); // calculate z position of current explosion

				SpawnExplosionAfterDash(enemy, selfTr, x_pos, z_pos);

				current_angle += angle_increment; // increment angle for next explosion
			}

			/*SpawnExplosionAfterDash(enemy, selfTr);*/

			agent->StopAgent();
			navAgent->autoRotate = true;

			m_State = DashState::DASH_COOLDOWN;
		}
		break;
		case Wiwa::BossUltronDashState::DashState::DASH_COOLDOWN:
		{
			m_TimerAfterDash += Time::GetDeltaTimeSeconds();

			if (m_TimerAfterDash >= 2.0f)
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
		//std::string playerStr = "PLAYER";

		//if (body1->id == enemy->GetEntity() && playerStr == body2->selfTagStr /* && m_AlreadyHitted == false*/)
		//{
		//	WI_INFO("Ultron Collided");
		//	GameStateManager::DamagePlayer(20);
		//	
		//	/*m_AlreadyHitted = true;*/
		//}
	}

	void BossUltronDashState::SpawnExplosionAfterDash(BossUltron* enemy, Transform3D* selfTransform, float explosionPositionX, float explosionPositionZ)
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

		if (!explosiveBarrelTr)
			return;

		explosiveBarrelTr->localPosition.x = Math::GetWorldPosition(selfTransform->worldMatrix).x + explosionPositionX;
		explosiveBarrelTr->localPosition.y = Math::GetWorldPosition(selfTransform->worldMatrix).y;
		explosiveBarrelTr->localPosition.z = Math::GetWorldPosition(selfTransform->worldMatrix).z + explosionPositionZ;
		
		physSys->CreateBody();

		explosionSys->EnableExplosion();
	}
}
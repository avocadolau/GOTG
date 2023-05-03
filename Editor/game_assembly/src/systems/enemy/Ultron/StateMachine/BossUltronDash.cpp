#include <wipch.h>
#include "BossUltronDash.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <cmath>

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
		/*Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

		enemy->RotateTo(playerTr->localPosition);*/

		enemy->m_Timer = 0.0f;
		m_TimerToRotate = 0.0f;
		m_TimerDash = 0.0f;
		m_TimerAfterDash = 0.0f;
		playerDistance = glm::vec3(0.0f);
		interpolatedDashDistance = glm::vec3(0.0f);
		m_State = DashState::DASH_INIT;
		/*animator->Blend("walk", true, 0.2f);*/
		/*enemy->m_DashTimer = 0;*/
	}

	void BossUltronDashState::UpdateState(BossUltron* enemy)
	{
		///*m_ChasingTimer += Time::GetDeltaTime();*/

		//Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		//Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		//float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);

		//Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		//if (agent)
		//{
		//	Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		//	agent->SetDestination(playerTr->localPosition);
		//}

		//if (animator->HasFinished())//&& !IsPlaying(enemy->GetEntity())
		//{
		//	//SentinelAudio
		//	/*PlaySound(ScriptEngine::CreateString("melee_moving"), enemy->m_PlayerId);*/
		//}

		///*
		//if (glm::distance(selfTr->localPosition, playerTr->localPosition) < 4.0f)
		//{
		//	enemy->SwitchState(enemy->m_AttackingState);
		//}
		//*/
		/*Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());*/

		/*directionDash = Math::PointAlongDirection(enemy->GetTransform()->localPosition, enemy->GetTransform()->localRotation, dashDistance);*/

		/*directionDash = glm::vec3(-2.0f, 0.0f, -33.0f);

		agent->SetDestination(directionDash);*/

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
			//Part 1 del Dash
			enemy->LookAt(playerTr->localPosition, 80.0f);
			
			if (m_TimerToRotate >= 2.0f)
			{
				m_TimerToRotate = 0.0f,
				m_State = DashState::DASH_PLAYING;
			}
		}
			break;
		case Wiwa::BossUltronDashState::DashState::DASH_PLAYING:
		{
			m_TimerDash += Time::GetDeltaTimeSeconds();

			if (m_TimerDash >= 0.2f && interpolationValue <= 0.9f)
			{
				m_TimerDash = 0.0f;
				interpolationValue = interpolationValue + 0.05;
			}

			/*glm::vec3 playerLastPosition = playerTr->localPosition;*/
			/*directionDash = Math::PointAlongDirection(enemy->GetTransform()->localPosition, enemy->GetTransform()->localRotation, dashDistance);*/
			/*directionDash = Math::GetRoationBetweenTwoPoints(enemy->GetTransform()->localPosition, enemy->GetTransform()->localRotation);*/
			
			/*agent->RequestMoveVelocity(directionDash * dashVelocity);*/
			//directionDash = glm::vec3(-2.0f, 0.0f, -33.0f);

			//TODO: Fer Funcio per calcular la posicio be (x = v * t)

			/*float dashVelocity = dashDistance / lifetimeDash;*/

			interpolatedDashDistance = Math::InterpolateTwoV3(enemy->GetTransform()->localPosition, playerDistance, interpolationValue);

			agent->SetPosition(interpolatedDashDistance);

			if (interpolationValue > 0.9f)
			{
				m_State = DashState::DASH_STOP;
			}
		}
			break;
		case Wiwa::BossUltronDashState::DashState::DASH_STOP:
		{
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

		

		//if (initiateDash == false)
		//{
		//	agent->StopAgent();

		//	directionDash = glm::vec3(0.0f);
		//	//Part 1 del Dash
		//	enemy->RotateTo(playerTr->localPosition);
		//	/*glm::vec3 playerLastPosition = playerTr->localPosition;*/
		//	/*directionDash = Math::PointAlongDirection(enemy->GetTransform()->localPosition, enemy->GetTransform()->localRotation, dashDistance);*/
		//	directionDash = Math::GetRoationBetweenTwoPoints(enemy->GetTransform()->localPosition, enemy->GetTransform()->localRotation);
		//	float dashVelocity = dashDistance / lifetimeDash;
		//	agent->RequestMoveVelocity(directionDash * dashVelocity);

		//	initiateDash = true;
		//}

		//if (initiateDash == true)
		//{
		//	m_TimerDash += Time::GetDeltaTimeSeconds();
		//	m_TimerAfterDash += Time::GetDeltaTimeSeconds();
		//	
		//	if (m_TimerDash >= 2.0f)
		//	{
		//		m_TimerDash = 0.0f;
		//		agent->StopAgent();


		//		//Posibiliad switchState AQUI
		//	}
		//}
	}

	void BossUltronDashState::ExitState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//animator->Blend("idle", true, true, 0.5f);
		//animator->Pause();
	}

	void BossUltronDashState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
		std::string playerStr = "PLAYER";

		if (body1->id == enemy->GetEntity() && playerStr == body2->selfTagStr /* && m_AlreadyHitted == false*/)
		{
			WI_INFO("Ultron Collided");
			GameStateManager::DamagePlayer(20);
			
			/*m_AlreadyHitted = true;*/
		}
	}
}
#include <wipch.h>
#include "TransitionSecondPhase.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include <Wiwa/ecs/components/game/Health.h>
#include "../../../../components/enemy/BossMovementPoints.h"
#include "../../../../systems/enemy/EnemySystem.h"

namespace Wiwa
{
	BossUltronSecondPhaseState::BossUltronSecondPhaseState()
	{
		m_WallPrefabPath = "assets\\Enemy\\Ultron\\UltronRegenWall_01.wiprefab";
		m_EnemySpawnerPath = "assets\\Enemy\\WaveSpawner\\UltronWaveSpawner_01.wiprefab";
	}

	BossUltronSecondPhaseState::~BossUltronSecondPhaseState()
	{

	}

	void BossUltronSecondPhaseState::EnterState(BossUltron* enemy)
	{
		m_TimerSecondPhase = 0.0f;
		m_TimerHealing = 0.0f;
		m_SpawnEnemies = false;

		m_AfterRegenPosition.clear();
		FillPremadePositionAfterRegen(enemy, m_AfterRegenPosition);
		m_SecondPhaseState = Wiwa::BossUltronSecondPhaseState::SecondPhaseState::MOVE_CENTER;
	}

	void BossUltronSecondPhaseState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		Health* stats = (Health*)em.GetComponentByIterator(enemy->m_Health);

		
		

		switch (m_SecondPhaseState)
		{
		case Wiwa::BossUltronSecondPhaseState::SecondPhaseState::MOVE_CENTER:
		{
			glm::vec3 m_CenterPositionBoss = { 0.0f, 7.0f,0.0f };
			glm::vec3 m_CenterPositionRegenWall = { 0.0f, 13.0f,0.0f };
			navAgentPtr->StopAgent();
			navAgentPtr->RemoveAgent();
			selfTr->localPosition = m_CenterPositionBoss;

			m_RegenWallPrefabId = em.LoadPrefab(m_WallPrefabPath);
			Transform3D* regenWallPrefabTr = em.GetComponent<Transform3D>(m_RegenWallPrefabId);
			

			PhysicsSystem* physSysWallRegen = em.GetSystem<PhysicsSystem>(m_RegenWallPrefabId);
			physSysWallRegen->DeleteBody();

			if (!regenWallPrefabTr || !playerTr)
				return;

			regenWallPrefabTr->localPosition = m_CenterPositionRegenWall;
			regenWallPrefabTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
			regenWallPrefabTr->localScale = selfTr->localScale;

			physSysWallRegen->CreateBody();

			m_TimerSecondPhase = 0.0f;

			m_SpawnEnemies = true;

			m_SecondPhaseState = SecondPhaseState::REGENERATE;
		}
		break;
		case Wiwa::BossUltronSecondPhaseState::SecondPhaseState::REGENERATE:
		{
			m_TimerSecondPhase += Time::GetDeltaTimeSeconds();
			m_TimerHealing += Time::GetDeltaTimeSeconds();

			if (m_TimerHealing >= 1.0f)
			{
				if (enemy->m_MaxHealth <= stats->health)
				{
					stats->health = enemy->m_MaxHealth;
				}
				else
				{
					stats->health = stats->health + 20.0f;
				}

				m_TimerHealing = 0.0f;
			}

			if (m_SpawnEnemies)
			{
				//Add enemies
				m_SpawnerPrefabId = em.LoadPrefab(m_EnemySpawnerPath);
				m_SpawnEnemies = false;
			}

			if (m_TimerSecondPhase >= 24.0f) //Cambiar por num enemigos derrotados
			{
				m_TimerSecondPhase = 0.0f;
				m_SecondPhaseState = SecondPhaseState::END_STATE;
			}
		}
		break;
		case Wiwa::BossUltronSecondPhaseState::SecondPhaseState::END_STATE:
		{

			//Destroy wall
			em.DestroyEntity(m_RegenWallPrefabId);
			em.DestroyEntity(m_SpawnerPrefabId);

			selfTr->localPosition = GetNewPositionAfterRegen();
			navAgentPtr->RegisterWithCrowd();
			navAgentPtr->SetPosition(selfTr->localPosition);
			navAgentPtr->StopAgent();

			enemy->SwitchState(enemy->m_MovementState);
		}
		break;
		
		default:
			break;
		}
	}

	void BossUltronSecondPhaseState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronSecondPhaseState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	void BossUltronSecondPhaseState::FillPremadePositionAfterRegen(BossUltron* enemy, std::vector<glm::vec3>& vec)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		size_t size = 0;
		Wiwa::BossMovementPoints* pointsList = nullptr;
		pointsList = em.GetComponents<BossMovementPoints>(&size);
		if (pointsList)
		{
			for (int i = 0; i < size; i++)
			{
				if (em.IsComponentRemoved<BossMovementPoints>(i)) {
				}
				else {
					Wiwa::BossMovementPoints* point = &pointsList[i];
					if (point)
					{
						vec.emplace_back(point->point);

						if (point->isCenter)
							enemy->m_SceneCenterPos = point->point;
					}
				}
			}
		}
	}

	glm::vec3 BossUltronSecondPhaseState::GetNewPositionAfterRegen()
	{
		std::uniform_int_distribution<> disEnemies(0, m_AfterRegenPosition.size() - 1);
		int randomNum = disEnemies(Application::s_Gen);
		return m_AfterRegenPosition[randomNum];
	}
}
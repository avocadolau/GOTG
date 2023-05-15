#include <wipch.h>
#include "BossUltronProjectileRainAttack.h"
#include "../../../../components/attack/RainProjectile.h"
#include "../../../attack/RainProjectileSystem.h"
#include "../BossUltron.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>


#define RAIN_PROJECTILE_NUMBER 10

Wiwa::BossUltronProjectileRainAttackState::BossUltronProjectileRainAttackState()
{
	m_ThunderMarkPath = "assets\\Enemy\\RainProjectile\\ThunderMark_01.wiprefab";
}

Wiwa::BossUltronProjectileRainAttackState::~BossUltronProjectileRainAttackState()
{

}

void Wiwa::BossUltronProjectileRainAttackState::EnterState(BossUltron* enemy)
{
	m_TimerRain = 0.0f;
	m_RainProjectileCounter = 0;
	m_RainState = ProjectileRainState::PREPARE_RAIN;
	Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
	//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
	//ParticleManager& pman = enemy->getScene().GetParticleManager();
	//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
	//EntityId currentEnemy = enemy->GetEntity();

	
	NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
	if (navAgent)
	{
		navAgent->autoRotate = true;
	}

	//EntityId currentEnemy = enemy->GetEntity();

	//pman.EmitBatch(currentEnemy);

	//animator->PlayAnimation("spawn", false);
}

void Wiwa::BossUltronProjectileRainAttackState::UpdateState(BossUltron* enemy)
{
	Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();	

	switch (m_RainState)
	{
	case ProjectileRainState::PREPARE_RAIN:
	{
		//WI_INFO("Prepare rain");

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();

		if (m_TimerRain >= TIME_BETWEEN_PROJECTILES)
		{
			m_RainState = ProjectileRainState::RAIN_ATTACK;
			m_TimerRain = 0.0f;
			m_RainProjectileCounter = 0;
			m_TimerThundersSpawn = 0.0f;
			m_TimerThundersMark = 0.0f;
		}

	}break;
	case ProjectileRainState::RAIN_ATTACK:
	{
		m_TimerThundersSpawn += Time::GetDeltaTimeSeconds();
		m_TimerThundersMark += Time::GetDeltaTimeSeconds();

		if (m_TimerThundersSpawn >= 0.1f)
		{
			m_TimerRain = 0.0f;

			if (!m_RandomPositionSetted)
			{
				m_ThunderMarkId1 = em.LoadPrefab(m_ThunderMarkPath);
				Transform3D* thunderMarkTr1 = em.GetComponent<Transform3D>(m_ThunderMarkId1);
				m_ThunderPos1 = RandomPointInHexagon();
				thunderMarkTr1->localPosition.x = m_ThunderPos1.x;
				thunderMarkTr1->localPosition.y = 0.1f;
				thunderMarkTr1->localPosition.z = m_ThunderPos1.z;

				m_RandomPositionSetted = true;
			}

			if (m_TimerThundersMark >= 1.1f)
			{
				SpawnThunderStorm(enemy, m_ThunderPos1, { 0.0f,-1.0f,0.0f });


				m_TimerThundersMark = 0.0f;
				m_TimerThundersSpawn = 0.0f;
				m_RandomPositionSetted = false;
				m_RainProjectileCounter++;

				em.DestroyEntity(m_ThunderMarkId1);
			}
		}

		if (RAIN_PROJECTILE_NUMBER <= m_RainProjectileCounter)
		{
			m_RainState = ProjectileRainState::END_STATE;
		}


	}break;
	case ProjectileRainState::END_STATE:
	{
		//WI_INFO("End rain");
		enemy->SwitchState(enemy->m_MovementState);

	}break;
	}	
	m_TimerRain += Time::GetDeltaTimeSeconds();
}

void Wiwa::BossUltronProjectileRainAttackState::ExitState(BossUltron* enemy)
{
}

glm::vec3 Wiwa::BossUltronProjectileRainAttackState::GetRandomPositionInRange(const glm::vec3& position, float range)
{
	float theta = 2.0f * PI_F * (float(RANDF(0.0f, RAND_MAX)) / float(RAND_MAX));
	float r = range * std::sqrt(float(RANDF(0.0f, RAND_MAX)) / float(RAND_MAX));

	glm::vec3 randomOffset;
	randomOffset.x = r * std::cos(theta);
	randomOffset.z = r * std::sin(theta);
	randomOffset.y = 0.0f;

	return position + randomOffset;
}

void Wiwa::BossUltronProjectileRainAttackState::SpawnThunderStorm(BossUltron* enemy, glm::vec3 thunderPosition, const glm::vec3& bull_dir)
{
	if (GameStateManager::s_PoolManager->s_RainProjectilePool->getCountDisabled() <= 0)
		return;

	Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
	GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
	EntityId newBulletId = EntityManager::INVALID_INDEX;
	newBulletId = GameStateManager::s_PoolManager->s_RainProjectilePool->GetFromPool();

	if (newBulletId == EntityManager::INVALID_INDEX)
		return;

	RainProjectileSystem* bulletSys = entityManager.GetSystem<RainProjectileSystem>(newBulletId);
	PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
	physSys->DeleteBody();

	// Set intial positions
	Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

	if (!bulletTr || !playerTr)
		return;

	bulletTr->localPosition = thunderPosition;
	bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
	RainProjectile* bullet = (RainProjectile*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<RainProjectile>(newBulletId));
	BossUltron* ultron = (BossUltron*)entityManager.GetComponentByIterator(enemy->m_Ultron);

	bullet->direction = bull_dir;

	physSys->CreateBody();

	bulletSys->EnableBullet();
}

glm::vec3 Wiwa::BossUltronProjectileRainAttackState::RandomPointInHexagon()
{
	// Create a random number generator
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	while (true) {
		// Select a random vertex of the hexagon
		int randomIndex = static_cast<int>(distribution(generator) * hexagonVertices.size());
		glm::vec3 vertex = hexagonVertices[randomIndex];

		// Generate random barycentric coordinates
		float u = distribution(generator);
		float v = distribution(generator);

		// Check if the point is inside the hexagon
		if (u + v > 1.0f) {
			u = 1.0f - u;
			v = 1.0f - v;
		}

		// Compute the point inside the hexagon
		glm::vec3 point = vertex + u * (hexagonVertices[(randomIndex + 1) % hexagonVertices.size()] - vertex) +
			v * (hexagonVertices[(randomIndex + 2) % hexagonVertices.size()] - vertex);

		return point;
	}
}

//bool Wiwa::BossUltronProjectileRainAttackState::SpawnProjectileRain(BossUltron* enemy, const glm::vec3& bull_dir)
//{
//	if (GameStateManager::s_PoolManager->s_RainProjectilePool->getCountDisabled() <= 0)
//		return false;
//
//	Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
//	GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
//	EntityId newBulletId = GameStateManager::s_PoolManager->s_RainProjectilePool->GetFromPool();
//
//	if (newBulletId == WI_INVALID_INDEX)
//		return false;
//
//	Wiwa::RainProjectileSystem* rainProjectileSystem = entityManager.GetSystem<Wiwa::RainProjectileSystem>(newBulletId);
//	Wiwa::PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
//	Wiwa::AnimatorSystem* animator = entityManager.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
//
//	animator->PlayAnimation("A_attack_shot", false);
//
//	if (physSys != nullptr)
//	{
//		physSys->DeleteBody();
//	}
//
//	// Set intial positions
//	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
//	Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
//
//	if (!bulletTr || !playerTr)
//		return false;
//
//	glm::vec3 spawnPosition = GetRandomPositionInRange(playerTr->localPosition, RAIN_RANGE);
//	spawnPosition.y += 30.0f;
//
//	EnemyData* stats = (EnemyData*)entityManager.GetComponentByIterator(enemy->m_StatsIt);
//	Ultron* ultron = (Ultron*)entityManager.GetComponentByIterator(enemy->m_Ultron);
//
//	bulletTr->localPosition = spawnPosition;
//	bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, 90.0f);
//	RainProjectile* bullet = (RainProjectile*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<RainProjectile>(newBulletId));
//	bullet->direction = bull_dir;
//	bullet->velocity = ultron->bulletSpeed;
//	bullet->lifeTime = ultron->bulletLifeTime;
//	bullet->damage = stats->damage;
//
//	physSys->CreateBody();
//
//	rainProjectileSystem->EnableBullet();
//
//	return true;
//}




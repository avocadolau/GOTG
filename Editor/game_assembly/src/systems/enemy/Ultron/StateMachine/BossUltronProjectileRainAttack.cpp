#include <wipch.h>
#include "BossUltronProjectileRainAttack.h"
#include "../../../../components/attack/RainProjectile.h"
#include "../../../attack/RainProjectileSystem.h"
#include "../BossUltron.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

Wiwa::BossUltronProjectileRainAttackState::BossUltronProjectileRainAttackState()
{
}

Wiwa::BossUltronProjectileRainAttackState::~BossUltronProjectileRainAttackState()
{
}

void Wiwa::BossUltronProjectileRainAttackState::EnterState(BossUltron* enemy)
{
	m_TimerRain = 0.0f;
	m_RainProjectileCounter = 0;
	m_RainState = ProjectileRainState::PREPARE_RAIN;
	//Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
	//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
	//ParticleManager& pman = enemy->getScene().GetParticleManager();
	//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
	//EntityId currentEnemy = enemy->GetEntity();

	
	//NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
	//if (navAgent)
	//{
	//	navAgent->autoRotate = false;
	//}

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
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();

		m_RainState = ProjectileRainState::RAIN_ATTACK;

	}break;
	case ProjectileRainState::RAIN_ATTACK:
	{
		if (m_TimerRain >= TIME_BETWEEN_PROJECTILES)
		{
			SpawnProjectileRain(enemy, { 0.0f,-1.0f,0.0f });

			m_TimerRain = 0.0f;
			m_RainProjectileCounter++;
		}

		if (RAIN_PROJECTILE_NUMBER >= m_RainProjectileCounter)
		{
			m_RainState = ProjectileRainState::END_STATE;
		}
		m_TimerRain += Time::GetDeltaTimeSeconds();

	}break;
	case ProjectileRainState::END_STATE:
	{
		enemy->SwitchState(enemy->m_MovementState);

	}break;
	}	
}

void Wiwa::BossUltronProjectileRainAttackState::ExitState(BossUltron* enemy)
{
}

bool Wiwa::BossUltronProjectileRainAttackState::SpawnProjectileRain(BossUltron* enemy, const glm::vec3& bull_dir)
{
	if (GameStateManager::s_PoolManager->s_RainProjectilePool->getCountDisabled() <= 0)
		return false;

	Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
	GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
	EntityId newBulletId = GameStateManager::s_PoolManager->s_RainProjectilePool->GetFromPool();

	Wiwa::RainProjectileSystem* rainProjectileSystem = entityManager.GetSystem<Wiwa::RainProjectileSystem>(newBulletId);
	Wiwa::PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
	Wiwa::AnimatorSystem* animator = entityManager.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

	animator->PlayAnimation("A_attack_shot", false);

	if (physSys != nullptr)
	{
		physSys->DeleteBody();
	}

	// Set intial positions
	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
	Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);

	if (!bulletTr || !playerTr)
		return false;

	glm::vec3 spawnPosition = GetRandomPositionInRange(playerTr->localPosition, RAIN_RANGE);
	spawnPosition.y += 4.0f;

	EnemyData* stats = (EnemyData*)entityManager.GetComponentByIterator(enemy->m_StatsIt);
	Ultron* ultron = (Ultron*)entityManager.GetComponentByIterator(enemy->m_Ultron);

	bulletTr->localPosition = spawnPosition;
	bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, 90.0f);
	RainProjectile* bullet = (RainProjectile*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<RainProjectile>(newBulletId));
	bullet->direction = bull_dir;
	bullet->velocity = ultron->bulletSpeed;
	bullet->lifeTime = ultron->bulletLifeTime;
	bullet->damage = stats->damage;

	physSys->CreateBody();

	rainProjectileSystem->EnableBullet();

	return true;
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

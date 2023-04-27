#include <wipch.h>
#include "ExplosiveBarrelSystem.h"
#include "../../components/attack/ExplosiveBarrel.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

namespace Wiwa
{
	ExplosiveBarrelSystem::ExplosiveBarrelSystem()
	{
		m_BarrelExplosionIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	ExplosiveBarrelSystem::~ExplosiveBarrelSystem()
	{

	}

	void ExplosiveBarrelSystem::OnAwake()
	{
		m_BarrelExplosionIt = GetComponentIterator<ExplosiveBarrel>();
	}

	void ExplosiveBarrelSystem::OnInit()
	{
		InitExplosion();
	}

	void ExplosiveBarrelSystem::InitExplosion()
	{
		ExplosiveBarrel* explosion = GetComponentByIterator<ExplosiveBarrel>(m_BarrelExplosionIt);

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
	}

	void ExplosiveBarrelSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		ExplosiveBarrel* explosion = GetComponentByIterator<ExplosiveBarrel>(m_BarrelExplosionIt);

		m_Timer += Time::GetDeltaTimeSeconds();

		if (m_Timer >= explosion->lifeTime)
		{
			/*Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);*/
			GameStateManager::s_PoolManager->s_ExplosiveBarrel->ReturnToPool(m_EntityId); // meter nueva pool
		}

	}


	void ExplosiveBarrelSystem::OnDestroy()
	{

	}

	void ExplosiveBarrelSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{
				ExplosiveBarrel* explosionPlayer = GetComponentByIterator<ExplosiveBarrel>(m_BarrelExplosionIt);
				GameStateManager::DamagePlayer(explosionPlayer->damage);
				//Spawn explosion
			}

			/*std::string enemyStr = "ENEMY_MELEE";
			if (enemyStr == body2->selfTagStr)
			{
				ExplosiveBarrel* explosionEnemy = GetComponentByIterator<ExplosiveBarrel>(m_BarrelExplosionIt);
				EnemySystem::ReceiveDamage(explosionEnemy->damage);
			}*/

			/*		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
					em.DestroyEntity(m_EntityId);*/
		}
	}

	/*void ExplosiveBarrelSystem::EnableExplosion()
	{
		InitExplosion();
	}

	bool ExplosiveBarrelSystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 20000.0f;
		}

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);
		physSystem->DeleteBody();

		m_Timer = 0.0f;
		return true;
	}*/

}
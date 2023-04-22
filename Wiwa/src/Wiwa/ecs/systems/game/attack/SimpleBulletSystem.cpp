#include <wipch.h>
#include "SimpleBulletSystem.h"
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>

namespace Wiwa
{
	SimpleBulletSystem::SimpleBulletSystem()
	{
		m_BulletIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	SimpleBulletSystem::~SimpleBulletSystem()
	{

	}

	void SimpleBulletSystem::OnAwake()
	{
		m_BulletIt = GetComponentIterator<SimpleBullet>();
	}

	void SimpleBulletSystem::OnInit()
	{
		InitBullet();
	}

	void SimpleBulletSystem::InitBullet()
	{
		SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(m_BulletIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
	}

	void SimpleBulletSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(m_BulletIt);

		m_Timer += Time::GetDeltaTimeSeconds();

		if (m_Timer >= bullet->lifeTime)
		{
			/*Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);*/
			GameStateManager::s_PoolManager->s_SimpleBulletsPool->ReturnToPool(m_EntityId);
		}	
	}

	void SimpleBulletSystem::OnDestroy()
	{

	}

	void SimpleBulletSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{
				SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(m_BulletIt);
				GameStateManager::DamagePlayer(bullet->damage);
			}
	
			/*Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);*/
			GameStateManager::s_PoolManager->s_SimpleBulletsPool->ReturnToPool(m_EntityId);
		}
	}

	bool SimpleBulletSystem::OnEnabledFromPool()
	{
		SimpleBullet* bullet = GetComponent<SimpleBullet>();
		if (bullet)
		{
			InitBullet();
		}

		return true;
	}

	bool SimpleBulletSystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 20000.0f;
		}

		SimpleBullet* bullet = GetComponent<SimpleBullet>();

		m_Timer = 0.0f;

		return true;
	}
}

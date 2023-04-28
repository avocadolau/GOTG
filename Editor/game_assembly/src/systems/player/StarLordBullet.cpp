#include "StarLordBullet.h"
#include "../../components/attack/SimpleBullet.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>

namespace Wiwa
{
	StarLordBulletSystem::StarLordBulletSystem()
	{
		m_BulletIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	StarLordBulletSystem::~StarLordBulletSystem()
	{

	}

	void StarLordBulletSystem::OnAwake()
	{
		m_BulletIt = GetComponentIterator<SimpleBullet>();
	}

	void StarLordBulletSystem::OnInit()
	{

	}

	void StarLordBulletSystem::InitBullet()
	{
		if (!getAwake())
			System::Awake();
		/*GameStateManager::s_PoolManager->SetScene(m_Scene);
		GameStateManager::s_PoolManager->s_SimpleBulletsPool->GetFromPool();*/

		SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(m_BulletIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
		//Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		//PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

		//if (physSystem)
		//{
		//	physSystem->DeactivateBody();
		//}
	}

	void StarLordBulletSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(m_BulletIt);

		m_Timer += Time::GetDeltaTimeSeconds();

		if (m_Timer >= bullet->lifeTime)
		{
			GameStateManager::s_PoolManager->s_StarLordBullets->ReturnToPool(m_EntityId);
		}
	}

	void StarLordBulletSystem::OnDestroy()
	{

	}

	void StarLordBulletSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string enemyStr = "ENEMY";
			if (enemyStr == body2->selfTagStr)
			{
				SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(m_BulletIt);
				GameStateManager::DamagePlayer(bullet->damage);
			}

			GameStateManager::s_PoolManager->s_StarLordBullets->ReturnToPool(m_EntityId);
		}
	}

	bool StarLordBulletSystem::EnableBullet()
	{
		SimpleBullet* bullet = GetComponent<SimpleBullet>();
		if (bullet)
		{
			InitBullet();
		}
		m_Timer = 0.0f;
		return true;
	}

	bool StarLordBulletSystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 3000.0f;
		}

		//SimpleBullet* bullet = GetComponent<SimpleBullet>();
		//
		//if (bullet)
		//{
		//	bullet->direction = glm::vec3(0.0f);
		//	bullet->lifeTime = 0.0f;
		//	bullet->velocity = 0.0f;
		//}

		//CollisionBody* collisionBody = GetComponent<CollisionBody>();

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

		physSystem->DeleteBody();

		m_Timer = 0.0f;

		return true;
	}
}
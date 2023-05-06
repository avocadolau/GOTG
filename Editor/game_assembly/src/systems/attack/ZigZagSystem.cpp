#include "ZigZagSystem.h"
#include "../../components/attack/ZigZagBullet.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>
#include "../../components/attack/Attack.h"

namespace Wiwa
{
	ZigZagBulletSystem::ZigZagBulletSystem()
	{
		m_BulletIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AttackIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
		m_ZigZagTimer = 0.0f;
	}

	ZigZagBulletSystem::~ZigZagBulletSystem()
	{

	}

	void ZigZagBulletSystem::OnAwake()
	{
		m_AttackIt = GetComponentIterator<Attack>();
		m_BulletIt = GetComponentIterator<ZigZagBullet>();

		Attack* attack = GetComponentByIterator<Attack>(m_AttackIt);
		if (attack)
		{
			std::string atStr = "ZIG_ZAG_BULLET";
			strcpy(attack->attackType, atStr.c_str());
			attack->isPlayerAttack = false;
			attack->isEnemyAttack = true;
		}
	}

	void ZigZagBulletSystem::OnInit()
	{

	}

	void ZigZagBulletSystem::InitBullet()
	{
		if (!getAwake())
			System::Awake();
		
		ZigZagBullet* bullet = GetComponentByIterator<ZigZagBullet>(m_BulletIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
	}

	void ZigZagBulletSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		ZigZagBullet* bullet = GetComponentByIterator<ZigZagBullet>(m_BulletIt);
		Transform3D* transform = (Transform3D*)em.GetComponentByIterator(em.GetComponentIterator<Transform3D>(m_EntityId));

		m_Timer += Time::GetDeltaTimeSeconds();
		m_ZigZagTimer += Time::GetDeltaTime();



		if (m_Timer >= bullet->lifeTime)
		{
			GameStateManager::s_PoolManager->s_ZigZagBulletPool->ReturnToPool(m_EntityId);
		}
	}

	void ZigZagBulletSystem::OnDestroy()
	{

	}

	void ZigZagBulletSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{
				ZigZagBullet* bullet = GetComponentByIterator<ZigZagBullet>(m_BulletIt);
				GameStateManager::DamagePlayer(bullet->damage);
			}

			GameStateManager::s_PoolManager->s_ZigZagBulletPool->ReturnToPool(m_EntityId);
		}
	}

	bool ZigZagBulletSystem::EnableBullet()
	{
		ZigZagBullet* bullet = GetComponent<ZigZagBullet>();
		if (bullet)
		{
			InitBullet();
		}
		m_Timer = 0.0f;
		m_ZigZagTimer = 0.0f;
		return true;
	}

	bool ZigZagBulletSystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 3000.0f;
		}

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

		physSystem->DeleteBody();

		m_Timer = 0.0f;
		m_ZigZagTimer = 0.0f;

		return true;
	}
}
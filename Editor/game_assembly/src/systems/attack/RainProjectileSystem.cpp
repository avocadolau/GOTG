#include "RainProjectileSystem.h"
#include "../../components/attack/RainProjectile.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>

Wiwa::RainProjectileSystem::RainProjectileSystem()
{
	m_BulletIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
	m_Timer = 0.0f;
	m_DamageHasBeenApplied = false;
}

Wiwa::RainProjectileSystem::~RainProjectileSystem()
{
}

void Wiwa::RainProjectileSystem::OnAwake()
{
	m_BulletIt = GetComponentIterator<RainProjectile>();
}

void Wiwa::RainProjectileSystem::OnInit()
{	
}

void Wiwa::RainProjectileSystem::InitRainProjectileBullet()
{
	if (!getAwake())
		System::Awake();

	RainProjectile* bullet = GetComponentByIterator<RainProjectile>(m_BulletIt);

	Wiwa::EntityManager& em = m_Scene->GetEntityManager();
	Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

	Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();

	physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);

}

void Wiwa::RainProjectileSystem::OnUpdate()
{
	if (!getAwake())
		System::Awake();
	if (!getInit())
		System::Init();

	RainProjectile* bullet = GetComponentByIterator<RainProjectile>(m_BulletIt);

	if (m_Timer >= bullet->lifeTime)
	{
		GameStateManager::s_PoolManager->s_RainProjectilePool->ReturnToPool(m_EntityId);
	}

	m_Timer += Time::GetDeltaTimeSeconds();
}

void Wiwa::RainProjectileSystem::OnDestroy()
{
}

void Wiwa::RainProjectileSystem::OnCollisionEnter(Object* body1, Object* body2)
{
	if (body1->id == m_EntityId)
	{
		if (m_DamageHasBeenApplied == false)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{

				RainProjectile* bullet = GetComponentByIterator<RainProjectile>(m_BulletIt);
				GameStateManager::DamagePlayer(bullet->damage);

				//GameStateManager::s_PoolManager->s_ClusterBulletsPool->ReturnToPool(m_EntityId);
				m_DamageHasBeenApplied = true;
			}
		}	
		
	}
}

bool Wiwa::RainProjectileSystem::EnableBullet()
{
	RainProjectile* clusterBullet = GetComponent<RainProjectile>();
	if (clusterBullet)
	{
		InitRainProjectileBullet();
	}

	return true;
	
}

bool Wiwa::RainProjectileSystem::OnDisabledFromPool()
{
	Transform3D* transform = GetComponent<Transform3D>();
	if (transform)
	{
		transform->localPosition.y = 4100.0f;
		//WI_INFO("transform local pos");
	}

	Wiwa::EntityManager& em = m_Scene->GetEntityManager();
	//WI_INFO("em");
	PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);
	//WI_INFO("physsys");
	physSystem->DeleteBody();
	//WI_INFO("delete body");

	m_Timer = 0.0f;
	m_DamageHasBeenApplied = false;

	return true;
}

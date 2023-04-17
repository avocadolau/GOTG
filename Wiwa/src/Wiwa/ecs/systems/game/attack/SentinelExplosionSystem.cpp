#include <wipch.h>
#include "SentinelExplosionSystem.h"
#include "Wiwa/ecs/components/game/attack/SentinelExplosion.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

namespace Wiwa
{
	SentinelExplosionSystem::SentinelExplosionSystem()
	{
		m_ExplosionIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	SentinelExplosionSystem::~SentinelExplosionSystem()
	{

	}

	void SentinelExplosionSystem::OnAwake()
	{
		m_ExplosionIt = GetComponentIterator<SentinelExplosion>();
	}

	void SentinelExplosionSystem::OnInit()
	{
		SentinelExplosion* explosion = GetComponentByIterator<SentinelExplosion>(m_ExplosionIt);

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();

		/*physicsManager.SetVelocity(obj, glm::normalize(explosion->direction) * bullet->velocity);*/

		//physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
	}

	void SentinelExplosionSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		SentinelExplosion* explosion = GetComponentByIterator<SentinelExplosion>(m_ExplosionIt);

		m_Timer += Time::GetDeltaTimeSeconds();

		if (m_Timer >= explosion->lifeTime)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}

	}


	void SentinelExplosionSystem::OnDestroy()
	{

	}

	void SentinelExplosionSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{
				SentinelExplosion* explosion = GetComponentByIterator<SentinelExplosion>(m_ExplosionIt);
				GameStateManager::DamagePlayer(explosion->damage);
			}

			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}
	}

}
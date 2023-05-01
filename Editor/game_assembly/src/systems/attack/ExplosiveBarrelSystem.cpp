#include "ExplosiveBarrelSystem.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>
#include "../../components/attack/Explosion.h"

namespace Wiwa
{
	ExplosiveBarrelSystem::ExplosiveBarrelSystem()
	{
		m_ExplosiveBarrelIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	ExplosiveBarrelSystem::~ExplosiveBarrelSystem()
	{

	}

	void ExplosiveBarrelSystem::OnAwake()
	{
		m_ExplosiveBarrelIt = GetComponentIterator<Explosion>();
	}

	void ExplosiveBarrelSystem::OnInit()
	{

	}

	void ExplosiveBarrelSystem::InitExplosion()
	{
		if (!getAwake())
			System::Awake();

		Explosion* explosiveBarrel = GetComponentByIterator<Explosion>(m_ExplosiveBarrelIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();
	}

	void ExplosiveBarrelSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		Explosion* explosiveBarrel = GetComponentByIterator<Explosion>(m_ExplosiveBarrelIt);

		m_Timer += Time::GetDeltaTimeSeconds();

		if (m_Timer >= explosiveBarrel->lifeTime)
		{
			GameStateManager::s_PoolManager->s_ExplosiveBarrel->ReturnToPool(m_EntityId);
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
				Explosion* explosiveBarrel = GetComponentByIterator<Explosion>(m_ExplosiveBarrelIt);
				GameStateManager::DamagePlayer(explosiveBarrel->damage);
			}
		}
	}
	/*void ExplosiveBarrelSystem::OnCollision(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{
				ExplosiveBarrel* explosiveBarrel = GetComponentByIterator<ExplosiveBarrel>(m_ExplosiveBarrelIt);
				GameStateManager::DamagePlayer(explosiveBarrel->damage);
			}
		}
	}*/

	bool ExplosiveBarrelSystem::EnableExplosion()
	{
		Explosion* explosiveBarrel = GetComponent<Explosion>();
		if (explosiveBarrel)
		{
			InitExplosion();
			/*GetTransform()->localPosition = pos;*/
		}
		m_Timer = 0.0f;
		return true;
	}

	bool ExplosiveBarrelSystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 2000.0f;
		}

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

		physSystem->DeleteBody();

		m_Timer = 0.0f;

		return true;
	}
}
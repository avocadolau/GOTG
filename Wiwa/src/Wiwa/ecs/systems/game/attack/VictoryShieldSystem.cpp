#include <wipch.h>
#include "VictoryShieldSystem.h"
#include "Wiwa/ecs/components/game/attack/VictoryShield.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

namespace Wiwa
{
	VictoryShieldSystem::VictoryShieldSystem()
	{
		m_ShieldIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	VictoryShieldSystem::~VictoryShieldSystem()
	{

	}

	void VictoryShieldSystem::OnAwake()
	{
		m_ShieldIt = GetComponentIterator<VictoryShield>();
		m_ShieldTransfromIt = GetComponentIterator<Transform3D>();
	}

	void VictoryShieldSystem::OnInit()
	{
		VictoryShield* shield = GetComponentByIterator<VictoryShield>(m_ShieldIt);

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		//Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		EntityId player = GameStateManager::GetPlayerId();

		//Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		
		m_PlayerTransformIt = GetComponentIterator<Transform3D>(player);

		Wiwa::Transform3D* tranformShield = GetComponentByIterator<Transform3D>(m_ShieldTransfromIt);
		Wiwa::Transform3D* transformPlayer = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

		tranformShield->position = transformPlayer->position - glm::vec3(2.0f, 0.0f, 0.0f);
		//physicsManager.SetVelocity(obj, glm::normalize(shield->direction) * shield->velocity);
		
	}

	void VictoryShieldSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		VictoryShield* shield = GetComponentByIterator<VictoryShield>(m_ShieldIt);

		Wiwa::Transform3D* tranformShield = GetComponentByIterator<Transform3D>(m_ShieldTransfromIt);
		Wiwa::Transform3D* transformPlayer = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

		tranformShield->position = transformPlayer->position;
		tranformShield->localRotation.y += shield->velocity * Time::GetDeltaTimeSeconds();

		if (m_Timer >= shield->lifeTime)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}

	}


	void VictoryShieldSystem::OnDestroy()
	{

	}

	void VictoryShieldSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		/*if (body1->id == m_EntityId && strcmp(body2->selfTagStr, "ENEMY_BULLET") == 0)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(body2->id);
		}*/
	}

}

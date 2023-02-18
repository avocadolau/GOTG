#include <wipch.h>
#include "PhysicsSystem.h"
#include "Wiwa/physics/PhysicsManager.h"

namespace Wiwa {
	PhysicsSystem::PhysicsSystem()
	{

	}
	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::OnAwake()
	{
	}

	void PhysicsSystem::OnInit()
	{
		m_PhysicsManager = &Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
	}

	void PhysicsSystem::OnUpdate()
	{
		m_PhysicsManager->UpdateEngineToPhysics();

		if (Time::IsPlaying())
		{
			Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
			const char* e_name = entityManager.GetEntityName(m_EntityId);
			WI_INFO("Updating physics of --> {}", e_name);

			m_PhysicsManager->StepSimulation();
			m_PhysicsManager->UpdatePhysicsToEngine();
		}

	}

	void PhysicsSystem::OnDestroy()
	{
		DeleteBody();
	}

	void PhysicsSystem::OnSystemAdded()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Init physics of --> {}", e_name);

		Rigidbody* rb = GetComponent<Rigidbody>();
		ColliderCube* cube = GetComponent<ColliderCube>();
		Transform3D* transform = GetComponent<Transform3D>();

		if (rb && cube)
		{
			m_PhysicsManager->AddBodyCube(m_EntityId, *cube, *transform, *rb);
			m_PhysicsManager->UpdateEngineToPhysics();
		}
	}
	void PhysicsSystem::OnSystemRemoved()
	{
		DeleteBody();
	}

	void PhysicsSystem::DeleteBody()
	{
		m_PhysicsManager->UpdateEngineToPhysics();
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Deleting physics of --> {}", e_name);

		btRigidBody* body = m_PhysicsManager->FindByEntityId(m_EntityId);

		if (body != nullptr)
			m_PhysicsManager->DeleteBody(body);
	}
}
#include <wipch.h>
#include "PhysicsSystem.h"
#include "Wiwa/physics/PhysicsManager.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/utilities/render/Model.h"
#include "Wiwa/ecs/components/Mesh.h"

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
	}

	void PhysicsSystem::OnUpdate()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
	}

	void PhysicsSystem::OnDestroy()
	{
		DeleteBody();
	}

	void PhysicsSystem::OnSystemAdded()
	{
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);

		Rigidbody* rb = GetComponent<Rigidbody>();
		Transform3D* transform = GetComponent<Transform3D>();

		ColliderCube* cube = GetComponent<ColliderCube>();
		ColliderSphere* sphere = GetComponent<ColliderSphere>();
		ColliderCylinder* cylinder = GetComponent<ColliderCylinder>();

		Mesh* mesh = GetComponent<Mesh>();

		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		if (rb)
		{
			if (cube)
			{
				if (mesh) cube->halfExtents = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId)->boundingBox.HalfSize();
				physicsManager.AddBodyCube(m_EntityId, *cube, *transform, *rb);
			}
			else if (sphere)
			{
				physicsManager.AddBodySphere(m_EntityId, *sphere, *transform, *rb);
			}
			else if (cylinder)
			{
				physicsManager.AddBodyCylinder(m_EntityId, *cylinder, *transform, *rb);
			}
		}
	}
	void PhysicsSystem::OnSystemRemoved()
	{
		DeleteBody();
	}

	void PhysicsSystem::DeleteBody()
	{
		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		MyRigidBody* body = physicsManager.FindByEntityId(m_EntityId);

		if (body != nullptr)
			physicsManager.DeleteBody(body);
	}
}
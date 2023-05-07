#include "TestingSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	TestingSystem::TestingSystem()
	{
		m_SpawnOnce = false;
	}

	TestingSystem::~TestingSystem()
	{
	}

	void TestingSystem::OnAwake()
	{
	}

	void TestingSystem::OnInit()
	{
		SetTransform();
	}

	void TestingSystem::OnUpdate()
	{
		if (Input::IsKeyPressed(Key::P) && !m_SpawnOnce)
		{
			EntityManager& em = GetEntityManager();
			EntityId id = em.LoadPrefab("assets\\Prefabs\\YondusFin\\YundusFin_01.wiprefab");
			Transform3D* tr = em.GetComponent<Transform3D>(id);
			PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(id);
			NavAgentSystem* navAgentSys = em.GetSystem<NavAgentSystem>(id);
			*tr = *GetTransform();
		/*	tr->localPosition.y = 2.3f;
			tr->position = tr->localPosition;
			physSys->ForceSetPosition(tr->localPosition);
			navAgentSys->OnInit();
			navAgentSys->SetPosition(tr->localPosition);*/
			m_SpawnOnce = true;
		}
	}
}
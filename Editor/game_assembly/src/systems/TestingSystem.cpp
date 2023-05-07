#include "TestingSystem.h"

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
			*tr = *GetTransform();
			m_SpawnOnce = true;
		}
	}
}
#include <wipch.h>
#include "EnemySystem.h"
#include <Wiwa/core/Input.h>
namespace Wiwa
{
	EnemySystem::EnemySystem()
	{
		m_Enemy = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		init = false;
	}
	EnemySystem::~EnemySystem()
	{
	}
	void EnemySystem::OnAwake()
	{

	}
	void EnemySystem::OnInit()
	{
		m_Enemy = GetComponentIterator<Enemy>();
		init = true;
	}
	void EnemySystem::OnUpdate()
	{
		if (!init)
		{
			OnInit();
			init = true;
		}

		/*Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Updating enemy --> {}", e_name);*/

		if (Wiwa::Input::IsKeyPressed(Wiwa::Key::M))
		{
			Enemy* enemy = GetComponentByIterator<Enemy>(m_Enemy);
			enemy->hasFinished = true;
		}
	}
	void EnemySystem::OnDestroy()
	{
	}
}
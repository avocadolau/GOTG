#include <wipch.h>
#include "GamePoolingManager.h"
#include <Wiwa/utilities/EntityPool.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	GamePoolingManager::GamePoolingManager()
	{
		s_PhalanxMeleePool = new EntityPool(1, 13, "assets\\enemy\\prefabs\\melee_phalanx.wiprefab");
		s_PhalanxRangedPool = new EntityPool(2, 13, "assets\\enemy\\prefabs\\ranged_phalanx.wiprefab");
		s_SimpleBulletsPool = new EntityPool(3, 1, "assets\\enemy\\simple_bullet\\simple_bullet.wiprefab");
		s_SentinelPool = new EntityPool(4, 13, "assets\\enemy\\prefabs\\melee_phalanx.wiprefab");

	}

	GamePoolingManager::~GamePoolingManager()
	{
		delete s_PhalanxMeleePool;
		delete s_PhalanxRangedPool;
		delete s_SimpleBulletsPool;
		delete s_SentinelPool;
	}

	void GamePoolingManager::SetScene(Scene* scene)
	{
		s_PhalanxMeleePool->SetScene(scene);
		s_PhalanxRangedPool->SetScene(scene);
		s_SimpleBulletsPool->SetScene(scene);
		s_SentinelPool->SetScene(scene);
	}

	void GamePoolingManager::LoadPool(int type, Scene* scene)
	{
		switch (type)
		{
		case 1:
			LoadPhalanxMeleePool(scene);
			break;
		case 2:
			LoadPhalanxRangedPool(scene);
			break;
		case 3:
			LoadSimpleBulletPool(scene);
			break;
		case 4:
			LoadSentinelPool(scene);
			break;
		default:
			break;
		}
	}
	void GamePoolingManager::UnloadPool(int type)
	{
		switch (type)
		{
		case 1:
			UnloadPhalanxMeleePool();
			break;
		case 2:
			UnloadPhalanxRangedPool();
			break;
		case 3:
			UnloadSimpleBulletPool();
			break;
		case 4:
			UnloadSentinelPool();
			break;
		default:
			break;
		}
	}
	void GamePoolingManager::LoadPhalanxMeleePool(Scene* scene)
	{
		s_PhalanxMeleePool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_PhalanxMeleePool->getMaxSize());
		for (int i = 0; i < s_PhalanxMeleePool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_PhalanxMeleePool->getPath());
		s_PhalanxMeleePool->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadPhalanxMeleePool()
	{
		s_PhalanxMeleePool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadPhalanxRangedPool(Scene* scene)
	{
		s_PhalanxRangedPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_PhalanxRangedPool->getMaxSize());
		for (int i = 0; i < s_PhalanxRangedPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_PhalanxRangedPool->getPath());
		s_PhalanxRangedPool->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadPhalanxRangedPool()
	{
		s_PhalanxRangedPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadSimpleBulletPool(Scene* scene)
	{
		s_SimpleBulletsPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_SimpleBulletsPool->getMaxSize());
		for (int i = 0; i < s_SimpleBulletsPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_SimpleBulletsPool->getPath());
		s_SimpleBulletsPool->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadSimpleBulletPool()
	{
		s_SimpleBulletsPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadSentinelPool(Scene* scene)
	{
		s_SentinelPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_SentinelPool->getMaxSize());
		for (int i = 0; i < s_SentinelPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_SentinelPool->getPath());
		s_SentinelPool->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadSentinelPool()
	{
		s_SentinelPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadAllPools(Scene* scene)
	{
		LoadPhalanxMeleePool(scene);
		LoadPhalanxRangedPool(scene);
		LoadSimpleBulletPool(scene);
		LoadSentinelPool(scene);
	}
	void GamePoolingManager::UnloadAllPools()
	{
		UnloadPhalanxMeleePool();
		UnloadPhalanxRangedPool();
		UnloadSimpleBulletPool();
		UnloadSentinelPool();
	}
}
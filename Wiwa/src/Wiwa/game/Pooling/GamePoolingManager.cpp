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
		s_SentinelPool = new EntityPool(3, 13, "assets\\enemy\\prefabs\\test_sentinel.wiprefab");
		s_SimpleBulletsPool = new EntityPool(4, 1, "assets\\enemy\\simple_bullet\\simple_bullet.wiprefab");
		s_SentinelExplosion = new EntityPool(5, 1, "assets\\enemy\\explosions\\test_explosion_3.wiprefab");
	}

	GamePoolingManager::~GamePoolingManager()
	{
		delete s_PhalanxMeleePool;
		delete s_PhalanxRangedPool;
		delete s_SentinelPool;
		delete s_SimpleBulletsPool;
		delete s_SentinelExplosion;	
	}

	void GamePoolingManager::SetScene(Scene* scene)
	{
		s_PhalanxMeleePool->SetScene(scene);
		s_PhalanxRangedPool->SetScene(scene);
		s_SentinelPool->SetScene(scene);
		s_SimpleBulletsPool->SetScene(scene);
		s_SentinelExplosion->SetScene(scene);
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
			LoadSentinelPool(scene);
			break;
		case 4:
			LoadSimpleBulletPool(scene);
			break;
		case 5:
			LoadSentinelExplosionPool(scene);
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
			UnloadSentinelPool();
			break;
		case 4:
			UnloadSimpleBulletPool();
			break;
		case 5:
			UnloadSentinelExplosionPool();
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

	void GamePoolingManager::LoadSentinelExplosionPool(Scene* scene)
	{
		s_SentinelExplosion->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_SentinelExplosion->getMaxSize());
		for (int i = 0; i < s_SentinelExplosion->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_SentinelExplosion->getPath());
		s_SentinelExplosion->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadSentinelExplosionPool()
	{
		s_SentinelExplosion->ReleaseAllPools();
	}

	void GamePoolingManager::LoadAllPools(Scene* scene)
	{
		LoadPhalanxMeleePool(scene);
		LoadPhalanxRangedPool(scene);
		LoadSentinelPool(scene);
		LoadSimpleBulletPool(scene);
		LoadSentinelExplosionPool(scene);

	}
	void GamePoolingManager::UnloadAllPools()
	{
		UnloadPhalanxMeleePool();
		UnloadPhalanxRangedPool();
		UnloadSentinelPool();
		UnloadSimpleBulletPool();
		UnloadSentinelExplosionPool();
	}
}
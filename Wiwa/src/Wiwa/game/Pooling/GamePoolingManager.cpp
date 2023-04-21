#include <wipch.h>
#include "GamePoolingManager.h"
#include <Wiwa/utilities/EntityPool.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	GamePoolingManager::GamePoolingManager()
	{
		s_PhalanxMeleePool = new EntityPool(1, 13, "assets\\Enemy\\Prefabs\\MeleePhalanx_01.wiprefab");
		s_PhalanxRangedPool = new EntityPool(2, 13, "assets\\Enemy\\Prefabs\\RangedPhalanx_01.wiprefab");
		s_SentinelPool = new EntityPool(3, 3, "assets\\Enemy\\Prefabs\\TestSentinel_01.wiprefab");
		s_SimpleBulletsPool = new EntityPool(4, 1, "assets\\Enemy\\SimpleBullet\\SimpleBullet_01.wiprefabSimpleBullet_01.wiprefab");
		s_SentinelExplosion = new EntityPool(5, 3, "assets\\Enemy\\Explosions\\TestExplosion_01.wiprefab");
		s_BossUltron = new EntityPool(6, 1, "assets\\Enemy\\Prefabs\\melee_phalanx.wiprefab");
		s_ClusterBulletsPool = new EntityPool(7,3, "assets\\Enemy\\cluster_bullet\\cluster_bullet.wiprefab"); 
		s_UltronLaserBeamPool = new EntityPool(8, 1, "assets\\Enemy\\ultron_laser_beam\\ultron_laser_beam.wiprefab");
		s_Subjugator = new EntityPool(9, 3, "assets\\Enemy\\Prefabs\\RangedPhalanx_01.wiprefab");
	}

	GamePoolingManager::~GamePoolingManager()
	{
		delete s_PhalanxMeleePool;
		delete s_PhalanxRangedPool;
		delete s_SentinelPool;	
		delete s_SimpleBulletsPool;
		delete s_SentinelExplosion;
		delete s_BossUltron;
		delete s_ClusterBulletsPool;
		delete s_UltronLaserBeamPool;
		delete s_Subjugator;
	}

	void GamePoolingManager::SetScene(Scene* scene)
	{
		s_PhalanxMeleePool->SetScene(scene);
		s_PhalanxRangedPool->SetScene(scene);
		s_SentinelPool->SetScene(scene);
		s_SimpleBulletsPool->SetScene(scene);
		s_SentinelExplosion->SetScene(scene);
		s_BossUltron->SetScene(scene);
		s_ClusterBulletsPool->SetScene(scene);
		s_UltronLaserBeamPool->SetScene(scene);
		s_Subjugator->SetScene(scene);
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
		case 6:
			LoadBossUltronPool(scene);
			break;
		case 7:
			LoadClusterBulletPool(scene);
			break;
		case 8:
			LoadUltronLaserBeamPool(scene);
			break;
		case 9:
			LoadSubjugatorPool(scene);
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
		case 6:
			UnloadBossUltronPool();
			break;
		case 7:
			UnloadClusterBulletPool();
			break;
		case 8:
			UnloadUltronLaserBeamPool();
			break;
		case 9:
			UnloadSubjugatorPool();
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

	void GamePoolingManager::LoadBossUltronPool(Scene* scene)
	{
		s_BossUltron->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_BossUltron->getMaxSize());
		for (int i = 0; i < s_BossUltron->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_BossUltron->getPath());
		s_BossUltron->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadBossUltronPool()
	{
		s_BossUltron->ReleaseAllPools();
	}

	void GamePoolingManager::LoadClusterBulletPool(Scene* scene)
	{
		s_ClusterBulletsPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_ClusterBulletsPool->getMaxSize());
		for (int i = 0; i < s_ClusterBulletsPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_ClusterBulletsPool->getPath());
		s_ClusterBulletsPool->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadClusterBulletPool()
	{
		s_ClusterBulletsPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadUltronLaserBeamPool(Scene* scene)
	{
		s_UltronLaserBeamPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_UltronLaserBeamPool->getMaxSize());
		for (int i = 0; i < s_UltronLaserBeamPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_UltronLaserBeamPool->getPath());
		s_UltronLaserBeamPool->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadUltronLaserBeamPool()
	{
		s_UltronLaserBeamPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadSubjugatorPool(Scene* scene)
	{
		s_Subjugator->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_Subjugator->getMaxSize());
		for (int i = 0; i < s_Subjugator->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_Subjugator->getPath());
		s_Subjugator->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadSubjugatorPool()
	{
		s_Subjugator->ReleaseAllPools();
	}

	void GamePoolingManager::LoadAllPools(Scene* scene)
	{
		LoadPhalanxMeleePool(scene);
		LoadPhalanxRangedPool(scene);
		LoadSentinelPool(scene);
		LoadSimpleBulletPool(scene);
		LoadSentinelExplosionPool(scene);
		LoadBossUltronPool(scene);
		LoadClusterBulletPool(scene);
		LoadUltronLaserBeamPool(scene);
		LoadSubjugatorPool(scene);
	}
	void GamePoolingManager::UnloadAllPools()
	{
		UnloadPhalanxMeleePool();
		UnloadPhalanxRangedPool();
		UnloadSentinelPool();
		UnloadSimpleBulletPool();
		UnloadSentinelExplosionPool();
		UnloadBossUltronPool();
		UnloadClusterBulletPool();
		UnloadUltronLaserBeamPool();
		UnloadSubjugatorPool();
	}
}
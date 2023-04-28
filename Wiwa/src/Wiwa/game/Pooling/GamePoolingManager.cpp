#include <wipch.h>
#include "GamePoolingManager.h"
#include <Wiwa/utilities/EntityPool.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	GamePoolingManager::GamePoolingManager()
	{
		s_PhalanxMeleePool = new EntityPool(Pool_Type::PHALANX_MELEE, 3, "assets\\Enemy\\PhalanxMelee\\MeleePhalanx_01.wiprefab");
		s_PhalanxRangedPool = new EntityPool(Pool_Type::PHALAN_RANGED, 3, "assets\\Enemy\\PhalanxRanged\\RangedPhalanx_01.wiprefab");
		s_SentinelPool = new EntityPool(Pool_Type::SENTINEL, 3, "assets\\Enemy\\Sentinel\\Sentinel_01.wiprefab");
		s_SimpleBulletsPool = new EntityPool(Pool_Type::SIMPLE_BULLET, 100, "assets\\Enemy\\SimpleBullet\\SimpleBullet_01.wiprefab");
		s_SentinelExplosion = new EntityPool(Pool_Type::SENTINEL_EXPLOSION, 3, "assets\\Enemy\\Explosions\\TestExplosion_01.wiprefab");
		s_BossUltron = new EntityPool(Pool_Type::BOSS_ULTRON, 1, "assets\\Enemy\\Prefabs\\melee_phalanx.wiprefab");
		s_ClusterBulletsPool = new EntityPool(Pool_Type::CLUSTER_BULLET,2, "assets\\Enemy\\ClusterBullet\\ClusterBullet_01.wiprefab");
		s_UltronLaserBeamPool = new EntityPool(Pool_Type::ULTRON_LASER_BEAM, 1, "assets\\Enemy\\UltronLaserBeam\\UltronLaserBeam_01.wiprefab");
		s_Subjugator = new EntityPool(Pool_Type::SUBJUGATOR, 6, "assets\\Enemy\\Prefabs\\Subjugator_01.wiprefab");
		s_ExplosiveBarrel = new EntityPool(Pool_Type::EXPLOSIVE_BARREL, 6, "assets\\Enemy\\Explosions\\ExplosiveBarrelExplosion_01.wiprefab");

		m_HasLoadedAll = false;
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
		delete s_ExplosiveBarrel;
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
		s_ExplosiveBarrel->SetScene(scene);
	}


	void GamePoolingManager::LoadPool(Pool_Type type, Scene* scene)
	{
		switch (type)
		{
		case Pool_Type::PHALANX_MELEE:
			LoadPhalanxMeleePool(scene);
			break;
		case Pool_Type::PHALAN_RANGED:
			LoadPhalanxRangedPool(scene);
			break;
		case Pool_Type::SENTINEL:
			LoadSentinelPool(scene);
			break;
		case Pool_Type::SIMPLE_BULLET:
			LoadSimpleBulletPool(scene);
			break;
		case Pool_Type::SENTINEL_EXPLOSION:
			LoadSentinelExplosionPool(scene);
			break;
		case Pool_Type::BOSS_ULTRON:
			LoadBossUltronPool(scene);
			break;
		case Pool_Type::CLUSTER_BULLET:
			LoadClusterBulletPool(scene);
			break;
		case Pool_Type::ULTRON_LASER_BEAM:
			LoadUltronLaserBeamPool(scene);
			break;
		case Pool_Type::SUBJUGATOR:
			LoadSubjugatorPool(scene);
			break;
		case Pool_Type::EXPLOSIVE_BARREL:
			LoadExplosiveBarrelPool(scene);
			break;
		default:
			break;
		}
	}
	void GamePoolingManager::UnloadPool(Pool_Type type)
	{
		switch (type)
		{
		case Pool_Type::PHALANX_MELEE:
			UnloadPhalanxMeleePool();
			break;
		case Pool_Type::PHALAN_RANGED:
			UnloadPhalanxRangedPool();
			break;
		case Pool_Type::SENTINEL:
			UnloadSentinelPool();
			break;
		case Pool_Type::SIMPLE_BULLET:
			UnloadSimpleBulletPool();
			break;
		case Pool_Type::SENTINEL_EXPLOSION:
			UnloadSentinelExplosionPool();
			break;
		case Pool_Type::BOSS_ULTRON:
			UnloadBossUltronPool();
			break;
		case Pool_Type::CLUSTER_BULLET:
			UnloadClusterBulletPool();
			break;
		case Pool_Type::ULTRON_LASER_BEAM:
			UnloadUltronLaserBeamPool();
			break;
		case Pool_Type::SUBJUGATOR:
			UnloadSubjugatorPool();
			break;
		case Pool_Type::EXPLOSIVE_BARREL:
			UnloadExplosiveBarrelPool();
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
		{
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_ClusterBulletsPool->getPath());
		}
			
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

	void GamePoolingManager::LoadExplosiveBarrelPool(Scene* scene)
	{
		s_ExplosiveBarrel->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_ExplosiveBarrel->getMaxSize());
		for (int i = 0; i < s_ExplosiveBarrel->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_ExplosiveBarrel->getPath());
		s_ExplosiveBarrel->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadExplosiveBarrelPool()
	{
		s_ExplosiveBarrel->ReleaseAllPools();
	}

	void GamePoolingManager::LoadAllPools(Scene* scene)
	{
		if (m_HasLoadedAll)
			return;

		LoadPhalanxMeleePool(scene);
		LoadPhalanxRangedPool(scene);
		LoadSentinelPool(scene);
		LoadSimpleBulletPool(scene);
		LoadSentinelExplosionPool(scene);
		//LoadBossUltronPool(scene);
		LoadClusterBulletPool(scene);
		LoadUltronLaserBeamPool(scene);
		//LoadSubjugatorPool(scene);
		LoadExplosiveBarrelPool(scene);

		m_HasLoadedAll = true;
	}
	void GamePoolingManager::UnloadAllPools()
	{
		m_HasLoadedAll = false;

		UnloadPhalanxMeleePool();
		UnloadPhalanxRangedPool();
		UnloadSentinelPool();
		UnloadSimpleBulletPool();
		UnloadSentinelExplosionPool();
		UnloadBossUltronPool();
		UnloadClusterBulletPool();
		UnloadUltronLaserBeamPool();
		UnloadSubjugatorPool();
		UnloadExplosiveBarrelPool();
	}
}
#include <wipch.h>
#include "GamePoolingManager.h"
#include <Wiwa/utilities/EntityPool.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	GamePoolingManager::GamePoolingManager()
	{
		s_PhalanxMeleePool = new EntityPool(Pool_Type::PHALANX_MELEE_GENERIC, 3, "assets\\Enemy\\PhalanxMelee\\MeleePhalanx_01.wiprefab");
		s_PhalanxRangedPool = new EntityPool(Pool_Type::PHALANX_RANGED_GENERIC, 3, "assets\\Enemy\\PhalanxRanged\\RangedPhalanx_01.wiprefab");
		s_SentinelPool = new EntityPool(Pool_Type::SENTINEL, 3, "assets\\Enemy\\Sentinel\\sk_sentinel01a.wiprefab");
		s_SimpleBulletsPool = new EntityPool(Pool_Type::SIMPLE_BULLET, 100, "assets\\Enemy\\SimpleBullet\\SimpleBullet_01.wiprefab");
		s_SentinelExplosion = new EntityPool(Pool_Type::SENTINEL_EXPLOSION, 3, "assets\\Enemy\\Explosions\\SentinelExplosion_01.wiprefab");
		s_BossUltron = new EntityPool(Pool_Type::BOSS_ULTRON, 1, "assets\\Enemy\\Prefabs\\melee_phalanx.wiprefab");
		s_ClusterBulletsPool = new EntityPool(Pool_Type::CLUSTER_BULLET,8, "assets\\Enemy\\ClusterBullet\\ClusterBullet_01.wiprefab");
		s_UltronLaserBeamPool = new EntityPool(Pool_Type::ULTRON_LASER_BEAM, 1, "assets\\Enemy\\UltronLaserBeam\\UltronLaserBeam_01.wiprefab");
		s_Subjugator = new EntityPool(Pool_Type::SUBJUGATOR, 6, "assets\\Enemy\\Subjugator\\sk_subjugator01.wiprefab");
		s_ExplosiveBarrel = new EntityPool(Pool_Type::EXPLOSIVE_BARREL, 6, "assets\\Enemy\\Explosions\\ExplosiveBarrelExplosion_01.wiprefab");
		s_StarLordBullets = new EntityPool(Pool_Type::STARLORD_BULLET, 25, "assets\\Prefabs\\Bullet\\P_StarLordBullet.wiprefab");
		s_RocketBullets = new EntityPool(Pool_Type::ROCKET_BULLET, 2, "assets\\Prefabs\\Bullet\\P_StarLordBullet.wiprefab");
		s_UltronSecondDashPool = new EntityPool(Pool_Type::ULTRON_SECOND_DASH, 1, "assets\\Enemy\\DashEffect\\DashEffect_01.wiprefab");
		s_RainProjectilePool = new EntityPool(Pool_Type::RAIN_PROJECTILE, 80, "assets\\Enemy\\RainProjectile\\RainProjectile_01.wiprefab");
		s_SmashExplosionPool = new EntityPool(Pool_Type::ULTRON_SMASH_EXPLOSION, 12, "assets\\Enemy\\Explosions\\SmashExplosion_01.wiprefab");
		s_ZigZagBulletPool = new EntityPool(Pool_Type::ZIG_ZAG_BULLET, 40, "assets\\Enemy\\ZigZagBullet\\ZigZagBullet_01.wiprefab");

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
		delete s_UltronSecondDashPool;
		delete s_RainProjectilePool;
		delete s_SmashExplosionPool;
		delete s_ZigZagBulletPool;
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
		s_UltronSecondDashPool->SetScene(scene);
		s_RainProjectilePool->SetScene(scene);
		s_SmashExplosionPool->SetScene(scene);
		s_ZigZagBulletPool->SetScene(scene);
	}


	void GamePoolingManager::LoadPool(Pool_Type type, Scene* scene)
	{
		switch (type)
		{
		case Pool_Type::PHALANX_MELEE_GENERIC:
			LoadPhalanxMeleePool(scene);
			break;
		case Pool_Type::PHALANX_RANGED_GENERIC:
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
		case Pool_Type::STARLORD_BULLET:
			LoadStarLordBulletsPool(scene);
			break;
		case Pool_Type::ROCKET_BULLET:
			LoadRocketBulletsPool(scene);
			break;
		case Pool_Type::ULTRON_SECOND_DASH:
			LoadSecondDashPool(scene);
			break;
		case Pool_Type::RAIN_PROJECTILE:
			LoadRainProjectilePool(scene);
			break;
		case Pool_Type::ULTRON_SMASH_EXPLOSION:
			LoadSmashExplosionPool(scene);
			break;
		case Pool_Type::ZIG_ZAG_BULLET:
			LoadZigZagBulletPool(scene);
			break;
		default:
			break;
		}
	}
	void GamePoolingManager::UnloadPool(Pool_Type type)
	{
		switch (type)
		{
		case Pool_Type::PHALANX_MELEE_GENERIC:
			UnloadPhalanxMeleePool();
			break;
		case Pool_Type::PHALANX_RANGED_GENERIC:
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
		case Pool_Type::STARLORD_BULLET:
			UnloadStarLordBulletsPool();
			break;
		case Pool_Type::ROCKET_BULLET:
			UnloadRocketBulletsPool();
			break;
		case Pool_Type::ULTRON_SECOND_DASH:
			UnloadSecondDashPool();
			break;
		case Pool_Type::RAIN_PROJECTILE:
			UnloadRainProjectilePool();
			break;
		case Pool_Type::ULTRON_SMASH_EXPLOSION:
			UnloadSmashExplosionPool();
			break;
		case Pool_Type::ZIG_ZAG_BULLET:
			UnloadZigZagBulletPool();
			break;
		default:
			break;
		}
	}
	void GamePoolingManager::LoadPhalanxMeleePool(Scene* scene)
	{
		if (s_PhalanxMeleePool->Loaded)
			return;

		s_PhalanxMeleePool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_PhalanxMeleePool->getMaxSize());
		int i = 0;
		while (i < s_PhalanxMeleePool->getMaxSize())
		{
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_PhalanxMeleePool->getPath());

			i++;
			if (i >= s_PhalanxMeleePool->getMaxSize())
				break;

			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab("assets\\Enemy\\PhalanxMelee\\MeleePhalanx_01a.wiprefab");

			i++;
			if (i >= s_PhalanxMeleePool->getMaxSize())
				break;

			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab("assets\\Enemy\\PhalanxMelee\\MeleePhalanx_01b.wiprefab");

			i++;
			if (i >= s_PhalanxMeleePool->getMaxSize())
				break;
		}
	/*	for (int i = 0; i < s_PhalanxMeleePool->getMaxSize(); i++)
		{
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_PhalanxMeleePool->getPath());

		}*/
		s_PhalanxMeleePool->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadPhalanxMeleePool()
	{
		s_PhalanxMeleePool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadPhalanxRangedPool(Scene* scene)
	{
		if (s_PhalanxRangedPool->Loaded)
			return;

		s_PhalanxRangedPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_PhalanxRangedPool->getMaxSize());
		int i = 0;
		while (i < s_PhalanxRangedPool->getMaxSize())
		{
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_PhalanxRangedPool->getPath());

			i++;
			if (i >= s_PhalanxRangedPool->getMaxSize())
				break;

			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab("assets\\Enemy\\PhalanxRanged\\RangedPhalanx_01a.wiprefab");

			i++;
			if (i >= s_PhalanxRangedPool->getMaxSize())
				break;

			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab("assets\\Enemy\\PhalanxRanged\\RangedPhalanx_01b.wiprefab");

			i++;
			if (i >= s_PhalanxRangedPool->getMaxSize())
				break;
		}
		/*for (int i = 0; i < s_PhalanxRangedPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_PhalanxRangedPool->getPath());*/
		s_PhalanxRangedPool->IncreasePoolSize(meleeEnemyIds);
	}
	void GamePoolingManager::UnloadPhalanxRangedPool()
	{
		s_PhalanxRangedPool->ReleaseAllPools();
	}
	void GamePoolingManager::LoadSentinelPool(Scene* scene)
	{
		if (s_SentinelPool->Loaded)
			return;

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
		if (s_SimpleBulletsPool->Loaded)
			return;

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
		if (s_SentinelExplosion->Loaded)
			return;

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
		if (s_BossUltron->Loaded)
			return;

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
		if (s_ClusterBulletsPool->Loaded)
			return;

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
		if (s_UltronLaserBeamPool->Loaded)
			return;

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
		if (s_Subjugator->Loaded)
			return;

		s_Subjugator->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_Subjugator->getMaxSize());
		int i = 0;
		while (i < s_Subjugator->getMaxSize())
		{
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_Subjugator->getPath());

			i++;
			if (i >= s_Subjugator->getMaxSize())
				break;

			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab("assets\\Enemy\\Subjugator\\sk_subjugator01_chief.wiprefab");

			i++;
			if (i >= s_Subjugator->getMaxSize())
				break;
		}
	/*	for (int i = 0; i < s_Subjugator->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_Subjugator->getPath());*/
		s_Subjugator->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadSubjugatorPool()
	{
		s_Subjugator->ReleaseAllPools();
	}

	void GamePoolingManager::LoadExplosiveBarrelPool(Scene* scene)
	{
		if (s_ExplosiveBarrel->Loaded)
			return;

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

	void GamePoolingManager::LoadStarLordBulletsPool(Scene* scene)
	{
		if (s_StarLordBullets->Loaded)
			return;

		s_StarLordBullets->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_StarLordBullets->getMaxSize());
		for (int i = 0; i < s_StarLordBullets->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_StarLordBullets->getPath());
		s_StarLordBullets->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadStarLordBulletsPool()
	{
		s_StarLordBullets->ReleaseAllPools();
	}

	void GamePoolingManager::LoadRocketBulletsPool(Scene* scene)
	{
		if (s_RocketBullets->Loaded)
			return;

		s_RocketBullets->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_RocketBullets->getMaxSize());
		for (int i = 0; i < s_RocketBullets->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_RocketBullets->getPath());
		s_RocketBullets->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadRocketBulletsPool()
	{
		s_RocketBullets->ReleaseAllPools();
	}

	void GamePoolingManager::LoadSecondDashPool(Scene* scene)
	{
		if (s_UltronSecondDashPool->Loaded)
			return;

		s_UltronSecondDashPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_UltronSecondDashPool->getMaxSize());
		for (int i = 0; i < s_UltronSecondDashPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_UltronSecondDashPool->getPath());
		s_UltronSecondDashPool->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadSecondDashPool()
	{
		s_UltronSecondDashPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadRainProjectilePool(Scene* scene)
	{
		if (s_RainProjectilePool->Loaded)
			return;

		s_RainProjectilePool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_RainProjectilePool->getMaxSize());
		for (int i = 0; i < s_RainProjectilePool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_RainProjectilePool->getPath());
		s_RainProjectilePool->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadRainProjectilePool()
	{
		s_RainProjectilePool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadSmashExplosionPool(Scene* scene)
	{
		if (s_SmashExplosionPool->Loaded)
			return;

		s_SmashExplosionPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_SmashExplosionPool->getMaxSize());
		for (int i = 0; i < s_SmashExplosionPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_SmashExplosionPool->getPath());
		s_SmashExplosionPool->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadSmashExplosionPool()
	{
		s_SmashExplosionPool->ReleaseAllPools();
	}

	void GamePoolingManager::LoadZigZagBulletPool(Scene* scene)
	{
		if (s_ZigZagBulletPool->Loaded)
			return;

		s_ZigZagBulletPool->SetScene(scene);
		std::vector<EntityId> meleeEnemyIds(s_ZigZagBulletPool->getMaxSize());
		for (int i = 0; i < s_ZigZagBulletPool->getMaxSize(); i++)
			meleeEnemyIds[i] = scene->GetEntityManager().LoadPrefab(s_ZigZagBulletPool->getPath());
		s_ZigZagBulletPool->IncreasePoolSize(meleeEnemyIds);
	}

	void GamePoolingManager::UnloadZigZagBulletPool()
	{
		s_ZigZagBulletPool->ReleaseAllPools();
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
		LoadSubjugatorPool(scene);
		LoadExplosiveBarrelPool(scene);
		LoadStarLordBulletsPool(scene);
		LoadRocketBulletsPool(scene);
		LoadSecondDashPool(scene);
		LoadRainProjectilePool(scene);
		LoadSmashExplosionPool(scene);
		LoadZigZagBulletPool(scene);

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
		//UnloadBossUltronPool();
		UnloadClusterBulletPool();
		UnloadUltronLaserBeamPool();
		UnloadSubjugatorPool();
		UnloadExplosiveBarrelPool();
		UnloadStarLordBulletsPool();
		UnloadRocketBulletsPool();
		UnloadSecondDashPool();
		UnloadRainProjectilePool();
		UnloadSmashExplosionPool();
		UnloadZigZagBulletPool();
	}
}
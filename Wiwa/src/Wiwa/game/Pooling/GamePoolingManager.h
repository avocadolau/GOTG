#pragma once
#include <Wiwa/utilities/EntityPool.h>

namespace Wiwa
{
    class Scene;
    class WI_API GamePoolingManager {
    public:
        GamePoolingManager();
        ~GamePoolingManager();
        EntityPool* s_PhalanxMeleePool;
        EntityPool* s_PhalanxRangedPool;
        EntityPool* s_SentinelPool;
        EntityPool* s_SimpleBulletsPool;
        EntityPool* s_SentinelExplosion;
        EntityPool* s_BossUltron;
        EntityPool* s_ClusterBulletsPool;
        EntityPool* s_UltronLaserBeamPool;
        EntityPool* s_Subjugator;
        EntityPool* s_ExplosiveBarrel;
        EntityPool* s_StarLordBullets;
        EntityPool* s_RocketBullets;
        EntityPool* s_UltronSecondDashPool;
        EntityPool* s_RainProjectilePool;
        EntityPool* s_SmashExplosionPool;
        EntityPool* s_ZigZagBulletPool;

        bool m_HasLoadedAll = false;
        void SetScene(Scene* scene);

        void LoadPool(Pool_Type type, Scene* scene);
        void UnloadPool(Pool_Type type);

        void LoadAllPools(Scene* scene);
        void UnloadAllPools();
    private:

        void LoadPhalanxMeleePool(Scene* scene);
        void UnloadPhalanxMeleePool();

        void LoadPhalanxRangedPool(Scene* scene);
        void UnloadPhalanxRangedPool();

        void LoadSentinelPool(Scene* scene);
        void UnloadSentinelPool();

        void LoadSimpleBulletPool(Scene* scene);
        void UnloadSimpleBulletPool();

        void LoadSentinelExplosionPool(Scene* scene);
        void UnloadSentinelExplosionPool();

        void LoadBossUltronPool(Scene* scene);
        void UnloadBossUltronPool();

        void LoadClusterBulletPool(Scene* scene);
        void UnloadClusterBulletPool();

        void LoadUltronLaserBeamPool(Scene* scene);
        void UnloadUltronLaserBeamPool();

        void LoadSubjugatorPool(Scene* scene);
        void UnloadSubjugatorPool();

        void LoadExplosiveBarrelPool(Scene* scene);
        void UnloadExplosiveBarrelPool();

        void LoadStarLordBulletsPool(Scene* scene);
        void UnloadStarLordBulletsPool();

        void LoadRocketBulletsPool(Scene* scene);
        void UnloadRocketBulletsPool();

        void LoadSecondDashPool(Scene* scene);
        void UnloadSecondDashPool();

        void LoadRainProjectilePool(Scene* scene);
        void UnloadRainProjectilePool();

        void LoadSmashExplosionPool(Scene* scene);
        void UnloadSmashExplosionPool();

        void LoadZigZagBulletPool(Scene* scene);
        void UnloadZigZagBulletPool();
    };
}
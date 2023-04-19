#pragma once

namespace Wiwa
{
    class EntityPool;
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
        
        
        void SetScene(Scene* scene);

        void LoadPool(int type, Scene* scene);
        void UnloadPool(int type);

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
    };
}
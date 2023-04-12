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
        EntityPool* s_SimpleBulletsPool;
        EntityPool* s_SentinelPool;
        
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

        void LoadSimpleBulletPool(Scene* scene);
        void UnloadSimpleBulletPool();

        void LoadSentinelPool(Scene* scene);
        void UnloadSentinelPool();
    };
}
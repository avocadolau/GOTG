#pragma once
#include <queue>
#include <memory>
#include <vector>
#include <string>

typedef size_t EntityId;

namespace Wiwa
{
    enum class Pool_Type
    {
        PHALANX_MELEE,
        PHALAN_RANGED,
        SENTINEL,
        SUBJUGATOR,
        BOSS_ULTRON,

        SIMPLE_BULLET,
        SENTINEL_EXPLOSION,
        CLUSTER_BULLET,
        ULTRON_LASER_BEAM,
    };

    class Scene;
    class WI_API EntityPool {
    private:
        std::queue<EntityId> m_ActiveEntities;
        std::queue<EntityId> m_DisabledEntities;
        Scene* m_Scene;
        std::string m_Path;
        int m_MaxSize;
        Pool_Type m_Type;
    public:
        EntityPool(Pool_Type type, int max_size, const char* path) : m_Type(type), m_MaxSize(max_size), m_Path(path), m_Loaded(false) {};
        ~EntityPool();

        EntityId GetFromPool();

        void ReturnToPool(EntityId entity_id);

        void IncreasePoolSize(const std::vector<EntityId>& new_entities, bool disable = true);

        void ReleaseAllPools();

        void SetScene(Scene* scene);

        inline const char* getPath() { return m_Path.c_str(); };

        inline int getMaxSize() { return m_MaxSize; };

        inline int getCountActive() { return (int)m_ActiveEntities.size(); };

        inline int getCountDisabled() { return (int)m_DisabledEntities.size(); };

        inline Pool_Type getType() { return m_Type; };

        inline  std::queue<EntityId> getDisabledEntities() { return m_DisabledEntities; };

        bool m_Loaded = false;
    };
}


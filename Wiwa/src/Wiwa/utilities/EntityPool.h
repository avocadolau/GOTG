#pragma once
#include <queue>
#include <memory>
#include <vector>
#include <string>

typedef size_t EntityId;

namespace Wiwa
{
    class Scene;
    class WI_API EntityPool {
    private:
        std::queue<EntityId> m_ActiveEntities;
        std::queue<EntityId> m_DisabledEntities;
        Scene* m_Scene;
        std::string m_Path;
        int m_MaxSize;
        int m_Type;
    public:
        EntityPool(int type, int max_size, const char* path) : m_Type(type), m_MaxSize(max_size), m_Path(path), m_Loaded(false) {};
        ~EntityPool();

        EntityId GetFromPool();

        void ReturnToPool(EntityId entity_id);

        void IncreasePoolSize(const std::vector<EntityId>& new_entities, bool disable = true);

        void ReleaseAllPools();

        void SetScene(Scene* scene);

        inline const char* getPath() { return m_Path.c_str(); };

        inline int getMaxSize() { return m_MaxSize; };

        inline int getType() { return m_Type; };

        bool m_Loaded = false;
    };
}


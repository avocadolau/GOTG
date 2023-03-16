#include <wipch.h>
//#include "PlayerSpawnerSystem.h"
//#include "../CharacterSystem.h"
//#include "../../PhysicsSystem.h"
//namespace Wiwa
//{
//    PlayerSpawnerSystem::PlayerSpawnerSystem()
//    {
//        m_CharacterData = { WI_INVALID_INDEX, WI_INVALID_INDEX };
//        m_Player = { WI_INVALID_INDEX, WI_INVALID_INDEX };
//    }
//
//    PlayerSpawnerSystem::~PlayerSpawnerSystem()
//    {
//    }
//
//    void PlayerSpawnerSystem::OnAwake()
//    {
//    }
//
//    void PlayerSpawnerSystem::OnInit()
//    {
//        GameStateManager::StartNewRoom();
//
//        m_CharacterData = GetComponentIterator<Character>();
//        Character characterData = *GetComponentByIterator<Character>(m_CharacterData);
//        CreatePlayer(characterData);
//    }
//
//    void PlayerSpawnerSystem::OnUpdate()
//    {
//    }
//
//    void PlayerSpawnerSystem::OnDestroy()
//    {
//        GameStateManager::EndCurrentRoom();
//    }
//
//    void PlayerSpawnerSystem::CreatePlayer(Character stats)
//    {
//        // Create an empty entity
//        Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
//        m_PlayerId = entityManager.CreateEntity("Player");
//
//        Transform3D playerTransform = *entityManager.GetComponent<Transform3D>(m_EntityId);;
//        entityManager.AddComponent<Transform3D>(m_PlayerId, playerTransform);
//
//        // Create a character component and character system
//        entityManager.AddComponent<Character>(m_PlayerId, stats);
//        entityManager.ApplySystem<Wiwa::CharacterSystem>(m_PlayerId);
//
//        CharacterController stats_;
//        stats_.velocity = 10.0f;
//        stats_.camYOffset = 0.0f;
//        stats_.camXOffset = 0.0f;
//        stats_.camZOffset = 0.0f;
//        stats_.camXAngle = 0.0f;
//        stats_.camYAngle = 0.0f;
//        stats_.camZAngle = 0.0f;
//        stats_.fireInterval = 0.3f;
//        stats_.fireTimer = 0.0f;
//        stats_.bulletLifeTime = 2.5f;
//        stats_.bulletSpeed = 30.0f;
//        stats_.bulletScale = 0.0f;
//        stats_.bulletTag = 2;
//        stats_.collisionTag1 = 4;
//
//        ColliderCapsule capsule;
//        capsule.height = 1;
//        capsule.radius = 1;
//
//        Rigidbody rigidBody;
//        rigidBody.positionOffset = { 0,0,0 };
//        rigidBody.scalingOffset = { 1,1,1 };
//        rigidBody.isTrigger = false;
//        rigidBody.isStatic = false;
//        rigidBody.doContinuousCollision = false;
//        rigidBody.selfTag = 1;
//        rigidBody.filterBits |= 1 << 3;
//        rigidBody.filterBits |= 1 << 4;
//
//        entityManager.AddComponent<ColliderCapsule>(m_PlayerId, capsule);
//        entityManager.AddComponent<Rigidbody>(m_PlayerId, rigidBody);
//        entityManager.ApplySystem<PhysicsSystem>(m_PlayerId);
//
//        entityManager.AddComponent<Wiwa::CharacterController>(m_PlayerId, stats_);
//
//        SystemHash sysHash = FNV1A_HASH("CharacterControllerSystem");
//        entityManager.ApplySystem(m_PlayerId, sysHash);
//        const std::vector<SystemHash>& system_list = entityManager.GetEntitySystemHashes(m_PlayerId);
//        bool itHasHash = false;
//        for (int i = 0; i < system_list.size(); i++)
//        {
//            if (system_list[i] == sysHash)
//                itHasHash = true;
//        }
//
//        if (itHasHash == true)
//        {
//            WI_INFO("It works");
//        }
//        else
//            WI_INFO("It doesn't work");
//        // Create a player component and player system
//       /* entityManager.AddComponent<Character>(m_PlayerId, stats);
//        entityManager.ApplySystem<Wiwa::CharacterSystem>(m_PlayerId);*/
//
//        // Save the player
//        //m_Player = GetComponentIterator<>(m_PlayerId);
//    }
//}
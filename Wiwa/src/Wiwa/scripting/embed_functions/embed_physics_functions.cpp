#include "wipch.h"
#include "embed_physics_functions.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/physics/PhysicsManager.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

void SetLinearVelocity(size_t id, glm::vec3 velocity)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
        
    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    physicsManager.SetVelocity(obj, velocity);
}

bool AddBodyToLog(size_t id)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    return physicsManager.AddBodyToLog(obj);
}

bool RemoveBodyFromLog(size_t id)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    return physicsManager.RemoveBodyFromLog(obj);
}
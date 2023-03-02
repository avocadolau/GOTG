#include "wipch.h"
#include "embed_physics_functions.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/physics/PhysicsManager.h"

void SetLinearVelocity(size_t id, glm::vec3 velocity)
{
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    physicsManager.SetVelocity(physicsManager.FindByEntityId(id), velocity);
}

bool AddBodyToLog(size_t id)
{
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    return physicsManager.AddBodyToLog(physicsManager.FindByEntityId(id));
}

bool RemoveBodyFromLog(size_t id)
{
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    return physicsManager.RemoveBodyFromLog(physicsManager.FindByEntityId(id));
}

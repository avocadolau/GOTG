#include "wipch.h"
#include "embed_physics_functions.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/physics/PhysicsManager.h"

bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
{
    Wiwa::PhysicsManager &physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    return physicsManager.AddBodyCube(id, cube, transform, rigid_body);
}
void SetLinearVelocity(size_t id, glm::vec3 velocity)
{
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    physicsManager.SetLinearVelocity(physicsManager.FindByEntityId(id), velocity);
}

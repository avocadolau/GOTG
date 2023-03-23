#include "wipch.h"
#include "embed_physics_functions.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/physics/PhysicsManager.h"
#include <Wiwa/scripting/ScriptEngine.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <mono/metadata/object.h>

void SetLinearVelocity(size_t id, glm::vec3 velocity)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
        
    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    physicsManager.SetVelocity(obj, velocity);
}

glm::vec3 GetLinearVelocity(size_t id)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    return glm::vec3(obj->velocity.x(), obj->velocity.y(), obj->velocity.z());
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
    return physicsManager.RemoveBodyFromLog(physicsManager.FindByEntityId(id));
}

MonoString* GetEntityTagString(size_t id)
{
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    int selfTag = em.GetComponent<Wiwa::CollisionBody>(id)->selfTag;
    const char* str = physicsManager.GetFilterTag(selfTag);
    return 	Wiwa::ScriptEngine::CreateString(str);
}

int GetEntityTagBits(size_t id)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    int selfTag = em.GetComponent<Wiwa::CollisionBody>(id)->selfTag;
    return selfTag;
}

int GetTagBitsByString(MonoString* str)
{
    char* name_p = mono_string_to_utf8(str);
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    return physicsManager.GetFilterTag(name_p);
}

int RayCastDistanceWalls(glm::vec3 from, glm::vec3 to)
{
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
    return physicsManager.RayTestWalls(btVector3(from.x, from.y, from.z), btVector3(to.x, to.y, to.z));
}

void SetTrigger(size_t id, bool value)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    physicsManager.SetTrigger(obj, value);
}

void ChangeCollisionTags(size_t id)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
    Wiwa::PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

    Wiwa::CollisionBody* cb = em.GetComponent<Wiwa::CollisionBody>(id);
    Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(id)->getBody();
    int bits = 0;
    bits |= 1 << cb->selfTag;
    physicsManager.ChangeCollisionTags(obj, bits, cb->filterBits);

    obj->selfTagStr = physicsManager.GetFilterTag(cb->selfTag);
}

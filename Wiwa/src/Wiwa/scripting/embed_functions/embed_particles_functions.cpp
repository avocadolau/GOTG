#include "wipch.h"
#include "embed_particles_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include "Wiwa/ecs/systems/ParticleEmitterExecutor.h"

void ParticleEmitterPlayBatch(size_t entity)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

    Wiwa::ParticleEmitterExecutor* emitterSys = em.GetSystem<Wiwa::ParticleEmitterExecutor>(entity);
    //WI_CORE_INFO("Fired batch");
    emitterSys->EmitBatch(entity);
}

void ParticleEmitterPlayLoop(size_t entity, float delaySec)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

    Wiwa::ParticleEmitterExecutor* emitterSys = em.GetSystem<Wiwa::ParticleEmitterExecutor>(entity);
    //WI_CORE_INFO("Started loop");

    emitterSys->Play(entity, delaySec);
}

void ParticleEmitterStopLoop(size_t entity)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

    Wiwa::ParticleEmitterExecutor* emitterSys = em.GetSystem<Wiwa::ParticleEmitterExecutor>(entity);
    //WI_CORE_INFO("Stopped loop");

    emitterSys->Stop(entity);
}

#include "wipch.h"
#include "embed_particles_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include "Wiwa/ecs/systems/ParticleEmitterExecutor.h"

void EmitterEmitBatch(size_t entity)
{
    Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

    Wiwa::ParticleEmitterExecutor* emitterSys = em.GetSystem<Wiwa::ParticleEmitterExecutor>(entity);

    emitterSys->EmitBatch();
}

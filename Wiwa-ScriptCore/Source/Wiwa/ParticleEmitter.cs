
namespace Wiwa
{
    using EntityId = System.UInt64;
    public class ParticleEmitterManger
    {
        public static void ParticleEmitterPlayBatch(EntityId entity)
        {
            InternalCalls.ParticleEmitterPlayBatch(entity);
        }

        public static void ParticleEmitterPlayLoop(EntityId entity, float delaySec)
        {
            InternalCalls.ParticleEmitterPlayLoop(entity, delaySec);
        }

        public static void ParticleEmitterStopLoop(EntityId entity)
        {
            InternalCalls.ParticleEmitterStopLoop(entity);
        }
    }
}

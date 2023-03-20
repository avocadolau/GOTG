
namespace Wiwa
{
    using EntityId = System.UInt64;
    public class ParticleEmitter
    {
        public static void EmitterEmitBatch(EntityId entity)
        {
            InternalCalls.EmitterEmitBatch(entity);
        }
    }
}

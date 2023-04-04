namespace Wiwa
{
    using EntityId = System.UInt64;
    public class Animator
    {
        public static void Blend(string targetAnimation, bool loop, float blendDuration, EntityId entity)
        {
            InternalCalls.Blend(targetAnimation,loop, blendDuration, entity);
        }

        public static void PlayAnimationName(string name, bool loop, EntityId entity)
        {
            InternalCalls.PlayAnimationName(name, loop, entity);
        }

        public static void PlayAnimation(EntityId entity)
        {
            InternalCalls.PlayAnimation(entity);
        }

        public static void PauseAnimation(EntityId entity)
        {
            InternalCalls.PlayAnimation(entity);
        }

        public static void RestartAnimation(EntityId entity)
        {
            InternalCalls.PlayAnimation(entity);
        }
        public static bool HasFinished(EntityId entity)
        {
            return InternalCalls.HasFinished(entity);
        }
    }
}


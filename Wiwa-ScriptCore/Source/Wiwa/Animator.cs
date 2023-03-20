namespace Wiwa
{
    using EntityId = System.UInt64;
    public class Animator
    {
        public static void PlayAnimationName(string name, EntityId entity)
        {
            InternalCalls.PlayAnimationName(name, entity);
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
    }

}


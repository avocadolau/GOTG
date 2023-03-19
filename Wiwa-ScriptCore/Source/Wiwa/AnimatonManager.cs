
namespace Wiwa
{
    using EntityId = System.UInt64;
    public class AnimatorManager
    {
        public static void PlayAnimationName(string name, EntityId entity)
        {
            InternalCalls.PlayAnimationName(name, entity);
        }
    }

}
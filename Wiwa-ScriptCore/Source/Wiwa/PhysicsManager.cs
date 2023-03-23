namespace Wiwa
{
    using EntityId = System.UInt64;
    public class PhysicsManager
    {
        public static void SetLinearVelocity(EntityId id, Vector3 velocity)
        {
            InternalCalls.SetLinearVelocity(id, velocity);
        }
        public static Vector3 GetLinearVelocity(EntityId id)
        {
            return InternalCalls.GetLinearVelocity(id);
        }
        public static bool AddBodyToLog(EntityId id)
        {
            return InternalCalls.AddBodyToLog(id);
        }

        public static bool RemoveBodyFromLog(EntityId id)
        {
            return InternalCalls.RemoveBodyFromLog(id);
        }
        public static string GetEntityTagString(EntityId id)
        {
            return InternalCalls.GetEntityTagString(id);
        }
        public static int GetEntityTagBits(EntityId id)
        {
            return InternalCalls.GetEntityTagBits(id);
        }

        public static int GetTagBitsByString(string tag)
        {
            return InternalCalls.GetTagBitsByString(tag);
        }

        public static int RayCastDistanceWalls(Vector3 from, Vector3 to)
        {
            return InternalCalls.RayCastDistanceWalls(from, to);
        }

        public static void SetTrigger(EntityId id, bool value)
        {
            InternalCalls.SetTrigger(id, value);
        }

        public static void ChangeCollisionTags(EntityId id, int filter_group, int filter_mask)
        {
            InternalCalls.ChangeCollisionTags(id, filter_group, filter_mask);
        }
    }
}


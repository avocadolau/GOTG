namespace Wiwa
{
    using EntityId = System.UInt64;
    public class AgentAIManager
    {
        public static void SendAIToPosition(EntityId entity_id, Vector3 target_pos)
        {
            InternalCalls.SendAIToPosition(entity_id, target_pos);
        }

        public static void SendAIToPositionById(EntityId entity_id, EntityId target_id)
        {
            InternalCalls.SendAIToPositionById(entity_id, target_id);
        }
    }
}


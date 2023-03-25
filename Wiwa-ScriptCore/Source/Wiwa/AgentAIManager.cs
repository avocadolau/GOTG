namespace Wiwa
{
    using System.Runtime.CompilerServices;
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
        
        public static float DistanceAgentTarget(EntityId entity_id, Vector3 target_pos)
        {
            return InternalCalls.DistanceAgentTarget(entity_id, target_pos);
        }
        
        public static float DistanceAgentTargetById(EntityId entity_id, EntityId target_id)
        {
            return InternalCalls.DistanceAgentTargetById(entity_id, target_id);
        }
        
        public static void StopMoving(EntityId entity_id)
        {
            InternalCalls.StopMoving(entity_id);
        }
        
        public static bool HasPath(EntityId entity_id)
        {
            return InternalCalls.HasPath(entity_id);
        }
    }
}


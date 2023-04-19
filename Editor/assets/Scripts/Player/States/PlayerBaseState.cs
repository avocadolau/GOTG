using System;
using System.Linq;
using Wiwa;
namespace Game
{
    using EntityId = UInt64;
    public abstract class  PlayerBaseState
    {
        public abstract void EnterState(ref PlayerStateMachine stateMachine, EntityId entityId);
        public abstract void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId);
        public abstract void ExitState(ref PlayerStateMachine stateMachine, EntityId entityId);
        public abstract void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2);
      
    }
}

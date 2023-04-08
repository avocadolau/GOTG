using System;
using System.Linq;
using Wiwa;
namespace Game
{
    using EntityId = UInt64;
    public abstract class  PlayerBaseState : PlayerController
    {
        public abstract void EnterState(ref PlayerController enemy, EntityId entityId);
        public abstract void UpdateState(ref PlayerController enemy, EntityId entityId);
        public abstract void ExitState(ref PlayerController enemy, EntityId entityId);
        public abstract void OnCollisionEnter(ref PlayerController enemy, EntityId id1, EntityId id2, string str1, string str2);
        
    }
}

using System;
using System.Linq;
using Wiwa;

namespace Game
{ 
    using EntityId = System.UInt64;
public abstract class MeleePhalanxBaseState
    {

        public abstract void EnterState(ref EnemyMeleePhalanx enemy, EntityId entityId);
        public abstract void UpdateState(ref EnemyMeleePhalanx enemy, EntityId entityId);
        public abstract void ExitState(ref EnemyMeleePhalanx enemy, EntityId entityId);
        public abstract void OnCollisionEnter(ref EnemyMeleePhalanx enemy, EntityId entityId);
    }
}
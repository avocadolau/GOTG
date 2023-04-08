using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerAttack : PlayerBaseState
    {
        private float shootTimer = 0f;
        public override void EnterState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Console.WriteLine("Player attack");
            Animator.PlayAnimationName("shootright", false, stateMachine.GetEntity());
        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
        }
        public override void ExitState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
        }
        public override void OnCollisionEnter( EntityId id1, EntityId id2, string str1, string str2)
        {
        }

      
    }
}
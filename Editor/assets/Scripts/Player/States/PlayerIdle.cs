using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerIdle : PlayerBaseState
    {
        public override void EnterState(ref  PlayerStateMachine stateMachine, EntityId entityId)
        {
            Console.WriteLine("Player idle");
            Animator.PlayAnimationName("idle", true, stateMachine.GetEntity());
        }
        public override void UpdateState(ref  PlayerStateMachine stateMachine, EntityId entityId)
        {
            if (stateMachine.movementInput != Vector3Values.zero)
                stateMachine.SwitchState(ref stateMachine, stateMachine.move);
        }
        public override void ExitState(ref  PlayerStateMachine stateMachine, EntityId entityId)
        {
            Animator.RestartAnimation(stateMachine.GetEntity());
        }
        public override void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {

        }
    }
}
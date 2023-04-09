using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerDash : PlayerBaseState
    {
        private float dashTimer = 0f;
        Vector3 targetPoint;
        Vector3 dashDirection;
        public override void EnterState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Console.WriteLine("Player Dash");
            dashTimer = stateMachine.GetCharacter().DashCooldown;
            if (stateMachine.movementInput == Vector3Values.zero)
            {
                dashDirection = Mathf.CalculateForward(ref stateMachine.GetTransform());
            }
            else
            {
                dashDirection = stateMachine.movementInput;
            }

            targetPoint = Mathf.PointAlongDirection(stateMachine.GetTransform().LocalPosition, dashDirection, stateMachine.GetCharacter().DashDistance);

            Animator.Blend("dash", false,0.1f, stateMachine.GetEntity());

        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            if(Vector3.Distance(targetPoint,stateMachine.GetTransform().LocalPosition) <= 1f )
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.idle);
                return;
            }

            stateMachine.velocity += dashDirection * stateMachine.GetCharacter().DashSpeed;

            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), stateMachine.velocity);
        }
        public override void ExitState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            dashDirection = Vector3Values.zero;

            stateMachine.DashEnable = false;
            stateMachine.cooldownTimer = stateMachine.GetCharacter().DashCooldown;

            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), Vector3Values.zero);
            Animator.RestartAnimation(stateMachine.GetEntity());
        }
        public override void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {

        }
    }
}
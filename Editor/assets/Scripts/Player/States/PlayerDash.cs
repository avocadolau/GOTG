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
        float dashParticleTimer;
        float maxDashTime = 1.5f;// to secure that the dash dosent go wiiii to the infinite
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
            maxDashTime = 1.5f;
        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            if(Vector3.Distance(targetPoint,stateMachine.GetTransform().LocalPosition) <= 1f )
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.idle);
                return;
            }

            maxDashTime -= Time.DeltaTime();

            if (maxDashTime <= 0)
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.idle);
                return;
            }
            stateMachine.velocity += dashDirection * stateMachine.GetCharacter().DashSpeed;

            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), stateMachine.velocity);

            //particles
            dashParticleTimer -= Time.DeltaTime();
            if (dashParticleTimer < 0)
            {
                EntityId pe_dash = stateMachine.GetChildByName("PE_Dash");

                ref Transform3D transform = ref stateMachine.GetComponent<Transform3D>(pe_dash);

                transform.Rotation.y = stateMachine.GetTransform().Rotation.y;

               // ParticleEmitterManger.ParticleEmitterPlayBatch(pe_dash);

                dashParticleTimer = 0.1f; //better if random between 2 values
            }
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
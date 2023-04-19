using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerMove: PlayerBaseState
    {
        private float footstepTimer = 0f;
        private float walkStepTimer = 0.62f;
        private float runStepTimer = 0.458f;
        float stepParticleTimer = 0.0f;
        public override void EnterState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Console.WriteLine("Player move");
            Animator.Blend("running", true, 0.2f, stateMachine.GetEntity());
        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            if (stateMachine.movementInput == Vector3Values.zero)
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.idle);
                return;
            }
            else if (stateMachine.shootInput != Vector3Values.zero || Input.IsMouseKeyDown(1)) // attack
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.attack);
                return;
            }
            else if ((Input.IsKeyDown(KeyCode.LeftShift) || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadLeftBumper))
                     && stateMachine.DashEnable)
            { 
                stateMachine.SwitchState(ref stateMachine, stateMachine.dash);
                return;
            }

            stateMachine.direction = stateMachine.movementInput;

            stateMachine.velocity = stateMachine.movementInput * stateMachine.GetCharacter().Speed;
            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), stateMachine.velocity);

            stateMachine.SetPlayerRotation(ref stateMachine.GetTransform().LocalRotation, stateMachine.movementInput, 1f);

            PlayFootStep(stateMachine.GetEntity());
            SpawnParticle(stateMachine.GetEntity());
        }
        public override void ExitState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Animator.RestartAnimation(stateMachine.GetEntity());
        }
        public override void OnCollisionEnter( EntityId id1, EntityId id2, string str1, string str2)
        {

        }

        void PlayFootStep(EntityId id)
        {
            if (footstepTimer >= runStepTimer)
            {
                footstepTimer = 0;
                Audio.PlaySound("player_walk", id);
            }
        }

        void SpawnParticle(EntityId id)
        {
            stepParticleTimer -= Time.DeltaTime();

            if (stepParticleTimer < 0)
            {
                ParticleEmitterManger.ParticleEmitterPlayBatch(id);

                stepParticleTimer = 0.4f; //better if random between 2 values
            }
        }
    }
}
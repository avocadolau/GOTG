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
            Animator.Blend("idle", true, 0.3f, stateMachine.GetEntity());

            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), Vector3Values.zero);
            stateMachine.SetPlayerRotation(ref stateMachine.GetTransform().LocalRotation, stateMachine.direction, 1f);
        }
        public override void UpdateState(ref  PlayerStateMachine stateMachine, EntityId entityId)
        {
            if (stateMachine.movementInput != Vector3Values.zero) // move
            { 
                stateMachine.SwitchState(ref stateMachine, stateMachine.move);
                return;
            }
            else if (stateMachine.shootInput != Vector3Values.zero || Input.IsMouseKeyDown(1)) // attack
            { 
                stateMachine.SwitchState(ref stateMachine, stateMachine.attack);
                return;
            }
            else if ((Input.IsKeyDown(KeyCode.LeftShift) || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadLeftBumper)) 
                    && stateMachine.DashEnable) // dash
            { 
                stateMachine.SwitchState(ref stateMachine, stateMachine.dash);
                return;
            }          
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
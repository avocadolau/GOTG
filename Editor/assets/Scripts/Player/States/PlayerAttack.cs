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
            Animator.PlayAnimationName("aiming", true, stateMachine.GetEntity());
        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            if (stateMachine.shootInput == Vector3Values.zero && stateMachine.movementInput == Vector3Values.zero)
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.idle);
                return;
            }else if(stateMachine.shootInput == Vector3Values.zero && stateMachine.movementInput != Vector3Values.zero)
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.move);
                return;
            }
            stateMachine.direction = stateMachine.shootInput;
            stateMachine.SetPlayerRotation(ref stateMachine.GetTransform().LocalRotation, stateMachine.shootInput, 1f);
            stateMachine.velocity = stateMachine.movementInput * stateMachine.GetCharacter().Speed;
            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), stateMachine.velocity);
            shootTimer += Time.DeltaTime();
            if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadRigthBumper) || Input.IsKeyDown(KeyCode.Space))
            { 
                Fire(ref stateMachine, stateMachine.shootInput); 
            }
            if (stateMachine.velocity != Vector3Values.zero)
                Animator.Blend("aiming", true, 0.2f, stateMachine.GetEntity());
        }

        public override void ExitState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Animator.RestartAnimation(stateMachine.GetEntity());
        }
        public override void OnCollisionEnter( EntityId id1, EntityId id2, string str1, string str2)
        {
        }
        public void Fire(ref PlayerStateMachine stateMachine, Vector3 shootInput)
        {
            ref StarlordShooter shooter = ref stateMachine.GetComponentByIterator<StarlordShooter>(stateMachine.shooterIt);
            Transform3D spawnPoint;
            //Decide wich hand is going next
            if (shootTimer >= stateMachine.GetCharacter().RateOfFire)
            {
                shootTimer = 0;
                if (shooter.ShootRight)
                {
                    spawnPoint = stateMachine.GetFirePosition("RightPos");
                    Animator.PlayAnimationName("shootright", false, stateMachine.GetEntity());
                }
                else
                {
                    spawnPoint = stateMachine.GetFirePosition("LeftPos"); ;
                    Animator.PlayAnimationName("shootleft", false, stateMachine.GetEntity());
                }
                shooter.ShootRight = !shooter.ShootRight;
                stateMachine.SpawnBullet(spawnPoint, shooter, stateMachine.GetCharacter(), Mathf.CalculateForward(ref spawnPoint));
            }
        }
    }
}
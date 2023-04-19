using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerAttack : PlayerBaseState
    {
        private float shootTimer = 0f;
        Vector2 mousePos;
        public override void EnterState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Console.WriteLine("Player attack");
            Animator.PlayAnimationName("aiming", true, stateMachine.GetEntity());
        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            if ((stateMachine.shootInput == Vector3Values.zero && !Input.IsMouseKeyDown(1)) && stateMachine.movementInput == Vector3Values.zero)
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.idle);
                return;
            }else if((stateMachine.shootInput == Vector3Values.zero && !Input.IsMouseKeyDown(1)) && stateMachine.movementInput != Vector3Values.zero)
            {
                stateMachine.SwitchState(ref stateMachine, stateMachine.move);
                return;
            }

            mousePos.x = Input.GetMouseX();
            mousePos.y = Input.GetMouseY();
            
            if(stateMachine.shootInput == Vector3Values.zero)
            {
                System.UInt64 cam_id = CameraManager.GetActiveCamera();
                //  stateMachine.direction = worldPos - stateMachine.GetTransform().Position;
 

               Vector3 worldPos = CameraManager.ScreenToWorlPosition(cam_id, mousePos, stateMachine.GetTransform().Position.y);
               // Vector2 wnp = CameraManager.GetNormalizedScreenPos(cam_id, mousePos);
               //-x cuz of window origin
                stateMachine.direction.x = -worldPos.x;
                stateMachine.direction.y = worldPos.y;
                stateMachine.direction.z = worldPos.z;
            }
            else
            {
                stateMachine.direction = stateMachine.shootInput;
            }

            stateMachine.SetPlayerRotation(ref stateMachine.GetTransform().LocalRotation, stateMachine.direction, 1f);
            stateMachine.velocity = stateMachine.movementInput * stateMachine.GetCharacter().Speed;
            PhysicsManager.SetLinearVelocity(stateMachine.GetEntity(), stateMachine.velocity);
            shootTimer += Time.DeltaTime();
            if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadRigthBumper) || Input.IsMouseKeyDown(0))
            { 
                Fire(ref stateMachine, stateMachine.shootInput); 
            }
            if (stateMachine.velocity != Vector3Values.zero)
                Animator.PlayAnimationName("running", true, stateMachine.GetEntity());
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
                stateMachine.SpawnBullet(ref spawnPoint, ref shooter, ref stateMachine.GetCharacter(), Mathf.CalculateForward(ref spawnPoint));
            }
        }
    }
}
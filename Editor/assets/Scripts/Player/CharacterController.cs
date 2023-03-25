using System;
using Wiwa;


namespace Game
{
    using EntityId = System.UInt64;
    [Component]
    public struct CharacterController
    {
        public float Velocity;
        public float GamepadDeadZone;
        public float RotationSpeed;

        public float DashCoolDown;
        public float DashDistance;
        public float DashSpeed;

        public float WalkTreshold;
    }
    class CharacterControllerSystem : Behaviour
    {
        private ComponentIterator characterControllerIt;
        private ComponentIterator transformIt;
        private ComponentIterator rigidBodyIt;
        private ComponentIterator shooterIt;

        private float dashTimer = 0f;
        private float dashCurrentVel = 0f;
        private bool isDashing = false;

        private Vector3 lastPos = Vector3Values.zero;

        private bool isShooting = false;
        private float shootTimer = 0f;

        private float footstepTimer = 0f;
        private float walkStepTimer = 0.6f;
        private float runStepTimer = 0.30f;

        private bool isWalking = false;

        void Awake()
        {

            //Setting components
            characterControllerIt = GetComponentIterator<CharacterController>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();
            shooterIt = GetComponentIterator<CharacterShooter>();

            dashTimer = GetComponentByIterator<CharacterController>(characterControllerIt).DashCoolDown;

            // need to get or hardcode step and run timers.
        }

        void Update()
        {
            //Components
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref CharacterController controller = ref GetComponentByIterator<CharacterController>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            Vector3 velocity = Vector3Values.zero;
            Vector3 input = Vector3Values.zero;

            if (!isDashing)
            {
                input = GetMovementInput(ref controller);
                velocity = input * controller.Velocity;
            }

            Dash(ref velocity, input, controller, transform, ref rb);

            PhysicsManager.SetLinearVelocity(m_EntityId, velocity);

            footstepTimer += Time.DeltaTime();

            if (input != Vector3Values.zero)
            {
                SetPlayerRotation(ref transform.LocalRotation, input, controller.RotationSpeed);
                PlayFootStep();
            }
            else
            {
                footstepTimer = 0;
            }

            UpdateAnimation(input, controller);


            Vector3 shootInput = GetShootingInput(ref controller);
            shootTimer += Time.DeltaTime();
            if (shootInput != Vector3Values.zero)
            {
                SetPlayerRotation(ref transform.LocalRotation, shootInput, controller.RotationSpeed);
                Fire();
            }
        }

        Vector3 GetMovementInput(ref CharacterController controller)
        {
            Vector3 input = Vector3Values.zero;

            //     z
            //     |
            //     |
            //-----|----- -x
            //     |
            //     |



            if (Input.IsKeyDown(KeyCode.A))
                input.x += 1;
            if (Input.IsKeyDown(KeyCode.D))
                input.x -= 1;
            if (Input.IsKeyDown(KeyCode.W))
                input.z += 1;
            if (Input.IsKeyDown(KeyCode.S))
                input.z -= 1;

            //This axis need to be inverted
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftX, controller.GamepadDeadZone);
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftY, controller.GamepadDeadZone);

            return input;
        }
        Vector3 GetShootingInput(ref CharacterController controller)
        {
            Vector3 input = Vector3Values.zero;

            //     z
            //     |
            //     |
            //-----|----- -x
            //     |
            //     |



            if (Input.IsKeyDown(KeyCode.Left))
                input.x += 1;
            if (Input.IsKeyDown(KeyCode.Right))
                input.x -= 1;
            if (Input.IsKeyDown(KeyCode.Up))
                input.z += 1;
            if (Input.IsKeyDown(KeyCode.Down))
                input.z -= 1;

            //This axis need to be inverted
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightX, controller.GamepadDeadZone);
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightY, controller.GamepadDeadZone);

            return input;
        }
        void SetPlayerRotation(ref Vector3 currentRotation, Vector3 input, float rotationSpeed)
        {
            float angle = AngleFromVec2(new Vector2(input.x, input.z));


            currentRotation.y = Mathf.LerpAngle(currentRotation.y, angle, rotationSpeed);

            if (currentRotation.y >= 360f)
                currentRotation.y = 0f;
        }
        void UpdateAnimation(Vector3 input, CharacterController controller)
        {
            if (input == Vector3Values.zero)
            {
                Animator.PlayAnimationName("idle", m_EntityId);
                return;
            }

            float mag = input.magnitude;
            Mathf.Clamp01(mag);

            if (mag <= controller.WalkTreshold)
            {
                isWalking = true;

                Animator.PlayAnimationName("walk", m_EntityId);
                return;
            }
            isWalking = false;
            Animator.PlayAnimationName("run", m_EntityId);
        }
        void Dash(ref Vector3 velocity, Vector3 input, CharacterController controller, Transform3D transform, ref CollisionBody cb)
        {
            dashTimer += Time.DeltaTime();

            if (!isDashing && dashTimer >= controller.DashCoolDown &&
            (Input.IsKeyDown(KeyCode.LeftShift) || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadA)))
            {
                isDashing = true;
                lastPos = transform.LocalPosition;
            }

            if (isDashing)
            {
                if (input == Vector3Values.zero)
                {
                    input = Mathf.CalculateForward(ref transform);
                }

                Vector3 targetPoint = Mathf.PointAlongDirection(lastPos, input, controller.DashDistance);
                dashCurrentVel += controller.DashSpeed;
                velocity = input * dashCurrentVel;

                float distance = Vector3.Distance(targetPoint, transform.LocalPosition);

                cb.filterBits &= ~(1 << PhysicsManager.GetTagBitsByString("ENEMY"));
                cb.filterBits &= ~(1 << PhysicsManager.GetTagBitsByString("COLUMN"));
                PhysicsManager.ChangeCollisionTags(m_EntityId);


                if (distance <= 2f)
                {
                    ResetDash();
                }

            }
        }
        void ResetDash()
        {
            ref CollisionBody cb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);
            dashTimer = 0f;
            dashCurrentVel = 0f;
            isDashing = false;
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("ENEMY");
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("COLUMN");
            PhysicsManager.ChangeCollisionTags(m_EntityId);

        }


        void Fire()
        {
            ref CharacterShooter shooter = ref GetComponentByIterator<CharacterShooter>(shooterIt);
            isShooting = true;
            if (shootTimer >= shooter.FireInterval)
            {
                shootTimer = 0f;

                Vector3 spawnPoint;
                //Decide wich hand is going next
                if (shooter.ShootRight)
                    spawnPoint = shooter.RightSpawnPos;
                else
                    spawnPoint = shooter.LeftSpawnPos;

                spawnPoint += GetComponentByIterator<Transform3D>(transformIt).LocalPosition;

                shooter.ShootRight = !shooter.ShootRight;
                SpawnBullet(spawnPoint, shooter);
            }
        }

        void PlayFootStep()
        {
            if (isWalking)
            {
                if (footstepTimer >= walkStepTimer)
                {
                    footstepTimer = 0;
                    Audio.PlaySound("player_walk", m_EntityId);
                }
            }
            else if (footstepTimer >= runStepTimer)
            {
                Console.Write($"{isWalking}"); ;
                footstepTimer = 0;
                Audio.PlaySound("player_walk", m_EntityId);
            }

        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 != m_EntityId)
                return;
            if (str2 == "WALL")
                ResetDash();
        }


        //TODO: Put on Mathf.cs
        public float AngleFromVec2(Vector2 vector)
        {
            return Mathf.Atan2(vector.x, vector.y) * Mathf.Rad2Deg;
        }
        void SpawnBullet(Vector3 position, CharacterShooter shooter)
        {
            float angle = GetComponentByIterator<Transform3D>(transformIt).LocalRotation.y;

            Console.WriteLine($"Angle {angle}");

            Vector3 direction = new Vector3(Mathf.Sin(angle), 0f, Mathf.Cos(angle));

            Console.WriteLine($"Direction {direction.x} {direction.z}");
            EntityId bullet = CreateEntity();

            ref Transform3D bulletTransform = ref GetComponent<Transform3D>(bullet);
            ref BulletComponent bulletComp = ref AddComponent<BulletComponent>(bullet);
            ref CollisionBody cb = ref AddComponent<CollisionBody>(bullet);
            ref ColliderSphere cs = ref AddComponent<ColliderSphere>(bullet);


            AddMesh(bullet, "Models/Bullet", "assets/Models/03_mat_addelements.wimaterial");


            bulletTransform.LocalPosition = position;
            bulletTransform.LocalScale = new Vector3(1f, 1f, 1f);

            cs.radius = 1;

            cb.scalingOffset = new Vector3(1f, 1f, 1f);
            cb.isTrigger = true;
            cb.isStatic = false;
            cb.doContinuousCollision = false;
            cb.selfTag = 3;
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("ENEMY");
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("WALL");
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("COLUMN");

            bulletComp.Velocity = shooter.BulletSpeed;
            bulletComp.LifeTime = shooter.BulletLifeTime;
            bulletComp.Damage = shooter.BulletDamage;
            bulletComp.Direction = direction;

            ApplySystem<MeshRenderer>(bullet);
            ApplySystem<PhysicsSystem>(bullet);
            ApplySystem<BulletController>(bullet);
            ApplySystem<AudioSystem>(bullet);

            Audio.PlaySound("player_shoot", m_EntityId);
        }
    }
}
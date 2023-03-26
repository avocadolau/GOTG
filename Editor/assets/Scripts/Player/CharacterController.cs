using System;
using Wiwa;


namespace Game
{
    using EntityId = UInt64;
    class CharacterControllerSystem : Behaviour
    {
        private ComponentIterator characterControllerIt;
        private ComponentIterator transformIt;
        private ComponentIterator rigidBodyIt;
        private ComponentIterator shooterIt;
        private ComponentIterator leftPosIt;
        private ComponentIterator rightPosIt;

        private float dashTimer = 0f;
        private float dashCurrentVel = 0f;
        private bool isDashing = false;

        private Vector3 lastPos = Vector3Values.zero;

        private bool isShooting = false;
        private float shootTimer = 0f;

        private float footstepTimer = 0f;
        private float walkStepTimer = 0.62f;
        private float runStepTimer = 0.458f;

        private bool isWalking = false;
        private bool isAiming = false;

        private float dieTimer = 5f;

        Vector3 lastDir = Vector3Values.zero;

        void Awake()
        {
            GameState.SetPlayer(m_EntityId, m_Scene);
            GameState.LoadPlayerProgression();
            //Setting components
            characterControllerIt = GetComponentIterator<Character>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();
            shooterIt = GetComponentIterator<StarlordShooter>();
            dashTimer = GetComponentByIterator<Character>(characterControllerIt).DashCooldown;




            // need to get or hardcode step and run timers. walkthreshol
        }

        void Update()
        {

            //Components
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref Character controller = ref GetComponentByIterator<Character>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            Vector3 velocity = Vector3Values.zero;
            Vector3 input = Vector3Values.zero;

            if (!isDashing)
            {
                input = GetMovementInput(ref controller);
                velocity = input * controller.Speed;
            }

            Dash(ref velocity, input, controller, transform, ref rb);
            //ChangePosition
            PhysicsManager.SetLinearVelocity(m_EntityId, velocity);

            footstepTimer += Time.DeltaTime();

            if (input != Vector3Values.zero && !isDashing)
            {
                lastDir = input;
                SetPlayerRotation(ref transform.LocalRotation, input, 1f);
                PlayFootStep();
            }
            else
            {
                footstepTimer = 0;
            }

            UpdateAnimation(input, controller);


            Vector3 shootInput = GetShootingInput(ref controller);
            shootTimer += Time.DeltaTime();
            isAiming = false;
            //rotates the character if aiming
            if (shootInput != Vector3Values.zero && !isDashing)
            {
                SetPlayerRotation(ref transform.LocalRotation, shootInput, 1f);
                lastDir = shootInput;
                isAiming = true;

                Animator.PlayAnimationName("aiming", m_EntityId);
                //if (isWalking)
                //{
                //    Animator.Blend("aiming", 1f, m_EntityId);
                //}
            }
            //FIRES the weapon, if the player is not aiming shoots the bullet is shot to the direction the character is looking
            if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadRigthBumper) || Input.IsKeyDown(KeyCode.Space))
            {
                if (shootInput == Vector3Values.zero)
                {
                    if (lastDir != Vector3Values.zero)
                    {
                        Fire(lastDir);
                    }
                    else Fire(new Vector3(0, 0, 1));
                }
                else
                {
                    Fire(shootInput);

                }
            }
            //Debug remove once tested
            if (Input.IsKeyDown(KeyCode.F1))
            {
                controller.Health = 0;
            }

            if (controller.Health <= 0)
            {
                dieTimer -= Time.DeltaTime();
                if (dieTimer <= 0)
                {
                    GameState.Die();
                }
            }

        }

        Vector3 GetMovementInput(ref Character controller)
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
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftX, GameState.GetControllerDeadzone());
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftY, GameState.GetControllerDeadzone());

            return input;
        }
        Vector3 GetShootingInput(ref Character controller)
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
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightX, GameState.GetControllerDeadzone());
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightY, GameState.GetControllerDeadzone());

            return input;
        }
        void SetPlayerRotation(ref Vector3 currentRotation, Vector3 input, float rotationSpeed)
        {
            float angle = AngleFromVec2(new Vector2(input.x, input.z));


            currentRotation.y = Mathf.LerpAngle(currentRotation.y, angle, rotationSpeed);

            if (currentRotation.y >= 360f)
                currentRotation.y = 0f;
        }
        void UpdateAnimation(Vector3 input, Character controller)
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
            Animator.PlayAnimationName("running", m_EntityId);
        }
        void Dash(ref Vector3 velocity, Vector3 input, Character controller, Transform3D transform, ref CollisionBody cb)
        {
            dashTimer += Time.DeltaTime();

            if (!isDashing && dashTimer >= controller.DashCooldown &&
            (Input.IsKeyDown(KeyCode.LeftShift) || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadLeftBumper)))
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


        void Fire(Vector3 shootInput)
        {
            EntityId leftPos = GetChildByName("LeftPos");
            EntityId rigthPos = GetChildByName("RightPos");
            leftPosIt = GetComponentIterator<Transform3D>(leftPos);
            rightPosIt = GetComponentIterator<Transform3D>(rigthPos);
            ref Character character = ref GetComponentByIterator<Character>(characterControllerIt);
            ref StarlordShooter shooter = ref GetComponentByIterator<StarlordShooter>(shooterIt);
            ref Transform3D left = ref GetComponentByIterator<Transform3D>(leftPosIt);
            ref Transform3D right = ref GetComponentByIterator<Transform3D>(rightPosIt);
            isShooting = true;
            if (shootTimer >= character.RateOfFire)
            {
                shootTimer = 0f;

                Transform3D spawnPoint;
                //Decide wich hand is going next
                if (shooter.ShootRight)
                {
                    spawnPoint = right;
                    Animator.PlayAnimationName("shoot_right", m_EntityId);
                }
                else
                {
                    spawnPoint = left;
                    Animator.PlayAnimationName("shoot_left", m_EntityId);
                }


                shooter.ShootRight = !shooter.ShootRight;
                SpawnBullet(spawnPoint, shooter, character, Mathf.CalculateForward(ref spawnPoint));
            }
        }

        void PlayFootStep()
        {
            // IF WALKING animation returns, activate this
            //Console.WriteLine(footstepTimer);
            //if (isWalking)
            //{
            //    if (footstepTimer >= walkStepTimer)
            //    {
            //        footstepTimer = 0;
            //        Audio.PlaySound("player_walk", m_EntityId);
            //    }
            //}
            //else if (footstepTimer >= runStepTimer)
            //{
            //    footstepTimer = 0;
            //    Audio.PlaySound("player_walk", m_EntityId);
            //}
            if (footstepTimer >= runStepTimer)
            {
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
        void SpawnBullet(Transform3D transform, StarlordShooter shooter, Character character, Vector3 bullDir)
        {
            EntityId bullet = CreateEntityNamed("Bullet");
            ref Transform3D playerTransform = ref GetComponent<Transform3D>();
            ref Transform3D bulletTransform = ref GetComponent<Transform3D>(bullet);
            ref BulletComponent bulletComp = ref AddComponent<BulletComponent>(bullet);
            ref CollisionBody cb = ref AddComponent<CollisionBody>(bullet);
            ref ColliderSphere cs = ref AddComponent<ColliderSphere>(bullet);

            AddMesh(bullet, "Models/Cube", "assets/Models/03_mat_addelements.wimaterial");

            bulletTransform.LocalPosition = transform.worldMatrix.GetPosition();
            bulletTransform.LocalRotation = transform.Rotation;
            bulletTransform.LocalScale = transform.Scale;

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
            bulletComp.Damage = character.Damage;
            bulletComp.Direction = bullDir;

            ApplySystem<MeshRenderer>(bullet);
            ApplySystem<PhysicsSystem>(bullet);
            ApplySystem<BulletController>(bullet);
            ApplySystem<AudioSystem>(bullet);

            Audio.PlaySound("player_shoot", m_EntityId);
        }
        Vector3 RotatePointAroundPivot(Vector3 point, Vector3 pivot, Vector3 angles)
        {
            return Quaternion.Euler(angles) * (point - pivot) + pivot;
        }
    }

}
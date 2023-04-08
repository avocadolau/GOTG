using System;
using Wiwa;


namespace Game
{
    using EntityId = UInt64;
    public class CharacterControllerSystem : Behaviour
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
        private int bulletCount = 0;

        private float footstepTimer = 0f;
        private float walkStepTimer = 0.62f;
        private float runStepTimer = 0.458f;
        float stepParticleTimer = 0.0f;
        float dashParticleTimer = 0.0f;

        private bool isWalking = false;
        private bool isAiming = false;
        private float animDur = 0.25f;
        private float animTimer = 0f;

        private float dieTimer = 5f;
        private bool pendingToLoad = true;
        Vector3 lastDir = Vector3Values.zero;
        void Awake()
        {
            //Setting components
            characterControllerIt = GetComponentIterator<Character>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();
            shooterIt = GetComponentIterator<StarlordShooter>();
            dashTimer = GetComponentByIterator<Character>(characterControllerIt).DashCooldown;

            GameState.SetPlayer(m_EntityId, m_Scene);
            GameState.LoadPlayerProgression();
        }
        void Init()
        {
        }
        void Update()
        {

            //Components
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref Character controller = ref GetComponentByIterator<Character>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            //Debug remove once tested
            if (CheckDeath(ref controller))
                return;
            //Debug remove when respective actions added
            CheckParticles(ref controller);

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
                if (animTimer > animDur)
                {
                    Animator.PlayAnimationName("aiming", true, m_EntityId);
                }
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

            if (isWalking)
            {
                stepParticleTimer -= Time.DeltaTime();

                if (stepParticleTimer < 0)
                {
                    ParticleEmitterManger.ParticleEmitterPlayBatch(m_EntityId);

                    stepParticleTimer = 0.4f; //better if random between 2 values


                }
            }
        }

        private bool CheckDeath(ref Character controller)
        {
            if (Input.IsKeyDown(KeyCode.F1))
            {
                controller.Health = 0;
            }

            if (controller.Health <= 0)
            {
                dieTimer -= Time.DeltaTime();

                EntityId pe_death = GetChildByName("PE_Death");
                ParticleEmitterManger.ParticleEmitterPlayBatch(pe_death);

                Animator.PlayAnimationName("death", false, m_EntityId);
                if (dieTimer <= 0)
                    GameState.Die();

                if (Animator.HasFinished(m_EntityId))
                {
                    return true;
                }
            }
            return false;
        }

        bool debugR = false;
        bool tempR = false;
        private void CheckParticles(ref Character controller)
        {
            tempR = debugR;
            if (Input.IsKeyDown(KeyCode.R))
            {
                debugR = true;

            }
            else
            {
                debugR = false;
            }

            //check if input is pressed once
            if (tempR != debugR)
            {
                //if is pressed
                if (debugR)
                {
                    //Console.WriteLine("Presed R");
                    //Hurt player
                    //EntityId pe_hurt = GetChildByName("PE_Hurt");
                    //ParticleEmitterManger.ParticleEmitterPlayBatch(pe_hurt);

                    //EntityId pe_death = GetChildByName("PE_Death");
                    //ParticleEmitterManger.ParticleEmitterPlayBatch(pe_death);

                    EntityId pe_shield = GetChildByName("PE_Shield");
                    ParticleEmitterManger.ParticleEmitterPlayBatch(pe_shield);
                }
                //if is released
                else
                {
                    //Console.WriteLine("Released R");

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
            animTimer += Time.DeltaTime();
            if (isShooting)
            {
                animTimer = 0f;
                isShooting = false;
            }
            isWalking = false;
            if (input == Vector3Values.zero && animTimer > animDur && !isAiming)
            {
                Animator.PlayAnimationName("idle", true, m_EntityId);
                return;
            }
            else if (input != Vector3Values.zero)
            {
                isWalking = true;
                Animator.PlayAnimationName("running", true, m_EntityId);
            }
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

                Animator.PlayAnimationName("dash", false, m_EntityId);
                dashParticleTimer -= Time.DeltaTime();

                //------------------------

                //EntityId pe_dash = GetChildByName("PE_Dash");
                //pman.EmitBatch(pe_hurt);
                //--------------------------------

                if (dashParticleTimer < 0)
                {
                    EntityId pe_dash = GetChildByName("PE_Dash");

                    ParticleEmitterManger.ParticleEmitterPlayBatch(pe_dash);

                    dashParticleTimer = 0.1f; //better if random between 2 values
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
                    Animator.PlayAnimationName("shootright",false, m_EntityId);
                }
                else
                {
                    spawnPoint = left;
                    Animator.PlayAnimationName("shootleft",false, m_EntityId);
                }
                if (isWalking)
                {
                    Animator.Blend("running", true, 1f, m_EntityId);
                }

                shooter.ShootRight = !shooter.ShootRight;
                SpawnBullet(spawnPoint, shooter, character, Mathf.CalculateForward(ref spawnPoint));
            }
        }

        void PlayFootStep()
        {
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
            Console.WriteLine("fired");

            string bulletName = "Bullet";
            bulletCount++;

            EntityId bullet = CreateEntityNamed(bulletName);
            ref Transform3D playerTransform = ref GetComponent<Transform3D>();
            ref Transform3D bulletTransform = ref GetComponent<Transform3D>(bullet);
            ref BulletComponent bulletComp = ref AddComponent<BulletComponent>(bullet);
            ref CollisionBody cb = ref AddComponent<CollisionBody>(bullet);
            ref ColliderSphere cs = ref AddComponent<ColliderSphere>(bullet);

            AddMesh(bullet, "Player/Bullet/PlaneBullet", "Player/Bullet/defaultmaterial.wimaterial");

            bulletTransform.LocalPosition = transform.worldMatrix.GetPosition();
            bulletTransform.LocalRotation = new Vector3(-90f, 0f, playerTransform.LocalRotation.y + 90f);
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
    }

}
using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;

    [Component]
    public struct CharacterController
    {
        public float maxVelocity;
        public float rotationSpeed;
        public float camYOffset;
        public float camXOffset;
        public float camZOffset;

        public float camXAngle;
        public float camYAngle;
        public float camZAngle;

        public float fireInterval;
        public float fireTimer;

        public float bulletLifeTime;
        public float bulletSpeed;
        public float bulletScale;

        public int bulletTag;
        public int collisionTag1;
        public int collisionTag2;
    }
    class CharacterControllerSystem : Behaviour
    {
        Vector3 campos;
        bool wallCollision = false;

        float rayCastTimer = 0;
        float upperRayWallDistance;
        float lowerRayWallDistance;
        float leftRayWallDistance;
        float rightRayWallDistance;

        bool Z_activated = false;
        bool Z2_activated = false;

        bool X_activated = false;
        bool X2_activated = false;

        float camSpeed = 0.1f;

        float referenceX;
        float referenceZ;


        Vector3 lastPlayerPos;
        //enum PlayerState
        //{
        //    RUN,
        //    RUN_SHOT,
        //    WAlK,
        //    WALK_SHOT,
        //    IDLE
        //}
        //PlayerState state;
        void Init()
        {
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            campos.y = transform.Position.y + 20;
            campos.x = transform.Position.x;
            campos.z = transform.Position.z - 5;

            referenceZ = transform.Position.z;
            referenceX = transform.Position.x;

            lastPlayerPos = transform.Position;

            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(90, -70, 0));
        }

        void InitCollisionFlags(ref CollisionBody rb, ref CharacterController character)
        {
            //int bitsSelf = 0;
            //bitsSelf |= 1 << character.bulletTag;
            rb.selfTag = character.bulletTag;

            rb.filterBits |= 1 << character.collisionTag1;
            rb.filterBits |= 1 << character.collisionTag2;
        }
        //void OnChangeState(PlayerState newState)
        //{
        //    state = newState;
        //    //Once input read, change player state
        //    switch (state)
        //    {
        //        case PlayerState.RUN:
        //            Animator.PlayAnimationName("run", m_EntityId);
        //            break;
        //        case PlayerState.IDLE:
        //            Animator.PlayAnimationName("idle", m_EntityId);
        //            break;
        //        default:
        //            break;
        //    }
        //}
        void Update()
        {
            //Console.WriteLine("Starting Update character controller");
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            //UpdateCamera(ref character, ref transform);
            UpdateInput(ref character, ref transform);




            Fire(ref character);
            //Console.WriteLine("Finish Update character controller");

            rayCastTimer += Time.DeltaTimeMS();

            if (rayCastTimer > 250.0f)
            {
                leftRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x + 7, transform.Position.y, transform.Position.z));
                rightRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x - 7, transform.Position.y, transform.Position.z));
                upperRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x, transform.Position.y, transform.Position.z + 7));
                lowerRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x, transform.Position.y, transform.Position.z - 7));
        
                //Console.WriteLine("LEFT RAY: " + leftRayWallDistance);
                //Console.WriteLine("RIGHT RAY: " + rightRayWallDistance);
                //Console.WriteLine("UPPER RAY: " + upperRayWallDistance);
                //Console.WriteLine("LOWER RAY: " + lowerRayWallDistance);
                rayCastTimer = 0;
                //Console.WriteLine("Left ray Wall distance" + leftRayWallDistance);
            }

        }
        private void Fire(ref CharacterController character)
        {
            //Console.WriteLine("Fire");
            float shootX = -Input.GetAxis(Gamepad.GamePad1, GamepadAxis.RightX);
            float shootY = -Input.GetAxis(Gamepad.GamePad1, GamepadAxis.RightY);

            character.fireTimer += Time.DeltaTime();

            if (character.fireTimer >= character.fireInterval)
            {
                character.fireTimer = 0.0f;

                if (System.Math.Abs(shootX) > 0.5f || System.Math.Abs(shootY) > 0.5f)
                {
                    // Shoot sound


                    //PlayMusic("player_shoot");
                    //PlayAudioEvent("player_shoot");
                    Vector3 bulletDir = new Vector3(shootX, 0, shootY);
                    SpawnBullet(ref character, new Vector3(0, 0, 0), bulletDir, 0);
                }
                else if (Input.IsKeyDown(KeyCode.Space))
                {
                    // Shoot sound

                    //PlayAudioEvent("player_shoot");
                    Vector3 bulletDir = new Vector3(0, 0, 1);
                    SpawnBullet(ref character, new Vector3(0, 0, 0), bulletDir, 0);
                }
            }
        }
        private void UpdateInput(ref CharacterController character, ref Transform3D transform)
        {
            //Console.WriteLine("Update Input");
            Vector3 forward = Mathf.CalculateForward(ref transform);
            Vector3 right = Mathf.CalculateRight(ref transform);
            //ref Rigidbody rigidbody = ref GetComponent<Rigidbody>();
            Vector3 finalVelocity = new Vector3(0, 0, 0);
            //float translation = character.maxVelocity * Time.DeltaTimeMS();

            Vector3 direction = new Vector3(0, 0, 0);
            direction += GetInputFromKeyboard(forward, right);
            direction += GetGamepadInputDirection(GamepadAxis.LeftX, GamepadAxis.LeftY);

            //transform.LocalPosition += direction;
            //finalVelocity = direction * translation;
            finalVelocity.x = Interpolate(0, character.maxVelocity, direction.x);
            finalVelocity.z = Interpolate(0, character.maxVelocity, direction.z);
            PhysicsManager.SetLinearVelocity(m_EntityId, finalVelocity);


            if (finalVelocity != Vector3Values.zero)
            {
                if (Mathf.Abs(finalVelocity.x) > character.maxVelocity * 0.3 || Mathf.Abs(finalVelocity.z) > character.maxVelocity * 0.3)
                {
                    Animator.PlayAnimationName("run", m_EntityId);

                }
                else Animator.PlayAnimationName("walk", m_EntityId);
            }
            else Animator.PlayAnimationName("idle", m_EntityId);




            Vector3 rotDirection = new Vector3(0, 0, 0);
            rotDirection += GetGamepadInputDirection(GamepadAxis.RightX, GamepadAxis.RightY);
            if (rotDirection == Vector3Values.zero)
            {
                if (direction != Vector3Values.zero)
                {
                    float angle = CalculateRotation(direction, forward);


                    if (angle > 25)
                    {
                        transform.LocalRotation.y += angle;
                        if (transform.LocalRotation.y > 360) transform.LocalRotation.y -= 360;
                    }
                }

            }
            else
            {
                float angle = CalculateRotation(rotDirection, forward);


                if (angle > 25)
                {
                    transform.LocalRotation.y += angle;
                    if (transform.LocalRotation.y > 360) transform.LocalRotation.y -= 360;
                }


            }
            //if (direction != Vector3Values.zero)
            //{
            //    float angle = CalculateRotation(direction, forward);

            //    transform.LocalRotation.y += angle;
            //    Console.WriteLine(angle);
            //    if (transform.LocalRotation.y > 360)
            //        transform.LocalRotation.y = 0f;
            //}

        }

        float CalculateRotation(Vector3 first, Vector3 second)
        {
            float angle = Mathf.Acos(Vector3.Dot(first.normalized, second.normalized)) * Mathf.Rad2Deg;
            if (!float.IsNaN(angle) && !float.IsInfinity(angle))
                return angle;
            return 0f;
        }
        private Vector3 GetGamepadInputDirection(GamepadAxis axis, GamepadAxis axis02)
        {
            Vector3 direction = new Vector3(0, 0, 0);

            direction.x = -Input.GetRawAxis(Gamepad.GamePad1, axis, 0f);
            direction.z = -Input.GetRawAxis(Gamepad.GamePad1, axis02, 0f);

            return direction;
        }
        private Vector3 GetInputFromKeyboard(Vector3 forward, Vector3 right)
        {
            Vector3 direction = new Vector3(0, 0, 0);
            ref Transform3D transform = ref GetComponent<Transform3D>();
            if (Input.IsKeyDown(KeyCode.W))
            {
                //PlayAudioEvent("player_walk");
                //     OnChangeState(PlayerState.WAlK);
                Animator.PlayAnimationName("run", m_EntityId);
                direction += forward;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
               // PlayAudioEvent("player_walk");
                //     OnChangeState(PlayerState.WAlK);
                Animator.PlayAnimationName("run", m_EntityId);
                direction -= forward;
            }
            if (Input.IsKeyDown(KeyCode.A))
            {
                //PlayAudioEvent("player_walk");
                //  OnChangeState(PlayerState.WAlK);
                Animator.PlayAnimationName("run", m_EntityId);
                direction += right;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                //PlayAudioEvent("player_walk");
                // OnChangeState(PlayerState.WAlK);
                Animator.PlayAnimationName("run", m_EntityId);
                direction -= right;
            }
            else
            {
                //  OnChangeState(PlayerState.IDLE);
                Animator.PlayAnimationName("idle", m_EntityId);
            }

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            lastPlayerPos.y = transform.Position.y;

            if (lowerRayWallDistance == -1 && upperRayWallDistance == -1)
            {
                lastPlayerPos.z = transform.Position.z;
            }

            if (leftRayWallDistance == -1 && rightRayWallDistance == -1)
            {
                lastPlayerPos.x = transform.Position.x;
            }

            float minCamVelocity = 0.0f;
            float maxCamVelocity = 0.3f;

            float normalicedDistance_Z = NormalizedValue(lastPlayerPos.z - (campos.z + 5), 0, 25);
            float normalicedDistance_X = NormalizedValue(lastPlayerPos.x - campos.x, 0, 25);

            float finalCamSpeed_X = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_X);
            float finalCamSpeed_Z = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_Z);

            campos.z += finalCamSpeed_Z;
            campos.x += finalCamSpeed_X;


            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(90, -70, 0));

            return direction;
        }

        private void UpdateCamera(ref CharacterController character, ref Transform3D transform)
        {
            //Console.WriteLine("Update camera");
            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            Vector3 campos = transform.Position;
            campos.y = transform.Position.y + character.camYOffset;
            campos.x = transform.Position.x + character.camXOffset;
            campos.z = transform.Position.z + character.camZOffset;

            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(character.camXAngle, character.camYAngle, 0));
        }


        void SpawnBullet(ref CharacterController character, Vector3 bullet_offset, Vector3 direction, float rot)
        {
            ref Transform3D parent = ref GetComponent<Transform3D>(m_EntityId);

            ulong entity = CreateEntity();

            // Take components of entity
            ref Transform3D newEntityTransform = ref GetComponent<Transform3D>(entity);
            ref BulletComponent bc = ref AddComponent<BulletComponent>(entity);
            AddMesh(entity, "Models/bullet", "assets/Models/03_mat_addelements.wimaterial");

            ref CollisionBody rb = ref AddComponent<CollisionBody>(entity);
            rb.scalingOffset.x = 1;
            rb.scalingOffset.y = 1;
            rb.scalingOffset.z = 1;
            rb.isTrigger = true;
            rb.isStatic = false;
            rb.doContinuousCollision = false;
            rb.selfTag = 0;
            //rb.filterBits |= 1 << 0;
            rb.filterBits = 0;
            InitCollisionFlags(ref rb, ref character);
            ref ColliderSphere collSph = ref AddComponent<ColliderSphere>(entity);
            collSph.radius = 1.0f;

            // Change position and scale
            newEntityTransform.LocalScale.x = character.bulletScale;
            newEntityTransform.LocalScale.y = character.bulletScale;
            newEntityTransform.LocalScale.z = character.bulletScale;

            newEntityTransform.LocalPosition.x = parent.LocalPosition.x;
            newEntityTransform.LocalPosition.y = parent.LocalPosition.y;
            newEntityTransform.LocalPosition.z = parent.LocalPosition.z;

            newEntityTransform.LocalRotation.y = direction.x * 90 + direction.z * 90;

            newEntityTransform.LocalPosition += bullet_offset;
            newEntityTransform.LocalPosition.y += 1;
            newEntityTransform.LocalRotation.y = 90.0f + rot;
            //newEntityTransform.LocalScale.x = newEntityTransform.LocalScale.y = newEntityTransform.LocalScale.z = 0.1f;
            //Console.WriteLine("entity: " + entity + " pos " + newEntityTransform.LocalPosition.x + " " + newEntityTransform.LocalPosition.y + " " + newEntityTransform.LocalPosition.z);
            // Add bullet component
            //bc.Velocity = 20.0f;
            bc.TimeToDestroy = character.bulletLifeTime;
            bc.Damage = 20;
            //bc.direction = direction;
            AddAudioSource(entity, "player_shoot", true, false);
            ApplySystem<AudioSystem>(entity);
            // Activate controller
            ApplySystem<BulletController>(entity);
            ApplySystem<MeshRenderer>(entity);
            ApplySystem<PhysicsSystem>(entity);
            PhysicsManager.SetLinearVelocity(entity, direction * character.bulletSpeed);
            //Console.WriteLine("entity: " + entity + " direction " + direction.x + " " + direction.y + " " + direction.z);
            PhysicsManager.AddBodyToLog(entity);

            // Activate controller
            ApplySystem<MeshRenderer>(entity);

        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            //Console.WriteLine(id1 + str1);
            //Console.WriteLine(id2 + str2);
            if (id1 == m_EntityId)
            {
                if (str2 == "WALL")
                {
                    wallCollision = true;
                }
                //Console.WriteLine("wall hit!!! ");
            }
        }

        void OnCollisionExit(EntityId id1, EntityId id2, string str1, string str2)
        {
            //Console.WriteLine(id1 + str1);
            //Console.WriteLine(id2 + str2);
            if (id1 == m_EntityId)
            {
                if (str2 == "WALL")
                {
                    wallCollision = false;
                }
                //Console.WriteLine("wall hit!!! ");
            }
        }

        float Interpolate(float value1, float value2, float weight)
        {
            return value1 * (1 - weight) + value2 * weight;
        }

        float NormalizedValue(float value, float min, float max)
        {
            return (value - min) / (max - min);
        }
    }
}
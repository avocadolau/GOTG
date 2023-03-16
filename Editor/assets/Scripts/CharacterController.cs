using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;

    [Component]
    public struct CharacterController
    {
        public float velocity;
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
        bool collision = false;
        int lastDirection = -1;
        void Init()
        {
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            campos.y = transform.Position.y + 20 + 35;
            campos.x = transform.Position.x + 0 + 0;
            campos.z = transform.Position.z -40 + 20;

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

        void Update()
        {
            //Console.WriteLine("Starting Update character controller");
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            //UpdateCamera(ref character, ref transform);
            UpdateInput(ref character, ref transform);

            Fire(ref character);
            //Console.WriteLine("Finish Update character controller");
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
                    PlayAudioEvent("player_shoot");
                    Vector3 bulletDir = new Vector3(shootX, 0, shootY);
                    SpawnBullet(ref character, new Vector3(0, 0, 0), bulletDir, 0);
                }
                else if (Input.IsKeyDown(KeyCode.Space))
                {
                    // Shoot sound
                    PlayAudioEvent("player_shoot");
                    Vector3 bulletDir = new Vector3(0, 0, 1);
                    SpawnBullet(ref character, new Vector3(0, 0, 0), bulletDir, 0);
                }
            }
        }
        private void UpdateInput(ref CharacterController character, ref Transform3D transform)
        {
            //Console.WriteLine("Update Input");
            Vector3 forward = Wiwa.Math.CalculateForward(ref transform);
            Vector3 right = Wiwa.Math.CalculateRight(ref transform);
            //ref Rigidbody rigidbody = ref GetComponent<Rigidbody>();
            float translation = character.velocity;

            Vector3 direction = new Vector3(0, 0, 0);
            direction += GetInputFromKeyboard(forward, right, translation);
            direction += GetInputGamepad(forward, right, translation);

            //transform.LocalPosition += direction;
            PhysicsManager.SetLinearVelocity(m_EntityId, direction);
        }
        private Vector3 GetInputGamepad(Vector3 forward, Vector3 right, float translation)
        {
            Vector3 direction = new Vector3(0, 0, 0);

            direction += right * translation * -Input.GetAxis(Gamepad.GamePad1, GamepadAxis.LeftX);
            direction += forward * translation * -Input.GetAxis(Gamepad.GamePad1, GamepadAxis.LeftY);

            return direction;
        }
        private Vector3 GetInputFromKeyboard(Vector3 forward, Vector3 right, float translation)
        {
            Vector3 direction = new Vector3(0, 0, 0);

            if (Input.IsKeyDown(KeyCode.W))
            {
                PlayAudioEvent("player_walk");
                direction += forward * translation;
                lastDirection = 1;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                PlayAudioEvent("player_walk");
                direction -= forward * translation;
                lastDirection = 2;
            }
            if (Input.IsKeyDown(KeyCode.A))
            {
                PlayAudioEvent("player_walk");
                direction += right * translation;
                lastDirection = 3;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                PlayAudioEvent("player_walk");
                direction -= right * translation;
                lastDirection = 4;
            }

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            if(collision == false)
            {
                if (Input.IsKeyDown(KeyCode.W))
                {
                    campos.z += direction.z / 300; // +
                }
                else if (Input.IsKeyDown(KeyCode.S))
                {
                    campos.z += direction.z / 300; // -
                }
                if (Input.IsKeyDown(KeyCode.A))
                {
                    campos.x += direction.x / 300; // +
                }
                else if (Input.IsKeyDown(KeyCode.D))
                {
                    campos.x += direction.x / 300; // -
                }
            }

            if(collision == true)
            {
                Console.WriteLine("Z direction: " + direction.z);
                Console.WriteLine("X direction: " + direction.x);

                //if (lastDirection == 2)
                //{
                //    direction.z--;
                //}
                //if (lastDirection == 1)
                //{
                //    direction.z++;
                //}
                //if (lastDirection == 4)
                //{
                //    direction.x--;
                //}
                //if (lastDirection == 3)
                //{
                //    direction.x++;
                //}

                if (lastDirection == 2)
                {
                    direction.z += 10;
                }
                if (lastDirection == 1)
                {
                    direction.z -= 10;
                }
                if (lastDirection == 4)
                {
                    direction.x += 10;
                }
                if (lastDirection == 3)
                {
                    direction.x -= 10;
                }
            }

            //campos.x = transform.Position.x + character.camXOffset + 0;
            //campos.y = transform.Position.y + character.camYOffset + 35;
            //campos.z = transform.Position.z + character.camZOffset + 20;

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

        void OnCollisionEnter(EntityId id1, EntityId id2)
        {
            if (id1 == m_EntityId && PhysicsManager.GetEntityTagString(id2) == "COLLISION_WALL")
            {
                collision = true;
                Console.WriteLine("wall hit!!! ");
            }
        }
        
        void OnCollisionExit(EntityId id1, EntityId id2)
        {
            if (id1 == m_EntityId && PhysicsManager.GetEntityTagString(id2) == "COLLISION_WALL")
            {
                collision = false;
                Console.WriteLine("wall UN-hit!!! ");
            }
        }
    }
}
using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    [Component]
    public struct CharacterControllerCmp
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
    }
    class CharacterController : Behaviour
    {
        void InitCollisionFlags(ref Rigidbody rb, ref CharacterControllerCmp character)
        {
            int bitsSelf = 0;
            bitsSelf |= 1 << character.bulletTag;
            rb.selfTag = bitsSelf;

            int bitsColl = 0;
            bitsColl = 1 << character.collisionTag1; 
            rb.filterBits = bitsColl;
        }

        void Update()
        {
            ref CharacterControllerCmp character = ref GetComponent<CharacterControllerCmp>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            UpdateCamera(ref character, ref transform);
            UpdateInput(ref character, ref transform);

            Fire(ref character);
        }
        private void Fire(ref CharacterControllerCmp character)
        {
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
        private void UpdateInput(ref CharacterControllerCmp character, ref Transform3D transform)
        {
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
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                PlayAudioEvent("player_walk");
                direction -= forward * translation;
            }
            if (Input.IsKeyDown(KeyCode.A))
            {
                PlayAudioEvent("player_walk");
                direction += right * translation;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                PlayAudioEvent("player_walk");
                direction -= right * translation;
            }

            return direction;
        }

        private void UpdateCamera(ref CharacterControllerCmp character, ref Transform3D transform)
        {
            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            Vector3 campos = transform.Position;
            campos.y = transform.Position.y + character.camYOffset;
            campos.x = transform.Position.x + character.camXOffset;
            campos.z = transform.Position.z + character.camZOffset;

            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(character.camXAngle, character.camYAngle, 0));
        }


        void SpawnBullet(ref CharacterControllerCmp character, Vector3 bullet_offset, Vector3 direction, float rot)
        {
            ref Transform3D parent = ref GetComponent<Transform3D>(m_EntityId);

            ulong entity = CreateEntity();

            // Take components of entity
            ref Transform3D newEntityTransform = ref GetComponent<Transform3D>(entity);
            ref BulletComponent bc = ref AddComponent<BulletComponent>(entity);
            AddMesh(entity, "Models/bullet", "assets/Models/03_mat_addelements.wimaterial");

            ref Rigidbody rb = ref AddComponent<Rigidbody>(entity);
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
            Console.WriteLine("entity: " + entity + " pos " + newEntityTransform.LocalPosition.x + " " + newEntityTransform.LocalPosition.y + " " + newEntityTransform.LocalPosition.z);
            // Add bullet component
            //bc.Velocity = 20.0f;
            bc.TimeToDestroy = character.bulletLifeTime;
            //bc.direction = direction;

            // Activate controller
            ApplySystem<BulletController>(entity);
            ApplySystem<MeshRenderer>(entity);
            ApplySystem<PhysicsSystem>(entity);
            PhysicsManager.SetLinearVelocity(entity, direction * character.bulletSpeed);
            Console.WriteLine("entity: " + entity + " direction " + direction.x + " " + direction.y + " " + direction.z);
            PhysicsManager.AddBodyToLog(entity);

            // Activate controller
            ApplySystem<MeshRenderer>(entity);

        }
    }
}
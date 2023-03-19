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

        void Init()
        {
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            campos.y = transform.Position.y + 20 + 35;
            campos.x = transform.Position.x + 0 + 0;
            campos.z = transform.Position.z - 40 + 20;

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

        void Update()
        {
            //Console.WriteLine("Starting Update character controller");
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            //UpdateCamera(ref character, ref transform);
            UpdateInput(ref character, ref transform);

            //Fire(ref character);
            //Console.WriteLine("Finish Update character controller");

            rayCastTimer += Time.DeltaTimeMS();

            if (rayCastTimer > 250.0f)
            {
                leftRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x + 15, transform.Position.y, transform.Position.z));
                rightRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x - 15, transform.Position.y, transform.Position.z));
                upperRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x, transform.Position.y, transform.Position.z + 15));
                lowerRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x, transform.Position.y, transform.Position.z - 15));

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
            float translation = character.velocity * Time.DeltaTime();

            Vector3 direction = new Vector3(0, 0, 0);
            direction += GetInputFromKeyboard(forward, right, translation);
            direction += GetGamepadInputDirection(GamepadAxis.LeftX, GamepadAxis.LeftY);

            //transform.LocalPosition += direction;
            PhysicsManager.SetLinearVelocity(m_EntityId, direction * translation);

            if (direction.x != 0f || direction.z != 0f)
            {

                float newRotation = CalculateRotation(direction, forward);
                if (newRotation != 0f)
                {
                    Console.WriteLine("New rotation:" + newRotation);
                    Console.WriteLine("Direction: " + direction.x + " X " + direction.y + " Y");
                    transform.LocalRotation.y += newRotation;
                }
                if (transform.LocalRotation.y > 360f)
                    transform.LocalRotation.y = 0f;
            }
        }
        public Vector3 RotateTowards(Vector3 direction, Vector3 newDirection, float rotationSpeed)
        {
            // Calculate the angle between the two vectors
            float angle = CalculateRotation(direction, newDirection);

            // Calculate the rotation axis
            Vector3 rotationAxis = Cross(direction, newDirection);

            // Create a rotation quaternion
            Quaternion rotation = AngleAxis(angle * rotationSpeed * Time.DeltaTime(), rotationAxis);

            // Rotate the direction vector towards the new direction vector
            direction = rotation * direction;

            return direction;
        }
        public Vector3 Cross(Vector3 a, Vector3 b)
        {
            Vector3 cross = new Vector3();

            cross.x = a.y * b.z - a.z * b.y;
            cross.y = a.z * b.x - a.x * b.z;
            cross.z = a.x * b.y - a.y * b.x;

            return cross;
        }
        public Quaternion AngleAxis(float angle, Vector3 axis)
        {
            // Convert the angle to radians
            float radians = angle * ((float)(180 / System.Math.PI));

            // Calculate the sin and cos of half the angle
            float halfSin = (float)System.Math.Sin(radians / 2f);
            float halfCos = (float)System.Math.Cos(radians / 2f);

            // Normalize the axis vector
            axis.Normalize();

            // Calculate the quaternion components
            float x = axis.x * halfSin;
            float y = axis.y * halfSin;
            float z = axis.z * halfSin;
            float w = halfCos;

            // Create and return the quaternion
            return new Quaternion(x, y, z, w);
        }
        float CalculateRotation(Vector3 first, Vector3 second)
        {
            float angle = Wiwa.Math.ACos(Wiwa.Math.Dot(first.Normalized(), second.Normalized())) * 180f / (float)System.Math.PI;
            if (!float.IsNaN(angle))
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
        private Vector3 GetInputFromKeyboard(Vector3 forward, Vector3 right, float translation)
        {
            Vector3 direction = new Vector3(0, 0, 0);
            ref Transform3D transform = ref GetComponent<Transform3D>();

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

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            //if (Input.IsKeyDown(KeyCode.W))
            //{
            //    if (upperRayWallDistance != -1)
            //    {
            //        campos.z += ((direction.z / 500) * (upperRayWallDistance)) / 800; // +
            //    }
            //    else if (upperRayWallDistance == -1)
            //    {
            //        campos.z += ((direction.z / 500) * (1600)) / 800; // +
            //    }
            //
            //    //Console.WriteLine("Zw direction: " + ((direction.z / 500) * (100)) / 600);
            //}
            //else if (Input.IsKeyDown(KeyCode.S))
            //{
            //    //campos.z += (direction.z / 500) * Math.Sqrt(lowerRayWallDistance, 5); // -
            //
            //    if (lowerRayWallDistance != -1)
            //    {
            //        campos.z += ((direction.z / 500) * (lowerRayWallDistance)) / 800; // -
            //    }
            //    else if (lowerRayWallDistance == -1)
            //    {
            //        campos.z += ((direction.z / 500) * (1600)) / 800; // -
            //    }
            //    //Console.WriteLine("Zs direction: " + ((direction.z / 500) * (100)) / 600);
            //}
            //if (Input.IsKeyDown(KeyCode.A))
            //{
            //    //campos.x += (direction.x / 500) * Math.Sqrt(leftRayWallDistance); // +
            //
            //    if (leftRayWallDistance != -1)
            //    {
            //        campos.x += ((direction.x / 500) * (leftRayWallDistance)) / 800; // +
            //    }
            //    else if (leftRayWallDistance == -1)
            //    {
            //        campos.x += ((direction.x / 500) * (1600)) / 800; // +
            //    }
            //
            //    //Console.WriteLine("Xa direction: " + ((direction.x / 500) * (100)) / 600);
            //}
            //else if (Input.IsKeyDown(KeyCode.D))
            //{
            //    //campos.x += (direction.x / 500) * Math.Sqrt(rightRayWallDistance, 5); // -
            //
            //    if (rightRayWallDistance != -1)
            //    {
            //        campos.x += ((direction.x / 500) * (rightRayWallDistance)) / 800; // -
            //    }
            //    else if (rightRayWallDistance == -1)
            //    {
            //        campos.x += ((direction.x / 500) * (1600)) / 800; // -
            //    }
            //
            //    //Console.WriteLine("Xd direction: " + ((direction.x / 500) * (100)) / 600);
            //}

            //if(transform.Position.z - campos.z > 40 || Z_activated == true)
            //{
            //    campos.z += 0.1f;
            //    Z_activated = true;
            //}
            //else if (transform.Position.z - campos.z < 5 || Z2_activated == true)
            //{
            //    campos.z  -= 0.1f;
            //    Z2_activated = true;
            //}
            //if (transform.Position.z - campos.z > 10 && transform.Position.z - campos.z < 35)
            //{
            //    Z_activated = false;
            //    Z2_activated = false;
            //}
            //
            //if (transform.Position.x - campos.x > 30 || X_activated == true)
            //{
            //    campos.x += 0.1f;
            //    X_activated = true;
            //}
            //else if (transform.Position.x - campos.x < -30 || X2_activated == true)
            //{
            //    campos.x -= 0.1f;
            //    X2_activated = true;
            //}
            //if(transform.Position.x - campos.x < 25 && transform.Position.x - campos.x > -25)
            //{
            //    X_activated = false;
            //    X2_activated = false;
            //}
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

            float normalicedDistance_Z = NormalizedValue(lastPlayerPos.z - (campos.z + 20), 0, 25);
            float normalicedDistance_X = NormalizedValue(lastPlayerPos.x - campos.x, 0, 25);

            float finalCamSpeed_X = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_X);
            float finalCamSpeed_Z = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_Z);

            campos.z += finalCamSpeed_Z;
            campos.x += finalCamSpeed_X;
            //if (transform.Position.z - campos.z > 20)
            //{
            //    campos.z += finalCamSpeed_Z; // +
            //}
            //else if (transform.Position.z - campos.z < 15)
            //{
            //    campos.z += finalCamSpeed_Z; // -
            //}
            //
            //if (transform.Position.x - campos.x > 5)
            //{
            //    campos.x += finalCamSpeed_X; // +
            //}
            //else if (transform.Position.x - campos.x < -5)
            //{
            //    campos.x += finalCamSpeed_X; // -
            //}

            /*
            if(transform.Position.z - campos.z > 40)
            {
                campos.z += 0.1f;
            }
            if (transform.Position.z - campos.z < 5)
            {
                campos.z  -= 0.1f;
            }
            if (transform.Position.x - campos.x > 30)
            {
                campos.x += 0.1f;
            }
            if (transform.Position.x - campos.x < -30)
            {
                campos.x -= 0.1f;
            }
            */

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

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine(id1 + str1);
            Console.WriteLine(id2 + str2);
            if (id1 == m_EntityId)
            {
                if (str2 == "WALL")
                {
                    wallCollision = true;
                }
                else if (str2 == "END_TRIGGER")
                {
                    GameState.SetRoomStateTriggerNext(true);
                    Console.WriteLine("Trigger true ");
                }
                //Console.WriteLine("wall hit!!! ");
            }
        }

        void OnCollisionExit(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine(id1 + str1);
            Console.WriteLine(id2 + str2);
            if (id1 == m_EntityId)
            {
                if (str2 == "WALL")
                {
                    wallCollision = false;
                }
                else if (str2 == "END_TRIGGER")
                {
                    GameState.SetRoomStateTriggerNext(false);
                    Console.WriteLine("Trigger false ");
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
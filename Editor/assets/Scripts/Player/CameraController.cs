using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;
    
    class CameraControllerSystem : Behaviour
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

            campos.y = transform.Position.y + 20;
            campos.x = transform.Position.x;
            campos.z = transform.Position.z;

            referenceZ = transform.Position.z;
            referenceX = transform.Position.x;

            lastPlayerPos = transform.Position;

            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(90, -70, 0));
        }

        void Update()
        {
            //Console.WriteLine("Starting Update character controller");
            ref CharacterController character = ref GetComponent<CharacterController>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

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

            float normalicedDistance_Z = NormalizedValue(lastPlayerPos.z - (campos.z), 0, 15);
            float normalicedDistance_X = NormalizedValue(lastPlayerPos.x - campos.x, 0, 15);

            float finalCamSpeed_X = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_X);
            float finalCamSpeed_Z = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_Z);

            campos.z += finalCamSpeed_Z;
            campos.x += finalCamSpeed_X;


            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(90, -70, 0));
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
                //Console.WriteLine("wall UNhit!!! ");
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
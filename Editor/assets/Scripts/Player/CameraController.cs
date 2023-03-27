using System;
using Wiwa;


namespace Game
{
    using EntityId = System.UInt64;

    class CameraControllerSystem : Behaviour
    {
        // Camera required variables delcaration
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

        float extraPosOffsetX;
        float extraPosOffsetZ;
        float extraPosOffsetDistanceX;
        float extraPosOffsetDistanceZ;

        float offsetDivision;

        Vector3 lastPlayerPos;

        void Init()
        {
            ref Character character = ref GetComponent<Character>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            // Some camera required variables initialitation

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            offsetDivision = 65; // <-- This variable divides the speed for the camera movement offset transition - the lower the value the faster the transition is
            extraPosOffsetDistanceX = 4; // <-- This variable determines how far the camera will go when offsetting on the X axis
            extraPosOffsetDistanceZ = 5; // <-- This variable determines how far the camera will go when offsetting on the Z axis

            extraPosOffsetX = 0;
            extraPosOffsetZ = 0;

            campos.y = transform.Position.y + 25;
            campos.x = transform.Position.x;
            campos.z = transform.Position.z - 10;

            referenceZ = transform.Position.z;
            referenceX = transform.Position.x;

            lastPlayerPos = transform.Position;

            CameraManager.SetPosition(cam_id, campos);
            CameraManager.SetCameraRotation(cam_id, new Vector3(90, -70, 0));
        }

        void Update()
        {
            //Console.WriteLine("Starting Update character controller");
            ref Character character = ref GetComponent<Character>();
            ref Transform3D transform = ref GetComponent<Transform3D>();

            // Raycast checks
            rayCastTimer += Time.DeltaTimeMS();

            if (rayCastTimer > 250.0f)
            {
                leftRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x + 7, transform.Position.y, transform.Position.z));
                rightRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x - 7, transform.Position.y, transform.Position.z));
                //upperRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x, transform.Position.y, transform.Position.z + 7)); //Not actually needed
                upperRayWallDistance = -1;
                lowerRayWallDistance = PhysicsManager.RayCastDistanceWalls(transform.Position, new Vector3(transform.Position.x, transform.Position.y, transform.Position.z - 7));

                //Console.WriteLine("LEFT RAY: " + leftRayWallDistance);
                //Console.WriteLine("RIGHT RAY: " + rightRayWallDistance);
                //Console.WriteLine("UPPER RAY: " + upperRayWallDistance);
                //Console.WriteLine("LOWER RAY: " + lowerRayWallDistance);
                rayCastTimer = 0;
            }

            System.UInt64 cam_id = CameraManager.GetActiveCamera();

            // Make the camera move an especified offset when walking on specified direction (with keyboard)

            //extraPosOffsetX = 0;
            //extraPosOffsetZ = 0;

            if (Input.IsKeyDown(KeyCode.A))
            {
                if(extraPosOffsetX <= (extraPosOffsetDistanceX)) extraPosOffsetX += (Time.DeltaTimeMS() / offsetDivision);
            }
            if (Input.IsKeyDown(KeyCode.D))
            {
                if(extraPosOffsetX >= -(extraPosOffsetDistanceX)) extraPosOffsetX -= (Time.DeltaTimeMS() / offsetDivision);
            }
            if (Input.IsKeyDown(KeyCode.W))
            {
                if (extraPosOffsetZ <= (extraPosOffsetDistanceZ)) extraPosOffsetZ += (Time.DeltaTimeMS() / offsetDivision);
            }
            if (Input.IsKeyDown(KeyCode.S))
            {
                if (extraPosOffsetZ >= -(extraPosOffsetDistanceZ)) extraPosOffsetZ -= (Time.DeltaTimeMS() / offsetDivision);
            }

            // Make the camera move an especified offset when walking on specified direction (with gamepad)

            if (Input.IsKeyDown(KeyCode.Left))
            {
                if (extraPosOffsetX <= (extraPosOffsetDistanceX)) extraPosOffsetX += (Time.DeltaTimeMS() / offsetDivision);
            }
            if (Input.IsKeyDown(KeyCode.Right))
            {
                if (extraPosOffsetX >= -(extraPosOffsetDistanceX)) extraPosOffsetX -= (Time.DeltaTimeMS() / offsetDivision);
            }
            if (Input.IsKeyDown(KeyCode.Up))
            {
                if (extraPosOffsetZ <= (extraPosOffsetDistanceZ)) extraPosOffsetZ += (Time.DeltaTimeMS() / offsetDivision);
            }
            if (Input.IsKeyDown(KeyCode.Down))
            {
                if (extraPosOffsetZ >= -(extraPosOffsetDistanceZ)) extraPosOffsetZ -= (Time.DeltaTimeMS() / offsetDivision);
            }

            // If raycast detect a wall do not update the position where camera have to move next

            lastPlayerPos.y = transform.Position.y;

            if (lowerRayWallDistance == -1 && upperRayWallDistance == -1)
            {
                lastPlayerPos.z = transform.Position.z + extraPosOffsetZ;
            }

            if (leftRayWallDistance == -1 && rightRayWallDistance == -1)
            {
                lastPlayerPos.x = transform.Position.x + extraPosOffsetX;
            }

            // Distance between central point and player point interpolation

            float minCamVelocity = 0.0f;
            float maxCamVelocity = 0.3f;

            float normalicedDistance_Z = NormalizedValue(lastPlayerPos.z - (campos.z + 10), 0, 20);
            float normalicedDistance_X = NormalizedValue(lastPlayerPos.x - campos.x, 0, 20);

            float finalCamSpeed_X = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_X);
            float finalCamSpeed_Z = Interpolate(minCamVelocity, maxCamVelocity, normalicedDistance_Z);

            // Move camera with changing speed depending on interpolation calculated previowsly

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
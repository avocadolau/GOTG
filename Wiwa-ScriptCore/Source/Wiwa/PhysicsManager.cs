namespace Wiwa
{
    using EntityId = System.UInt64;
    public class PhysicsManager
    {
        public static bool AddBodyCube(EntityId id, ref ColliderCube colliderCube, ref Transform3D transform , ref Rigidbody rigidBody)
        {
            return InternalCalls.AddBodyCube(id, ref colliderCube, ref transform , ref rigidBody); 
        }

        public static void SetLinearVelocity(EntityId id, Vector3 velocity) 
        {
            InternalCalls.SetLinearVelocity(id, velocity);
        }

        public static bool AddBodyToLog(EntityId id)
        {
            return InternalCalls.AddBodyToLog(id);
        }

        public static bool RemoveBodyFromLog(EntityId id)
        {
            return InternalCalls.RemoveBodyFromLog(id);
        }
        //public static CameraId GetActiveCamera()
        //{
        //    return InternalCalls.GetActiveCamera();
        //}

        //public static void SetPosition(CameraId camid, Vector3 pos)
        //{
        //    InternalCalls.SetCameraPosition(camid, pos);
        //}

        //public static void LookAt(CameraId camid, Vector3 pos)
        //{
        //    InternalCalls.CameraLookAt(camid, pos);
        //}

        //public static void SetFront(CameraId camid, Vector3 front)
        //{
        //    InternalCalls.SetCameraFront(camid, front);
        //}

        //public static void SetCameraRotation(CameraId camid, Vector3 rot)
        //{
        //    InternalCalls.SetCameraRotation(camid, rot);
        //}
    }
}


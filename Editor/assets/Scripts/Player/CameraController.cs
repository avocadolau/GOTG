using System;
using Wiwa;


namespace Game
{
    class CameraControllerSystem : Behaviour
    {
        private Vector3 cameraLastPos;
        private Vector3 cameraPosition;
        private ComponentIterator transformIt;
        private float cameraYPos = 40f;
        private float cameraZOffset = -15f;
        private float lerpTime = 0.1f;
        private Vector3 cameraFollowerPos;
        private Vector3 cameraAngles;
        void Init()
        {
            cameraAngles = new Vector3(90, -70, 0);
            cameraFollowerPos = new Vector3(0f, 0f, 5f);
            transformIt = GetComponentIterator<Transform3D>(GetChildByName("CameraFollower"));

            System.UInt64 cam_id = CameraManager.GetActiveCamera();
            ref Transform3D t3d = ref GetComponentByIterator<Transform3D>(transformIt);

            if (transformIt.componentId == Constants.WI_INVALID_INDEX)
            {
                Console.WriteLine("Can't get the child");
                return;
            }

            t3d.LocalPosition = cameraFollowerPos;

            cameraPosition = t3d.worldMatrix.GetPosition();
            cameraLastPos = cameraPosition;
            cameraPosition.y = cameraYPos;

            CameraManager.SetPosition(cam_id, cameraPosition);
            CameraManager.SetCameraRotation(cam_id, cameraAngles);
        }

        void Update()
        {
            System.UInt64 cam_id = CameraManager.GetActiveCamera();
            cameraLastPos = cameraPosition;

            cameraPosition = GetComponentByIterator<Transform3D>(transformIt).worldMatrix.GetPosition();

            cameraPosition = Vector3.Lerp(cameraPosition, cameraLastPos, lerpTime * Time.DeltaTime());

            cameraPosition.y = cameraYPos;
            cameraPosition.z += cameraZOffset;

            CameraManager.SetPosition(cam_id, cameraPosition);
            CameraManager.SetCameraRotation(cam_id, cameraAngles);
        }
    }
}
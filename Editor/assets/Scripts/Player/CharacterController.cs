using System;
using Wiwa;


namespace Game
{
    [Component]
    public struct CharacterController
    {
        public float Velocity;
        public float GamepadDeadZone;
        public float RotationSpeed;
    }
    class CharacterControllerSystem : Behaviour
    {
        private ComponentIterator characterControllerIt;
        private ComponentIterator transformIt;
        private ComponentIterator rigidBodyIt;
        void Awake()
        {
            characterControllerIt = GetComponentIterator<CharacterController>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();
        }

        void Update()
        {
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref CharacterController controller = ref GetComponentByIterator<CharacterController>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            Vector3 input = GetInput(ref controller);
            input *= controller.Velocity * Time.DeltaTimeMS();

            PhysicsManager.SetLinearVelocity(m_EntityId, input);

            if(input != Vector3Values.zero)
                SetPlayerRotation(ref transform.LocalRotation, input, controller.RotationSpeed);
        }

        Vector3 GetInput(ref CharacterController controller)
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
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftX, controller.GamepadDeadZone);
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftY, controller.GamepadDeadZone);

            return input;
        }
        void SetPlayerRotation(ref Vector3 currentRotation, Vector3 input, float rotationSpeed)
        {
            float angle = Mathf.Atan2(input.x, input.z) * Mathf.Rad2Deg;
            currentRotation.y = Mathf.LerpAngle(currentRotation.y, angle, rotationSpeed);
        }
    }
}
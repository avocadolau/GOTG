using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;

    [Component]
    public struct CharacterController
    {
        public float Velocity;
        public float GamepadDeadZone;
    }
    class CharacterControllerSystem : Behaviour
    {
        private ComponentIterator characterControllerIt;
        private ComponentIterator transformIt;
        void Awake()
        {
            characterControllerIt = GetComponentIterator<CharacterController>();
            transformIt = GetComponentIterator<Transform3D>();
        }

        void Update()
        {
            Transform3D transform = GetComponentByIterator<Transform3D>(transformIt);
            CharacterController controller = GetComponentByIterator<CharacterController>(characterControllerIt);
            Vector3 input = GetInput();
            transform.LocalRotation += input * controller.Velocity * Time.DeltaTime();
        }

        Vector3 GetInput()
        {
            CharacterController controller = GetComponentByIterator<CharacterController>(characterControllerIt);

            Vector3 input = Vector3Values.zero;

            if (Input.IsKeyDown(KeyCode.A))
                input.x -= 1;
            if (Input.IsKeyDown(KeyCode.D))
                input.x += 1;
            if (Input.IsKeyDown(KeyCode.W))
                input.z += 1;
            if (Input.IsKeyDown(KeyCode.S))
                input.z -= 1;

            input.x += Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftX, controller.GamepadDeadZone);
            input.z += Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftY, controller.GamepadDeadZone);

            return input;
        }
    }
}
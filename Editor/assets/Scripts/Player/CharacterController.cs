using Wiwa;


namespace Game
{
    [Component]
    public struct CharacterController
    {
        public float Velocity;
        public float GamepadDeadZone;
        public float RotationSpeed;

        public float DashCoolDown;
        public float DashMaxSpeed;
        public float DashIncrement;

        public float WalkTreshold;
    }
    class CharacterControllerSystem : Behaviour
    {
        private ComponentIterator characterControllerIt;
        private ComponentIterator transformIt;
        private ComponentIterator rigidBodyIt;

        private float dashTimer = 0f;
        private float dashCurrentVel = 0f;
        private bool isDashing = false;
        void Awake()
        {
            //Setting components
            characterControllerIt = GetComponentIterator<CharacterController>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();

            dashTimer = GetComponentByIterator<CharacterController>(characterControllerIt).DashCoolDown;
        }

        void Update()
        {
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref CharacterController controller = ref GetComponentByIterator<CharacterController>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            Vector3 input = GetMovementInput(ref controller);
            Vector3 velocity = input * controller.Velocity * Time.DeltaTimeMS();


            bool isDashing = Dash(ref velocity, input, controller, transform);

            PhysicsManager.SetLinearVelocity(m_EntityId, velocity);

            if (input != Vector3Values.zero)
                SetPlayerRotation(ref transform.LocalRotation, input, controller.RotationSpeed);

            UpdateAnimation(input, controller);


            Vector3 shootInput = GetShootingInput(ref controller);
            if (shootInput != Vector3Values.zero)
                SetPlayerRotation(ref transform.LocalRotation, shootInput, controller.RotationSpeed);
        }

        Vector3 GetMovementInput(ref CharacterController controller)
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
        Vector3 GetShootingInput(ref CharacterController controller)
        {
            Vector3 input = Vector3Values.zero;

            //     z
            //     |
            //     |
            //-----|----- -x
            //     |
            //     |



            if (Input.IsKeyDown(KeyCode.Left))
                input.x += 1;
            if (Input.IsKeyDown(KeyCode.Right))
                input.x -= 1;
            if (Input.IsKeyDown(KeyCode.Up))
                input.z += 1;
            if (Input.IsKeyDown(KeyCode.Down))
                input.z -= 1;

            //This axis need to be inverted
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightX, controller.GamepadDeadZone);
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightY, controller.GamepadDeadZone);

            return input;
        }
        void SetPlayerRotation(ref Vector3 currentRotation, Vector3 input, float rotationSpeed)
        {
            float angle = Mathf.Atan2(input.x, input.z) * Mathf.Rad2Deg;
            currentRotation.y = Mathf.LerpAngle(currentRotation.y, angle, rotationSpeed);
        }
        void UpdateAnimation(Vector3 input, CharacterController controller)
        {
            if (input == Vector3Values.zero)
            {
                Animator.PlayAnimationName("idle", m_EntityId);
                return;
            }

            float mag = input.magnitude;
            Mathf.Clamp01(mag);

            if (mag <= controller.WalkTreshold)
            {
                Animator.PlayAnimationName("walk", m_EntityId);
                return;
            }
            Animator.PlayAnimationName("run", m_EntityId);
        }
        bool Dash(ref Vector3 velocity, Vector3 input, CharacterController controller, Transform3D transform)
        {
            dashTimer += Time.DeltaTime();

            if (dashTimer >= controller.DashCoolDown &&
            (Input.IsKeyDown(KeyCode.LeftShift) || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadA)))
            {
                isDashing = true;
                return true;
            }

            if (isDashing)
            {
                if (input == Vector3Values.zero)
                {
                    input = Mathf.CalculateForward(ref transform);
                }

                dashCurrentVel += controller.DashIncrement;
                velocity = input * dashCurrentVel * Time.DeltaTimeMS();

                if (dashCurrentVel >= controller.DashMaxSpeed)
                {
                    dashTimer = 0f;
                    dashCurrentVel = 0f;
                    isDashing = false;
                }
                return true;

            }

            return false;
        }
    }
}
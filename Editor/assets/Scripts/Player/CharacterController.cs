using System;
using Wiwa;


namespace Game
{
    using EntityId = System.UInt32;
    [Component]
    public struct CharacterController
    {
        public float Velocity;
        public float GamepadDeadZone;
        public float RotationSpeed;

        public float DashCoolDown;
        public float DashDistance;
        public float DashSpeed;

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

        private Vector3 lastPos = Vector3Values.zero;

        private int normalTags = 0;
        private int dashTags = 0;
        void Awake()
        {
            InitTags();
            //Setting components
            characterControllerIt = GetComponentIterator<CharacterController>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();

            dashTimer = GetComponentByIterator<CharacterController>(characterControllerIt).DashCoolDown;
        }
        void InitTags()
        {
            dashTags |= 1 << PhysicsManager.GetTagBitsByString("WALL");
            normalTags |= 1 << PhysicsManager.GetTagBitsByString("WALL");
            normalTags |= 1 << PhysicsManager.GetTagBitsByString("COLUMN");
            normalTags |= 1 << PhysicsManager.GetTagBitsByString("ENEMY");
            normalTags |= 1 << PhysicsManager.GetTagBitsByString("END_ROOM_TRIGGER");
            normalTags |= 1 << PhysicsManager.GetTagBitsByString("START_RUN_TRIGGER");
        }
        void Update()
        {
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref CharacterController controller = ref GetComponentByIterator<CharacterController>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            Vector3 velocity = Vector3Values.zero;

            Vector3 input = GetMovementInput(ref controller);

            if (!isDashing)
                velocity = input * controller.Velocity;

            Dash(ref velocity, input, controller, transform, ref rb);

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
        void Dash(ref Vector3 velocity, Vector3 input, CharacterController controller, Transform3D transform, ref CollisionBody cb)
        {
            dashTimer += Time.DeltaTime();

            if (dashTimer >= controller.DashCoolDown &&
            (Input.IsKeyDown(KeyCode.LeftShift) || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadA)))
            {
                isDashing = true;
                lastPos = transform.LocalPosition;
            }

            if (isDashing)
            {
                if (input == Vector3Values.zero)
                {
                    input = Mathf.CalculateForward(ref transform);
                }

                Vector3 targetPoint = Mathf.PointAlongDirection(lastPos, input, controller.DashDistance);
                dashCurrentVel += controller.DashSpeed;
                velocity = input * dashCurrentVel;

                float distance = Vector3.Distance(targetPoint, transform.LocalPosition);
                PhysicsManager.ChangeCollisionTags(m_EntityId);
                Console.WriteLine($"Target: {targetPoint.x} X {targetPoint.y} Y {targetPoint.z}");
                Console.WriteLine($"Distance {distance}");

                if (distance <= 2f)
                {
                    dashTimer = 0f;
                    dashCurrentVel = 0f;
                    isDashing = false;
                    PhysicsManager.ChangeCollisionTags(m_EntityId);
                }

            }
        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 == m_EntityId)
                return;
            if (str2 == "WALL")
                isDashing = false;
        }
    }
}
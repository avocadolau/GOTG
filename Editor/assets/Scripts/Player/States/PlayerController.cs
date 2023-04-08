using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = UInt64;
    public class PlayerController : Behaviour
    {
        public ComponentIterator statsIt;
        public ComponentIterator transformIt;
        public ComponentIterator rigidBodyIt;
        public ComponentIterator shooterIt;
        public ComponentIterator leftPosIt;
        public ComponentIterator rightPosIt;
        public StarlordShooter shoot;
        public Vector3 shootInput;
        public Vector3 movementInput;
        public Vector3 velocity;
 
        public virtual void Awake()
        {
            statsIt.componentId = Constants.WI_INVALID_INDEX;
            transformIt.componentIndex = Constants.WI_INVALID_INDEX;
            rigidBodyIt.componentId = Constants.WI_INVALID_INDEX;
            shooterIt.componentIndex = Constants.WI_INVALID_INDEX;
            leftPosIt.componentId = Constants.WI_INVALID_INDEX;
            rightPosIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        public virtual void Init()
        {
            statsIt = GetComponentIterator<Character>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();
            shooterIt = GetComponentIterator<StarlordShooter>();
        }
        public virtual void Update()
        {
            movementInput = GetMovementInput();
            shootInput = GetShootingInput();
        }
        public ref Character GetCharacter()
        {
            return ref GetComponentByIterator<Character>(statsIt); 
        }

        public ref Transform3D GetTransform()
        {
            return ref GetComponentByIterator<Transform3D>(transformIt);
        }

        public ref CollisionBody GetRigidBody()
        {
            return ref GetComponentByIterator<CollisionBody>(rigidBodyIt);
        }

        public EntityId GetEntity()
        {
            return m_EntityId;
        }
        public void ReceiveDamage(ref Character controller, int damage)
        {

        }

        public virtual void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
        }
        public Vector3 GetMovementInput()
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
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftX, GameState.GetControllerDeadzone());
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftY, GameState.GetControllerDeadzone());

            return input;
        }
        Vector3 GetShootingInput()
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
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightX, GameState.GetControllerDeadzone());
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightY, GameState.GetControllerDeadzone());

            return input;
        }

        public float AngleFromVec2(Vector2 vector)
        {
            return Mathf.Atan2(vector.x, vector.y) * Mathf.Rad2Deg;
        }
        void SetPlayerRotation(ref Vector3 currentRotation, Vector3 input, float rotationSpeed)
        {
            float angle = AngleFromVec2(new Vector2(input.x, input.z));


            currentRotation.y = Mathf.LerpAngle(currentRotation.y, angle, rotationSpeed);

            if (currentRotation.y >= 360f)
                currentRotation.y = 0f;
        }

        void Fire(Vector3 shootInput)
        {
            //EntityId leftPos = GetChildByName("LeftPos");
            //EntityId rigthPos = GetChildByName("RightPos");
            //leftPosIt = GetComponentIterator<Transform3D>(leftPos);
            //rightPosIt = GetComponentIterator<Transform3D>(rigthPos);
            //ref Character character = ref GetComponentByIterator<Character>(statsIt);
            //ref StarlordShooter shooter = ref GetComponentByIterator<StarlordShooter>(shooterIt);
            //ref Transform3D left = ref GetComponentByIterator<Transform3D>(leftPosIt);
            //ref Transform3D right = ref GetComponentByIterator<Transform3D>(rightPosIt);

            //Transform3D spawnPoint;
            ////Decide wich hand is going next
            //if (shooter.ShootRight)
            //{
            //    spawnPoint = right;
            //    Animator.PlayAnimationName("shootright", false, m_EntityId);
            //}
            //else
            //{
            //    spawnPoint = left;
            //    Animator.PlayAnimationName("shootleft", false, m_EntityId);
            //}
            ////if (isWalking)
            ////{
            ////    Animator.Blend("running", true, 1f, m_EntityId);
            ////}

            //shooter.ShootRight = !shooter.ShootRight;
            //SpawnBullet(spawnPoint, shooter, character, Mathf.CalculateForward(ref spawnPoint));
            //}
        }

        void SpawnBullet(Transform3D transform, StarlordShooter shooter, Character character, Vector3 bullDir)
        {
            Console.WriteLine("fired");

            string bulletName = "Bullet";

            EntityId bullet = CreateEntityNamed(bulletName);
            ref Transform3D playerTransform = ref GetComponent<Transform3D>();
            ref Transform3D bulletTransform = ref GetComponent<Transform3D>(bullet);
            ref BulletComponent bulletComp = ref AddComponent<BulletComponent>(bullet);
            ref CollisionBody cb = ref AddComponent<CollisionBody>(bullet);
            ref ColliderSphere cs = ref AddComponent<ColliderSphere>(bullet);

            AddMesh(bullet, "Player/Bullet/PlaneBullet", "Player/Bullet/defaultmaterial.wimaterial");

            bulletTransform.LocalPosition = transform.worldMatrix.GetPosition();
            bulletTransform.LocalRotation = new Vector3(-90f, 0f, playerTransform.LocalRotation.y + 90f);
            bulletTransform.LocalScale = transform.Scale;

            cs.radius = 1;
            cb.scalingOffset = new Vector3(1f, 1f, 1f);
            cb.isTrigger = true;
            cb.isStatic = false;
            cb.doContinuousCollision = false;
            cb.selfTag = 3;
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("ENEMY");
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("WALL");
            cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("COLUMN");

            bulletComp.Velocity = shooter.BulletSpeed;
            bulletComp.LifeTime = shooter.BulletLifeTime;
            bulletComp.Damage = character.Damage;
            bulletComp.Direction = bullDir;

            ApplySystem<MeshRenderer>(bullet);
            ApplySystem<PhysicsSystem>(bullet);
            ApplySystem<BulletController>(bullet);
            ApplySystem<AudioSystem>(bullet);

            Audio.PlaySound("player_shoot", m_EntityId);
        }
    }
}

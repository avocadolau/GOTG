using System;
using Wiwa;
namespace Game
{
    [Component]
    public struct CharacterShooterComponent
    {
        public float fireTimer;
        public float fireInterval;
        public float bulletScale;
        public float bulletLifeTime;
        public float bulletSpeed;
        public int bulletTag;
        public int collisionTag1;
        public int collisionTag2;
    }
    public class CharacterShooter : Behaviour
    {
        //Called the first frame
        private ComponentIterator transformIt;

        void Awake()
        {

        }
        //Called after Awake()
        void Init()
        {
            transformIt = GetComponentIterator<Transform3D>();
        }
        //Called every frame
        void Update()
        {
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref CharacterShooterComponent characterShoot = ref GetComponent<CharacterShooterComponent>();
            ref CharacterController characterCtrl = ref GetComponent<CharacterController>();

            Fire(ref characterShoot, transform.LocalRotation);
        }
        private void Fire(ref CharacterShooterComponent character, Vector3 bullDirection)
        {
            Console.WriteLine("Fire");
            float shootX = -Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightX, 0);
            float shootY = -Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.RightY, 0);

            character.fireTimer += Time.DeltaTime();

            Vector3 bullDir = new Vector3(shootX, 0, shootY);
            if (character.fireTimer >= character.fireInterval)
            {
                character.fireTimer = 0.0f;

                if (System.Math.Abs(shootX) > 0.5f || System.Math.Abs(shootY) > 0.5f)
                {
                    // Shoot sound


                    //PlayMusic("player_shoot");
                    //PlayAudioEvent("player_shoot");
                    //Vector3 bulletDir = new Vector3(shootX, 0, shootY);
                    SpawnBullet(ref character, new Vector3(0, 0, 0), bullDir, bullDirection.y);
                }
                else if (Input.IsKeyDown(KeyCode.Space))
                {
                    // Shoot sound

                    //PlayAudioEvent("player_shoot");
                    //Vector3 bulletDir = new Vector3(0, 0, 1);
                    SpawnBullet(ref character, new Vector3(0, 0, 0), bullDir, 0);
                }
            }
        }
        void SpawnBullet(ref CharacterShooterComponent character, Vector3 bullet_offset, Vector3 direction, float rot)
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
            //AddAudioSource(entity, "player_shoot", true, false);    ///Please add another sound
            //ApplySystem<AudioSystem>(entity);
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

        void InitCollisionFlags(ref CollisionBody rb, ref CharacterShooterComponent character)
        {
            //int bitsSelf = 0;
            //bitsSelf |= 1 << character.bulletTag;
            rb.selfTag = character.bulletTag;

            rb.filterBits |= 1 << character.collisionTag1;
            rb.filterBits |= 1 << character.collisionTag2;
        }
    }
}

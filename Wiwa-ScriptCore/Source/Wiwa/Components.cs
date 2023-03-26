namespace Wiwa
{
    public struct Constants
    {
        public const ulong WI_INVALID_INDEX = 18446744073709551615;
    }
    public struct ComponentIterator
    {
        public ulong componentId;
        public ulong componentIndex;
        public ulong componentSize;
    }

    #region Transform3D
    [Component]
    public struct Transform3D
    {
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale;

        public Vector3 LocalPosition;
        public Vector3 LocalRotation;
        public Vector3 LocalScale;

        //public Matrix4x4 worldMatrix;
        //public Matrix4x4 localMatrix;
        public Vector4 world_mat_r1;
        public Vector4 world_mat_r2;
        public Vector4 world_mat_r3;
        public Vector4 world_mat_r4;

        public Vector4 local_mat_r1;
        public Vector4 local_mat_r2;
        public Vector4 local_mat_r3;
        public Vector4 local_mat_r4;
    }
    #endregion
    #region CollisionBody
    [Component]
    public struct CollisionBody
    {
        public Vector3 positionOffset;
        public Vector3 scalingOffset;
        public bool isTrigger;
        public bool isStatic;
        public bool doContinuousCollision;
        public int selfTag;
        public int filterBits;

    }
    [Component]
    public struct ColliderCube
    {
        public Vector3 halfExtents;
    }

    [Component]
    public struct ColliderCylinder
    {
        public float height;
        public float radius;
    }

    [Component]
    public struct ColliderSphere
    {
        public float radius;
    }
    #endregion
    #region Mesh
    //[Component]
    //public unsafe struct Mesh
    //{
    //    public ResourceId meshId;
    //    public ResourceId materialId;
    //    public bool drawChildren;
    //    public ulong modelIndex;
    //    public fixed char mesh_path[128];
    //    public fixed char mat_path[128];
    //}
    #endregion
    #region Utils
    [Component]
    public struct TestComponent
    {
        public int val1;
        public float val2;
    }
    #endregion

    #region Game
    [Component]
    public struct Wave
    {
        public int maxEnemies;
        public int currentEnemiesAlive;
        public bool hasFinished;
    }
    [Component]
    public struct Character
    {
        public int MaxHealth;
        public int Health;
        public int MaxShield;
        public int Shield;
        public int Damage;

        public float RateOfFire;
        public float Speed;
        public float DashDistance;
        public float DashSpeed;
        public float DashCooldown;
        public float WalkTreshold;
    }
    //[Component]
    //public struct EnemySpawner
    //{
    //    public int maxEnemiesPerWave;
    //    public int maxWaveCount;
    //    public int currentWaveCount;
    //    public float timeBetweenWaves;
    //    public bool hasFinished;
    //}
    //[Component]
    //public struct CharacterController
    //{
    //    public float velocity;
    //    public float camYOffset;
    //    public float camXOffset;
    //    public float camZOffset;

    //    public float camXAngle;
    //    public float camYAngle;
    //    public float camZAngle;

    //    public float fireInterval;
    //    public float fireTimer;

    //    public float bulletLifeTime;
    //    public float bulletSpeed;
    //    public float bulletScale;

    //    public int bulletTag;
    //    public int collisionTag1;
    //}
    #endregion
    #region AgentAI
    [Component]
    public struct AgentAI
    {
        public float speed;
        public Vector3 target;
    }

    #endregion
    #region ParticleEmitter
    [Component]
    public struct ParticleEmitter
    {
        public bool isPlaying;
    }
    #endregion
}
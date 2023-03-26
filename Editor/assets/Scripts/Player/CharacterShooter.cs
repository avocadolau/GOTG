using Wiwa;
namespace Game
{
    [Component]
    public struct StarlordShooter
    {
        public float BulletLifeTime;
        public float BulletSpeed;

        public Vector3 LeftSpawnPos;
        public Vector3 RightSpawnPos;

        public bool ShootRight;
    }
}

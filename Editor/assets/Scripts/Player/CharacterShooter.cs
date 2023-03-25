using Wiwa;
namespace Game
{
    [Component]
    public struct CharacterShooter
    {
        public float FireInterval;
        public float BulletLifeTime;
        public float BulletSpeed;
        public int BulletDamage;

        public Vector3 LeftSpawnPos;
        public Vector3 RightSpawnPos;

        public bool ShootRight;
    }
}

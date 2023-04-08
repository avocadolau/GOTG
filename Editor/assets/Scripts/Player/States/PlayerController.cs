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
        public Character stats;
        public Transform3D transform;
        public CollisionBody rb;
        public StarlordShooter shoot;
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
           transform = GetComponentByIterator<Transform3D>(transformIt);
           stats = GetComponentByIterator<Character>(statsIt);
           rb =  GetComponentByIterator<CollisionBody>(rigidBodyIt);

        }
        public void ReceiveDamage(ref Character controller, int damage)
        {

        }

        public virtual void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
        }
    }
}

using System;
using Wiwa;
namespace Game
{
    [Component]
    public struct MeleeAttack
    {
        public float duration;
        public int damage;
        public bool hasFinished;
    }
    public class MeleeAttackSystem : Behaviour
    {
        private ComponentIterator meleeAttackIt;
        private ComponentIterator transformIt;
        private ComponentIterator colliderIt;
        public float timer;

        void Awake()
        {
            meleeAttackIt.componentId = Constants.WI_INVALID_INDEX;
            meleeAttackIt.componentIndex = Constants.WI_INVALID_INDEX;
            transformIt.componentId = Constants.WI_INVALID_INDEX;
            transformIt.componentIndex = Constants.WI_INVALID_INDEX;
            colliderIt.componentId = Constants.WI_INVALID_INDEX;
            colliderIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {
            meleeAttackIt = GetComponentIterator<MeleeAttack>();
            transformIt = GetComponentIterator<Transform3D>();
            colliderIt = GetComponentIterator<CollisionBody>();
            TriggerAttack();
        }

        void Update()
        {

        }

        void TriggerAttack()
        {

        }
    }
}

using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerDeath: PlayerBaseState
    {
        public override void EnterState(ref PlayerController controller, EntityId entityId)
        {
            Animator.PlayAnimationName("death", false, m_EntityId);
        }
        public override void UpdateState(ref PlayerController controller, EntityId entityId)
        {
        }
        public override void ExitState(ref PlayerController controller, EntityId entityId)
        {
        }
        public override void OnCollisionEnter(ref PlayerController controller, EntityId id1, EntityId id2, string str1, string str2)
        {

        }
    }
}
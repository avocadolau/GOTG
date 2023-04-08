using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerDash : PlayerBaseState
    {
        private float dashTimer = 0f;
        public override void EnterState(ref PlayerController controller, EntityId entityId)
        {
            dashTimer = stats.DashCooldown;
            Animator.PlayAnimationName("dash", false, m_EntityId);
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
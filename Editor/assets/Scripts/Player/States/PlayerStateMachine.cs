using System;
using System.Linq;
using Wiwa;
namespace Game
{
    using EntityId = UInt64;
    class PlayerStateMachine : PlayerController
    {
        PlayerBaseState currentState;
        public PlayerIdle idle = new PlayerIdle();
        public PlayerMove move = new PlayerMove();
        public PlayerMove attack = new PlayerMove();
        public PlayerDash dash = new PlayerDash();
        public PlayerDeath death = new PlayerDeath();
        public override void Awake()
        {
            base.Awake();
        }

        public override void Init()
        {
            base.Init();
            currentState = idle;
            PlayerController self = this;
            currentState.EnterState(ref self, m_EntityId);
        }

        public override void Update()
        {


            base.Update();
            PlayerController self = this;
            currentState.UpdateState(ref self, m_EntityId);
            CheckHealth();
        }

        public override void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            base.OnCollisionEnter(id1, id2, str1, str2);
            PlayerController self = this;
            currentState.OnCollisionEnter(ref self, id1, id2, str1, str2);
        }

        public void SwitchState(PlayerBaseState state)
        {
            PlayerController self = this;
            currentState.ExitState(ref self, m_EntityId);
            currentState = state;
            currentState.EnterState(ref self, m_EntityId);
        }
        void CheckHealth()
        {
            if(stats.Health <= 0)
            {
                //moriste
            }    
        }
    }
}

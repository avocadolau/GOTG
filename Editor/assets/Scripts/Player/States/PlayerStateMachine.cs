using System;
using System.Linq;
using Wiwa;
namespace Game
{
    using EntityId = UInt64;
    public class PlayerStateMachine : PlayerController
    {
        PlayerBaseState currentState;
        public PlayerIdle idle = new PlayerIdle();
        public PlayerMove move = new PlayerMove();
        public PlayerAttack attack = new PlayerAttack();
        public PlayerDash dash = new PlayerDash();
        public PlayerDeath death = new PlayerDeath();
        
        public override void Awake()
        {
            base.Awake();
            Console.WriteLine("awake player state machine");
            GameState.SetPlayer(m_EntityId, m_Scene);
            GameState.LoadPlayerProgression();
        }

        public override void Init()
        {
            base.Init();
            currentState = idle;
            PlayerStateMachine self = this;
            currentState.EnterState(ref self, GetEntity());
        }

        public override void Update()
        {
            base.Update();
            PlayerStateMachine self = this;
            currentState.UpdateState(ref self, GetEntity());
            CheckHealth();
            DashCooldown();
        }

        public override void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            base.OnCollisionEnter(id1, id2, str1, str2);
            currentState.OnCollisionEnter(id1, id2, str1, str2);
        }

        public void SwitchState(ref PlayerStateMachine stateMachine, PlayerBaseState state)
        {
            PlayerStateMachine self = this;
            currentState.ExitState(ref self, GetEntity());
            currentState = state;
            currentState.EnterState(ref self, GetEntity());
        }
        void CheckHealth()
        {       
            if (GetCharacter().Health <= 0)
            {
                PlayerStateMachine self = this;
                SwitchState(ref self, death);
            }
        }

        void DashCooldown()
        {
            if(!DashEnable)
            {
                //count cooldown
                cooldownTimer -= Time.DeltaTime();
                if (cooldownTimer <= 0)
                    DashEnable = true;
            }
        }
    }
}

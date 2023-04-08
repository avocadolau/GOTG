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
        public PlayerMove attack = new PlayerMove();
        public PlayerDash dash = new PlayerDash();
        public PlayerDeath death = new PlayerDeath();
        
        public void Awake()
        {
            Console.WriteLine("awake player state machine");
        }

        public void Init()
        {
            currentState = idle;
            PlayerStateMachine self = this;
            currentState.EnterState(ref self, GetEntity());
        }

        public  void Update()
        {
            PlayerStateMachine self = this;
            currentState.UpdateState(ref self, GetEntity());
            CheckHealth();
        }

        public void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
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
    }
}

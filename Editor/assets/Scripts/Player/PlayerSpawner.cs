//using System;
//using Wiwa;

//namespace Game
//{
//    using EntityId = UInt64;

//    class PlayerSpawner : Behaviour
//    {

//        void Init()
//        {
//            ref Transform3D tr = ref GetComponent<Transform3D>();

//            EntityId playerId = LoadPrefab("assets\\Player\\Final\\Player.wiprefab");
//            ref Transform3D playerTr = ref GetComponent<Transform3D>(playerId);

//            playerTr.LocalPosition = tr.worldMatrix.GetPosition();
//        }
//    }
//}

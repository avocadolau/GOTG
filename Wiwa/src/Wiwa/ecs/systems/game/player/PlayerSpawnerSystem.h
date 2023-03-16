//#pragma once
//#include "../../System.h"
//
//#include <Wiwa/utilities/Reflection.h>
//
////class CharacterController
////{
////public:
////	float velocity;
////	float camYOffset;
////	float camXOffset;
////	float camZOffset;
////
////	float camXAngle;
////	float camYAngle;
////	float camZAngle;
////
////	float fireInterval;
////	float fireTimer;
////
////	float bulletLifeTime;
////	float bulletSpeed;
////	float bulletScale;
////
////	int bulletTag;
////	int collisionTag1;
////};
//
//namespace Wiwa {
//	class WI_API PlayerSpawnerSystem : public System {
//	private:
//		EntityManager::ComponentIterator m_CharacterData;
//		EntityManager::ComponentIterator m_Player;
//		EntityId m_PlayerId;
//	public:
//		PlayerSpawnerSystem();
//		~PlayerSpawnerSystem();
//
//		void OnAwake() override;
//
//		void OnInit() override;
//
//		void OnUpdate() override;
//
//		void OnDestroy() override;
//
//		void CreatePlayer(Character stats);
//	};
//}
//
//REGISTER_SYSTEM(Wiwa::PlayerSpawnerSystem);
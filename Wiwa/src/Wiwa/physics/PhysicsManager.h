#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/RigidBody.h>
#include <Wiwa/ecs/components/ColliderCube.h>
#include <Wiwa/ecs/components/ColliderCylinder.h>
#include <Wiwa/ecs/components/ColliderSphere.h>
#include <Wiwa/ecs/components/ColliderCapsule.h>
#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/Uniforms.h>
#include <Wiwa/core/Resources.h>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include <vector>
#include <bitset>
#include <algorithm>

#define MAX_BITS 32
// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f)
class DebugDrawer;
class Camera;
namespace Wiwa {

	struct MyRigidBody {
		MyRigidBody(btRigidBody& body_, const size_t id_) : btBody(&body_), id(id_) {};

		btRigidBody* btBody;
		size_t id;
	};

	enum CollisionType {
		CT_ENTER,
		CT_LOOP,
		CT_LEAVE,
		CT_NONE
	};

	struct CollisionData {
		size_t entityA;
		size_t entityB;
		CollisionType collisionType;
	};

	class WI_API PhysicsManager
	{
	public:
		PhysicsManager();

		~PhysicsManager();

		// Init & Delete World
		bool InitWorld();

		bool StepSimulation();

		bool UpdateEngineToPhysics();

		bool UpdatePhysicsToEngine();

		bool CleanWorld();

		bool DeleteBody(MyRigidBody* body);

		// Add bodies
		bool AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCapsule(size_t id, const Wiwa::ColliderCapsule& capsule, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		// Manipulate bodies
		void SetBodyMass(MyRigidBody* body, const float mass);

		void SetBodyGravity(MyRigidBody* body, const btVector3 gravity);

		void SetTrigger(MyRigidBody* body, const bool isTrigger);

		void SetStatic(MyRigidBody* body, const bool isStatic);

		MyRigidBody* FindByEntityId(size_t id);

		void ManipulateBody(MyRigidBody* body, const btVector3& vector);

		void UpdateCollisionType(size_t first, size_t second);

		bool OnSave();

		bool OnLoad();

		bool getInit();

		void DebugDrawWorld();

		bool AddFilterTag(const char* str);
		
		void RemoveFilterTag(const int index);

	private:
		bool m_Debug;
		bool m_HasBeenInit;
		btDiscreteDynamicsWorld* m_World;

		btDefaultCollisionConfiguration* m_Collision_conf;
		btCollisionDispatcher* m_Dispatcher;
		btBroadphaseInterface* m_Broad_phase;
		btSequentialImpulseConstraintSolver* m_Solver;

		btOverlapFilterCallback* m_filterCallback;

		//  btDefaultVehicleRaycaster* v_Vehicle_raycaster;
		DebugDrawer* m_Debug_draw;

		std::list<btCollisionShape*> m_Shapes;
		std::list<MyRigidBody*> m_Bodies;
		std::list<btDefaultMotionState*> m_Motions;
		std::list<btTypedConstraint*> m_Constraints;

		std::vector<CollisionData> m_CollisionList;

	private:
		std::list<MyRigidBody*> m_BodiesToLog;

	public:
		bool AddBodyToLog(MyRigidBody* body_to_log);
		bool RemoveBodyFromLog(MyRigidBody* body_to_log);
		bool LogBodies();

	public:
		std::vector<std::string> filterStrings;
		std::vector<std::bitset<MAX_BITS>> fliterBitsSets;

		void bin(unsigned n)
		{
			if (n > 1)
				bin(n >> 1);

			WI_INFO("{}", n & 1);
		}
	};
}

// write your own collision group / mask functionality using broadphase callbacks rather than use Bullet's
struct CustomFilterCallBack : public btOverlapFilterCallback
{
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
		collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

		//add some additional logic here that modified 'collides'


		return true;
	}
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer()
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;

	ResourceId lineDisplayShaderId;
	Wiwa::Shader* lineDisplayShader;
	DefaultUnlitUniforms lineDisplayShaderUniforms;
};
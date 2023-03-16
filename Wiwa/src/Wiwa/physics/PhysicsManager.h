#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/RigidBody.h>
#include <Wiwa/ecs/components/ColliderCube.h>
#include <Wiwa/ecs/components/ColliderCylinder.h>
#include <Wiwa/ecs/components/ColliderSphere.h>
#include <Wiwa/ecs/components/ColliderCapsule.h>
#include <Wiwa/ecs/components/RayCast.h>
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
#define SUB_STEPS 3
class DebugDrawer;
class Camera;
namespace Wiwa {

	struct Object {
		Object(btCollisionObject& body_, const size_t id_, const bool doContinuousCollision_ = false) : collisionObject(&body_), velocity(0.0f, 0.0f, 0.0f), id(id_), doContinuousCollision(doContinuousCollision_){};

		btCollisionObject* collisionObject;
		btVector3 velocity;
		size_t id;
		bool doContinuousCollision;
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

		bool ResolveContacts();

	private:
		bool ResolveContactA(btCollisionObject* obj_a, const btVector3& vec, const bool is_static, const bool is_trigger_a, const bool is_trigger_b);
		bool ResolveContactB(btCollisionObject* obj_a, const btVector3& vec, const bool is_static, const bool is_trigger_a, const bool is_trigger_b);
	public:

		bool UpdateObjects(const float dt);

		bool UpdateEngineToPhysics();

		bool UpdatePhysicsToEngine();

		bool CleanWorld();

		bool DeleteBody(Object* body);

		// Add bodies
		bool AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCapsule(size_t id, const Wiwa::ColliderCapsule& capsule, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);
	private:
		bool AddBodyInternal(size_t id, btCollisionObject* collision_object, btCollisionShape* collision_shape, Wiwa::Rigidbody& rigid_body);
	public:

		// Manipulate bodies
		bool SetVelocity(Object*body, const glm::vec3 velocity);

		bool SetRotation(Object* body, const glm::vec3 euler_angles);

		//void SetTrigger(MyObject* body, const bool isTrigger);

		Object* FindByEntityId(size_t id);

		void UpdateCollisionType(size_t first, size_t second);

		bool OnSave();

		bool OnLoad();

		bool getInit();

		// Query world
		void DebugDrawWorld();

		bool AddFilterTag(const char* str);
		
		void RemoveFilterTag(const int index);

		void RayTest(const btVector3& ray_from_world, const btVector3& ray_to_world);

	private:
		bool m_Debug;
		bool m_HasBeenInit;
		btCollisionWorld* m_World;

		btDefaultCollisionConfiguration* m_Collision_conf;
		btCollisionDispatcher* m_Dispatcher;
		btBroadphaseInterface* m_Broad_phase;
		btSequentialImpulseConstraintSolver* m_Solver;

		btOverlapFilterCallback* m_filterCallback;

		//  btDefaultVehicleRaycaster* v_Vehicle_raycaster;
		DebugDrawer* m_Debug_draw;

		std::list<btCollisionShape*> m_Shapes;
		std::list<Object*> m_CollObjects;
		//std::list<btDefaultMotionState*> m_Motions;
		std::list<btTypedConstraint*> m_Constraints;

		std::vector<CollisionData> m_CollisionList;

	private:
		std::list<Object*> m_BodiesToLog;

	public:
		bool AddBodyToLog(Object* body_to_log);
		bool RemoveBodyFromLog(Object* body_to_log);
		bool LogBodies();

	public:
		std::vector<std::string> filterStrings;
		std::vector<std::bitset<MAX_BITS>> fliterBitsSets;

		/*void bin(unsigned n)
		{
			if (n > 1)
				bin(n >> 1);

			WI_INFO("{}", n & 1);
		}*/

		static void bin(int n, const char* str)
		{
			std::cout << std::endl << std::endl;
			int i;
			std::cout << str << " --> " << "0";
			for (i = 1 << 30; i > 0; i = i / 2)
			{
				if ((n & i) != 0)
				{
					std::cout << "1";
				}
				else
				{
					std::cout << "0";
				}
			}
		}
	};
}

// write your own collision group / mask functionality using broadphase callbacks rather than use Bullet's
struct CustomFilterCallBack : public btOverlapFilterCallback
{
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const;
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
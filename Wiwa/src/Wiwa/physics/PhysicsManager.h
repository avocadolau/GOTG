#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/RigidBody.h>
#include <Wiwa/ecs/components/ColliderCube.h>
#include <Wiwa/ecs/components/ColliderCylinder.h>
#include <Wiwa/ecs/components/ColliderSphere.h>
#include "Wiwa/ecs/components/Transform3D.h"

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include <vector>
// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f)

namespace Wiwa {
	class DebugDrawer;

	class WI_API PhysicsManager
	{
		struct ObjectData
		{
			Wiwa::Rigidbody* rigidBody;
			Wiwa::Transform3D* transform3d;
			size_t id;
		};

	public:
		PhysicsManager();

		~PhysicsManager();

		bool InitWorld();

		bool StepSimulation();

		bool UpdateEngineToPhysics();

		bool UpdatePhysicsToEngine();

		bool CleanWorld();

		bool DeleteBody(btRigidBody* body);

		bool AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		btRigidBody* FindByEntityId(size_t id);

		inline glm::vec3 WiwaToGLM(Wiwa::Vector3f vector)
		{
			return glm::vec3(vector.x, vector.y, vector.z);
		}

		bool getInit();

		bool LogBodies();

	private:
		bool m_Debug;
		bool m_HasBeenInit;
		btDiscreteDynamicsWorld* m_World;

		btDefaultCollisionConfiguration* m_Collision_conf;
		btCollisionDispatcher* m_Dispatcher;
		btBroadphaseInterface* m_Broad_phase;
		btSequentialImpulseConstraintSolver* m_Solver;

		//  btDefaultVehicleRaycaster* v_Vehicle_raycaster;
		DebugDrawer* m_Debug_draw;

		std::list<btCollisionShape*> m_Shapes;
		std::list<btRigidBody*> m_Bodies;
		std::list<btDefaultMotionState*> m_Motions;
		std::list<btTypedConstraint*> m_Constraints;
	};

	class DebugDrawer : public btIDebugDraw
	{
	public:
		DebugDrawer()
		{}

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		void drawContactPoint(const btVector3& point_onB, const btVector3& normal_onB, btScalar distance, int life_time, const btVector3& color);
		void reportErrorWarning(const char* warning_string);
		void draw3dText(const btVector3& location, const char* text_string);
		void setDebugMode(int debug_mode);
		int  getDebugMode() const;

		DebugDrawModes mode;
	};

}



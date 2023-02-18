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

class DebugDrawer;

class WI_API PhysicsManager
{
private:
	PhysicsManager();

	struct ObjectData
	{
		Wiwa::Rigidbody* rigidBody;
		Wiwa::Transform3D* transform3d;
		size_t id;
	};

public:
	~PhysicsManager();

	static bool InitWorld();

	static bool StepSimulation();

	static bool UpdateWorld();

	static bool UpdateEngineToPhysics();
	
	static bool UpdatePhysicsToEngine();

	static bool CleanWorld();

	static bool AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

	static bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

	static bool AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

private:
	static bool s_Debug;

	static btDiscreteDynamicsWorld* s_World;

	static btDefaultCollisionConfiguration* s_Collision_conf;
	static btCollisionDispatcher* s_Dispatcher;
	static btBroadphaseInterface* s_Broad_phase;
	static btSequentialImpulseConstraintSolver* s_Solver;

	//static btDefaultVehicleRaycaster* v_Vehicle_raycaster;
	static DebugDrawer* s_Debug_draw;

	static std::vector<btCollisionShape*> s_Shapes;
	static std::vector<btRigidBody*> s_Bodies;
	static std::vector<btDefaultMotionState*> s_Motions;
	static std::vector<btTypedConstraint*> s_Constraints;
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

	 DebugDrawModes s_Mode;
};

inline glm::vec3 WiwaToGLM(Wiwa::Vector3f vector);

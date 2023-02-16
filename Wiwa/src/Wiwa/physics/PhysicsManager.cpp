#include <wipch.h>
#include "PhysicsManager.h"
#include <glew.h>
#include <Wiwa/utilities/Log.h>
#include "Wiwa/utilities/time/Time.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

bool PhysicsManager::s_Debug = true;

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::InitWorld()
{
	s_Collision_conf = new btDefaultCollisionConfiguration();
	s_Dispatcher = new btCollisionDispatcher(s_Collision_conf);
	s_Broad_phase = new btDbvtBroadphase();
	s_Solver = new btSequentialImpulseConstraintSolver();
	s_Debug_draw = new DebugDrawer();

	s_World = new btDiscreteDynamicsWorld(s_Dispatcher, s_Broad_phase, s_Solver, s_Collision_conf);
	s_Debug_draw->setDebugMode(s_Debug_draw->DBG_MAX_DEBUG_DRAW_MODE);
	s_World->setDebugDrawer(s_Debug_draw);
	s_World->setGravity(GRAVITY * 2);

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		s_World->addRigidBody(body);

	}

	return true;
}

bool PhysicsManager::UpdateSimulation()
{
	s_World->stepSimulation(Wiwa::Time::GetDeltaTime(), 15);

	int numManifolds = s_World->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = s_World->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			//ComponentCollider* pbodyA = (ComponentCollider*)obA->getUserPointer();
			//ComponentCollider* pbodyB = (ComponentCollider*)obB->getUserPointer();

			//if (pbodyA && pbodyB)
			//{
			//	//std::list<Module*>* item = pbodyA->collision_listeners.getFirst();
			//	for (std::list<Module*>::iterator item = pbodyA->m_Collision_listeners.begin(); item != pbodyA->m_Collision_listeners.end(); item++)
			//	{
			//		(*item)->OnCollision(pbodyA, pbodyB);
			//	}

			//	for (std::list<Module*>::iterator item2 = pbodyB->m_Collision_listeners.begin(); item2 != pbodyB->m_Collision_listeners.end(); item2++)
			//	{
			//		(*item2)->OnCollision(pbodyB, pbodyA);
			//	}
			//}
		}
	}
	return true;
}

bool PhysicsManager::UpdateWorld()
{
	// Iterate each component rigidbody and get transform component
	for (std::vector<btRigidBody*>::iterator item = s_Bodies.begin(); item != s_Bodies.end(); item++)
	{
		Wiwa::Transform3D* transform = (Wiwa::Transform3D*)(*item)->getUserPointer();
		
		btTransform offsetedTransform((*item)->getWorldTransform());

		// Get the transform fromn physyics
		btVector3 posBullet = offsetedTransform.getOrigin();
		btQuaternion rotBullet = offsetedTransform.getRotation();

		glm::quat rotEngine = glm::quat(rotBullet.w(), rotBullet.x(), rotBullet.y(), rotBullet.z());
		glm::vec3 finalOffset = rotEngine * m_PositionOffset;
		glm::vec3 posEngine = glm::vec3(posBullet.x() - finalOffset.x, posBullet.y() - finalOffset.y, posBullet.z() - finalOffset.z);

		// Remove the offset because offset is internal only(collider wise)
		offsetedTransform.setOrigin(btVector3(posEngine.x, posEngine.y, posEngine.z));
		offsetedTransform.setRotation(rotBullet);
		offsetedTransform.getOpenGLMatrix(glm::value_ptr(mat));

		transform.
	}
	return true;
}

bool PhysicsManager::CleanWorld()
{
	// Remove from the world all collision bodies
	for (int i = s_World->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = s_World->getCollisionObjectArray()[i];
		s_World->removeCollisionObject(obj);
	}

	//for (std::list<btTypedConstraint*>::iterator item = constraints.begin(); item != constraints.end(); item++)
	//{
	//	ComponentCollider* collA = (ComponentCollider*)(*item)->getRigidBodyA().getUserPointer();
	//	ComponentCollider* collB = (ComponentCollider*)(*item)->getRigidBodyB().getUserPointer();
	//	ComponentConstraint* constr = collA->m_GameObject->GetConstraint();
	//	collA->m_GameObject->DeAssignComponent(ComponentType::CONSTRAINT);
	//	collB->m_GameObject->DeAssignComponent(ComponentType::CONSTRAINT);
	//	delete constr;
	//	world->removeConstraint((*item));
	//	delete* item;
	//	*item = nullptr;
	//}

	/*constraints.clear();*/

	for(std::vector<btDefaultMotionState*>::iterator item = s_Motions.begin(); item != s_Motions.end(); item++)
	{
		delete *item;
		*item = nullptr;
	}

	s_Motions.clear();

	for (std::vector<btCollisionShape*>::iterator item = s_Shapes.begin(); item != s_Shapes.end(); item++)
	{
		delete* item;
		*item = nullptr;
	}

	s_Shapes.clear();

	for (std::vector<btRigidBody*>::iterator item = s_Bodies.begin(); item != s_Bodies.end(); item++)
	{
		delete (ObjectData*)(*item)->getUserPointer();
		delete* item;
		*item = nullptr;
	}

	s_Bodies.clear();

	delete s_Debug_draw;
	delete s_Solver;
	delete s_Broad_phase;
	delete s_Dispatcher;
	delete s_Collision_conf;

	return true;
}

bool PhysicsManager::AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform,  Wiwa::Rigidbody& rigid_body)
{
	btCollisionShape* colShape = new btSphereShape(sphere.radius);
	s_Shapes.push_back(colShape);
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

	btVector3 localInertia(0, 0, 0);
	if (rigid_body.mass != 0.f)
		colShape->calculateLocalInertia(rigid_body.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	s_Motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid_body.mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);

	ObjectData* data = new ObjectData();
	data->rigidBody = &rigid_body;
	data->transform3d = &transform;
	data->id = id;
	
	s_World->addRigidBody(body);
	s_Bodies.push_back(body);
	return true;
}

bool PhysicsManager::AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 Scaling;
	glm::quat Rotation;
	glm::vec3 Translation;
	glm::decompose(transform.worldMatrix, Scaling, Rotation, Translation, skew, perspective);
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.halfExtents.x, cube.halfExtents.y, cube.halfExtents.z));
	s_Shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

	btVector3 localInertia(0, 0, 0);
	if (rigid_body.mass != 0.f)
		colShape->calculateLocalInertia(rigid_body.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	s_Motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid_body.mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);

	ObjectData* data = new ObjectData();
	data->rigidBody = &rigid_body;
	data->transform3d = &transform;
	data->id = id;

	s_World->addRigidBody(body);
	s_Bodies.push_back(body);

	return true;
}

bool PhysicsManager::AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height * 0.5f, cylinder.radius, 0.0f));
	s_Shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

	btVector3 localInertia(0, 0, 0);
	if (rigid_body.mass != 0.f)
		colShape->calculateLocalInertia(rigid_body.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	s_Motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid_body.mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);

	ObjectData* data = new ObjectData();
	data->rigidBody = &rigid_body;
	data->transform3d = &transform;
	data->id = id;

	s_World->addRigidBody(body);
	s_Bodies.push_back(body);

	return true;
}



// =============================================

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glUseProgram(0);
	glColor3f(0, 0, 255);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
	glLineWidth(1.0f);
}

void DebugDrawer::drawContactPoint(const btVector3& point_onB, const btVector3& normal_onB, btScalar distance, int life_time, const btVector3& color)
{
	//point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	//point.color.Set(color.getX(), color.getY(), color.getZ());
	//point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warning_string)
{
	WI_INFO("Bullet warning: {}", warning_string);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* text_string)
{
	WI_INFO("Bullet draw text: {}", text_string);
}

void DebugDrawer::setDebugMode(int debug_mode)
{
	s_Mode = (DebugDrawModes)debug_mode;
}

int DebugDrawer::getDebugMode() const
{
	return s_Mode;
}

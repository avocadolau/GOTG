#include <wipch.h>
#include "PhysicsManager.h"
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"

#include <glew.h>

#include <Wiwa/utilities/Log.h>
#include "Wiwa/utilities/time/Time.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Wiwa {
	PhysicsManager::PhysicsManager()
	{
		m_HasBeenInit = false;
	}

	PhysicsManager::~PhysicsManager()
	{
	}

	bool PhysicsManager::InitWorld()
	{
		m_Debug = true;
		m_Collision_conf = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_Collision_conf);
		m_Broad_phase = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver();
		m_Debug_draw = new DebugDrawer();

		m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broad_phase, m_Solver, m_Collision_conf);

		m_Debug_draw->setDebugMode(m_Debug_draw->DBG_MAX_DEBUG_DRAW_MODE);
		m_World->setDebugDrawer(m_Debug_draw);
		m_World->setGravity(GRAVITY * 2);

		// Big plane as ground
		{
			btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

			btDefaultMotionState* myMotionState = new btDefaultMotionState();
			btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

			btRigidBody* body = new btRigidBody(rbInfo);
			m_World->addRigidBody(body);

		}

		m_HasBeenInit = true;
		return true;
	}

	bool PhysicsManager::StepSimulation()
	{
		m_World->stepSimulation(Wiwa::Time::GetDeltaTime(), 15);

		int numManifolds = m_World->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = m_World->getDispatcher()->getManifoldByIndexInternal(i);
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

	bool PhysicsManager::UpdateEngineToPhysics()
	{
		// Set the position offset
		// Get the position from the engine
		for (std::list<btRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			ObjectData* entityData = (ObjectData*)(*item)->getUserPointer();

			// Get the position from the engine
			glm::vec3 posEngine = WiwaToGLM(entityData->transform3d->localPosition);
			glm::quat rotEngine = glm::quat(entityData->transform3d->localMatrix);
			//glm::quat rotEngine = entityData->transform3d->rotation; old

			// Get the offset
			glm::vec3 finalOffset = rotEngine * entityData->rigidBody->positionOffset;
			glm::vec3 finalPosBullet = posEngine + finalOffset;

			// Apply the offset because offset, it is internal only(collider wise)
			btTransform offsettedCollider;
			offsettedCollider.setFromOpenGLMatrix(glm::value_ptr(entityData->transform3d->localMatrix));
			offsettedCollider.setOrigin(btVector3(finalPosBullet.x, finalPosBullet.y, finalPosBullet.z));
			offsettedCollider.setRotation(btQuaternion(rotEngine.x, rotEngine.y, rotEngine.z, rotEngine.w));

			(*item)->setWorldTransform(offsettedCollider);
		}

		return true;
	}

	bool PhysicsManager::UpdatePhysicsToEngine()
	{
		// Physics to Engine
		for (std::list<btRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			ObjectData* entityData = (ObjectData*)(*item)->getUserPointer();

			btTransform bulletTransform((*item)->getWorldTransform());

			// Get the transform from physics world
			btVector3 posBullet = bulletTransform.getOrigin();
			btQuaternion rotationBullet = bulletTransform.getRotation();

			glm::quat rotationEngine = glm::quat(rotationBullet.w(), rotationBullet.x(), rotationBullet.y(), rotationBullet.z());
			glm::vec3 finalOffset = rotationEngine * entityData->rigidBody->positionOffset;
			glm::vec3 posEngine = glm::vec3(posBullet.x() - finalOffset.x, posBullet.y() - finalOffset.y, posBullet.z() - finalOffset.z);

			// Remove the offset because offset is internal only(collider wise)
			bulletTransform.setOrigin(btVector3(posEngine.x, posEngine.y, posEngine.z));
			bulletTransform.setRotation(rotationBullet);
			bulletTransform.getOpenGLMatrix(glm::value_ptr(entityData->transform3d->localMatrix));

			(*item)->getCollisionShape()->setLocalScaling((btVector3(entityData->rigidBody->scalingOffset.x, entityData->rigidBody->scalingOffset.y, entityData->rigidBody->scalingOffset.z)));
			m_World->updateSingleAabb((*item));
		}

		return true;
	}

	bool PhysicsManager::CleanWorld()
	{
		// Remove from the world all collision bodies
		for (int i = m_World->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_World->getCollisionObjectArray()[i];
			m_World->removeCollisionObject(obj);
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

		for (std::list<btDefaultMotionState*>::iterator item = m_Motions.begin(); item != m_Motions.end(); item++)
		{
			delete* item;
			*item = nullptr;
		}

		m_Motions.clear();

		for (std::list<btCollisionShape*>::iterator item = m_Shapes.begin(); item != m_Shapes.end(); item++)
		{
			delete* item;
			*item = nullptr;
		}

		m_Shapes.clear();

		for (std::list<btRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			delete (ObjectData*)(*item)->getUserPointer();
			delete* item;
			*item = nullptr;
		}

		m_Bodies.clear();

		delete m_Debug_draw;
		delete m_Solver;
		delete m_Broad_phase;
		delete m_Dispatcher;
		delete m_Collision_conf;
		delete m_World;

		m_HasBeenInit = false;
		return true;
	}

	bool PhysicsManager::DeleteBody(btRigidBody* body)
	{
		//int constrainNums = body->m_Body->getNumConstraintRefs();
		//if (constrainNums > 0)
		//{
		//	for (int i = 0; i < constrainNums; i++)
		//	{
		//		constraints.remove(body->m_Body->getConstraintRef(i));
		//		world->removeConstraint(body->m_Body->getConstraintRef(i));
		//		delete body->m_Body->getConstraintRef(i);
		//	}
		//}

		m_Motions.remove((btDefaultMotionState*)body->getMotionState());
		delete body->getMotionState();

		m_Shapes.remove(body->getCollisionShape());
		delete body->getCollisionShape();

		m_World->removeRigidBody(body);
		delete body;
		return true;
	}

	bool PhysicsManager::AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		btCollisionShape* colShape = new btSphereShape(sphere.radius);
		m_Shapes.push_back(colShape);
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

		btVector3 localInertia(0, 0, 0);
		if (rigid_body.mass != 0.f)
			colShape->calculateLocalInertia(rigid_body.mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		m_Motions.push_back(myMotionState);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid_body.mass, myMotionState, colShape, localInertia);

		btRigidBody* body = new btRigidBody(rbInfo);
		body->setUserIndex(id);

		ObjectData* data = new ObjectData();
		data->rigidBody = &rigid_body;
		data->transform3d = &transform;
		data->id = id;

		body->setUserPointer(data);
		m_World->addRigidBody(body);
		m_Bodies.push_back(body);

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(id);
		WI_INFO("New rigidbody for --> {}", e_name);

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
		m_Shapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

		btVector3 localInertia(0, 0, 0);
		if (rigid_body.mass != 0.f)
			colShape->calculateLocalInertia(rigid_body.mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		m_Motions.push_back(myMotionState);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid_body.mass, myMotionState, colShape, localInertia);

		btRigidBody* body = new btRigidBody(rbInfo);
		body->setUserIndex(id);

		ObjectData* data = new ObjectData();
		data->rigidBody = &rigid_body;
		data->transform3d = &transform;
		data->id = id;

		body->setUserPointer(data);
		m_World->addRigidBody(body);
		m_Bodies.push_back(body);

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(id);
		WI_INFO("New rigidbody for --> {}", e_name);

		return true;
	}

	bool PhysicsManager::AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height * 0.5f, cylinder.radius, 0.0f));
		m_Shapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

		btVector3 localInertia(0, 0, 0);
		if (rigid_body.mass != 0.f)
			colShape->calculateLocalInertia(rigid_body.mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		m_Motions.push_back(myMotionState);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid_body.mass, myMotionState, colShape, localInertia);

		btRigidBody* body = new btRigidBody(rbInfo);
		body->setUserIndex(id);

		ObjectData* data = new ObjectData();
		data->rigidBody = &rigid_body;
		data->transform3d = &transform;
		data->id = id;

		body->setUserPointer(data);
		m_World->addRigidBody(body);
		m_Bodies.push_back(body);

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(id);
		WI_INFO("New rigidbody for --> {}", e_name);

		return true;
	}

	btRigidBody* PhysicsManager::FindByEntityId(size_t id)
	{
		for (std::list<btRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			if ((*item)->getUserIndex() == id)
				return *item;
		}
		return nullptr;
	}

	bool PhysicsManager::getInit()
	{
		return m_HasBeenInit;
	}

	// =============================================

	void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		/*glUseProgram(0);
		glColor3f(255, 0, 0);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glVertex3f(from.getX(), from.getY(), from.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
		glEnd();
		glLineWidth(1.0f);*/
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
		mode = (DebugDrawModes)debug_mode;
	}

	int DebugDrawer::getDebugMode() const
	{
		return mode;
	}
}



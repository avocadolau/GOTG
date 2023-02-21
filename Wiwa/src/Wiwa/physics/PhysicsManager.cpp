#include <wipch.h>
#include "PhysicsManager.h"
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/utilities/render/CameraManager.h"
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
		m_Collision_conf = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_Collision_conf);
		m_Broad_phase = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver();
		m_Debug_draw = new DebugDrawer();

		// Bullet Bounding box
		m_Debug_draw->lineDisplayShaderId = Resources::Load<Shader>("resources/shaders/debug/line_display");
		m_Debug_draw->lineDisplayShader = Resources::GetResourceById<Shader>(m_Debug_draw->lineDisplayShaderId);
		m_Debug_draw->lineDisplayShader->Compile("resources/shaders/debug/line_display");
		m_Debug_draw->lineDisplayShaderUniforms.Model = m_Debug_draw->lineDisplayShader->getUniformLocation("u_Model");
		m_Debug_draw->lineDisplayShaderUniforms.View = m_Debug_draw->lineDisplayShader->getUniformLocation("u_View");
		m_Debug_draw->lineDisplayShaderUniforms.Projection = m_Debug_draw->lineDisplayShader->getUniformLocation("u_Proj");
	}

	PhysicsManager::~PhysicsManager()
	{
		delete m_Debug_draw;
		delete m_Solver;
		delete m_Broad_phase;
		delete m_Dispatcher;
		delete m_Collision_conf;
	}

	bool PhysicsManager::InitWorld()
	{
		m_Debug = true;
		m_HasBeenInit = true;

		m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broad_phase, m_Solver, m_Collision_conf);

		m_Debug_draw->setDebugMode(m_Debug_draw->DBG_MAX_DEBUG_DRAW_MODE);
		m_World->setDebugDrawer(m_Debug_draw);
		m_World->setGravity(GRAVITY);

		//// Big plane as ground
		//{
		//	btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		//	btDefaultMotionState* myMotionState = new btDefaultMotionState();
		//	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		//	btRigidBody* body = new btRigidBody(rbInfo);
		//	m_World->addRigidBody(body);

		//}

		WI_INFO("Physics Manager Init");
		
		return true;
	}

	bool PhysicsManager::StepSimulation()
	{
		m_World->stepSimulation(Wiwa::Time::GetDeltaTimeSeconds(), 15);

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
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			Transform3D*transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			Rigidbody*rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>((*item)->id);

			// Get the position from the engine
			glm::vec3 posEngine = transform3d->localPosition;
			glm::quat rotEngine = glm::quat(transform3d->localMatrix);
			//glm::quat rotEngine = entityData->transform3d->rotation; old

			// Get the offset
			glm::vec3 finalOffset = rotEngine * rigidBody->positionOffset;
			glm::vec3 finalPosBullet = posEngine + finalOffset;

			// Apply the offset because offset, it is internal only(collider wise)
			btTransform offsettedCollider;
			offsettedCollider.setFromOpenGLMatrix(glm::value_ptr(transform3d->localMatrix));
			offsettedCollider.setOrigin(btVector3(finalPosBullet.x, finalPosBullet.y, finalPosBullet.z));
			offsettedCollider.setRotation(btQuaternion(rotEngine.x, rotEngine.y, rotEngine.z, rotEngine.w));

			(*item)->btBody->getCollisionShape()->setLocalScaling((btVector3(rigidBody->scalingOffset.x, rigidBody->scalingOffset.y, rigidBody->scalingOffset.z)));
			(*item)->btBody->setWorldTransform(offsettedCollider);
		}
		return true;
	}

	bool PhysicsManager::UpdatePhysicsToEngine()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		// Physics to Engine
		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			Transform3D* transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			Rigidbody* rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>((*item)->id);

			btTransform bulletTransform((*item)->btBody->getWorldTransform());

			// Get the transform from physics world
			btVector3 posBullet = bulletTransform.getOrigin();
			btQuaternion rotationBullet = bulletTransform.getRotation();

			glm::quat rotationEngine = glm::quat(rotationBullet.w(), rotationBullet.x(), rotationBullet.y(), rotationBullet.z());
			glm::vec3 finalOffset = rotationEngine * rigidBody->positionOffset;
			glm::vec3 posEngine = glm::vec3(posBullet.x() - finalOffset.x, posBullet.y() - finalOffset.y, posBullet.z() - finalOffset.z);

			// Remove the offset because offset is internal only(collider wise)
			transform3d->localPosition = posEngine;
			bulletTransform.setOrigin(btVector3(posEngine.x, posEngine.y, posEngine.z));
			bulletTransform.setRotation(rotationBullet);
			bulletTransform.getRotation().getEulerZYX(transform3d->localRotation.z, transform3d->localRotation.y, transform3d->localRotation.z);
			/*bulletTransform.getOpenGLMatrix(glm::value_ptr(entityData->transform3d->localMatrix));*/

			(*item)->btBody->getCollisionShape()->setLocalScaling((btVector3(rigidBody->scalingOffset.x, rigidBody->scalingOffset.y, rigidBody->scalingOffset.z)));
			m_World->updateSingleAabb((*item)->btBody);
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

		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			m_World->removeRigidBody((*item)->btBody);
			//delete (ObjectData*)(*item)->getUserPointer();
			delete (*item)->btBody;
			delete*item;
			*item = nullptr;
		}

		m_Bodies.clear();

		delete m_World;

		m_HasBeenInit = false;
		return true;
	}

	bool PhysicsManager::DeleteBody(MyRigidBody* body)
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

		m_Motions.remove((btDefaultMotionState*)body->btBody->getMotionState());
		delete body->btBody->getMotionState();

		m_Shapes.remove(body->btBody->getCollisionShape());
		delete body->btBody->getCollisionShape();

		m_Bodies.remove(body);
		m_World->removeRigidBody(body->btBody);
		//delete (ObjectData*)(body)->getUserPointer();
		delete body->btBody;
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

		btRigidBody* btBody = new btRigidBody(rbInfo);

		MyRigidBody* myBodyData = new MyRigidBody(*btBody, id);
		m_World->addRigidBody(btBody);
		m_Bodies.push_back(myBodyData);
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

		btRigidBody* btBody = new btRigidBody(rbInfo);

		MyRigidBody* myBodyData = new MyRigidBody(*btBody, id);
		m_World->addRigidBody(btBody);
		m_Bodies.push_back(myBodyData);
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

		btRigidBody* btBody = new btRigidBody(rbInfo);

		MyRigidBody* myBodyData = new MyRigidBody(*btBody, id);
		m_World->addRigidBody(btBody);
		m_Bodies.push_back(myBodyData);
		return true;
	}

	PhysicsManager::MyRigidBody* PhysicsManager::FindByEntityId(size_t id)
	{
		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			if ((*item)->id == id)
				return *item;
		}
		return nullptr;
	}

	bool PhysicsManager::getInit()
	{
		return m_HasBeenInit;
	}

	bool PhysicsManager::LogBodies()
	{
		if (m_Bodies.empty())
			return false;

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* name = Wiwa::SceneManager::getActiveScene()->getName();
		WI_INFO("SCENE {} World has total of {} bodies", name, m_Bodies.size());
		int num = 0;
		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			
			/*int id = (*item)->getUserIndex();*/
			btVector3 pos = (*item)->btBody->getWorldTransform().getOrigin();
			/*const char* e_name = entityManager.GetEntityName(id); */
			WI_INFO("Index {} is {} {} {}", num, pos.x(), pos.y(), pos.z()); 
			num++;
		}
		return true;
	}
	void PhysicsManager::DebugDrawWorld()
	{
		Camera* camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(camera->getProjection()));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(camera->getView()));

		//m_Debug_draw->lineDisplayShader->Bind();
		////m_Debug_draw->lineDisplayShader->setUniform(m_Debug_draw->lineDisplayShaderUniforms.Model, transform);
		//m_Debug_draw->lineDisplayShader->setUniform(m_Debug_draw->lineDisplayShaderUniforms.View, camera->getView());
		//m_Debug_draw->lineDisplayShader->setUniform(m_Debug_draw->lineDisplayShaderUniforms.Projection, camera->getProjection());

		m_World->debugDrawWorld();
		//m_Debug_draw->lineDisplayShader->UnBind();
		camera->frameBuffer->Unbind();
	}
}

//void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//	WI_INFO("Line from {} {} {} to {} {} {}", from.x(), from.y(), from.z(), to.x(), to.y(), to.z());
//
//	// Create Vertex Array Object
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	// Create a Vertex Buffer Object and copy the vertex data to it
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	GLfloat lineVertices[] = {
//		from.x(), from.y(), from.z(),
//		to.x(), to.y(), to.z()
//	};
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineVertices, GL_STATIC_DRAW);
//
//	// Create an element array
//	GLuint ebo;
//	glGenBuffers(1, &ebo);
//	GLuint elements[] = {
//		0, 1,
//	};
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), elements, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lineVertices), (void*)0);
//	
//	glBindVertexArray(vao);
//	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	glDeleteVertexArrays(1, &vao);
//	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ebo);
//}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	WI_INFO("Line from {} {} {} to {} {} {}", from.x(), from.y(), from.z(), to.x(), to.y(), to.z());
	//GLfloat lineVertices[] = {
	//	from.x(), from.y(), from.z(),
	//	to.x(), to.y(), to.z()
	//};
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer
	glUseProgram(0);
	glColor3f(255, 0, 0);
	glLineWidth(3.0f);
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
	mode = (DebugDrawModes)debug_mode;
}

int DebugDrawer::getDebugMode() const
{
	return mode;
}
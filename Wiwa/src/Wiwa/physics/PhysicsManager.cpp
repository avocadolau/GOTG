#include <wipch.h>
#include "PhysicsManager.h"
#include "Wiwa/ecs/systems/System.h"
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/utilities/render/CameraManager.h"
#include <glew.h>

#include <Wiwa/utilities/Log.h>
#include "Wiwa/utilities/time/Time.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Wiwa/utilities/json/JSONDocument.h>

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

		AddFilterTag("COLLISION_EVERYTHING");

		WI_INFO("Physics Manager Init");
		
		return true;
	}

	bool PhysicsManager::StepSimulation()
	{
		// Set collision booleans to false
		for (size_t i = 0; i < m_CollisionList.size(); i++)
		{
			m_CollisionList[i].collisionType = CT_LEAVE;
		}

		// Step simulation
		m_World->stepSimulation(Wiwa::Time::GetDeltaTimeSeconds(), 15);

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		int numManifolds = m_World->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = m_World->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			if (numContacts > 0)
			{
				size_t idA = obA->getUserIndex();
				size_t idB = obB->getUserIndex();

				UpdateCollisionType(idA, idB);
			}
		}

		// Collision callbacks list update
		for (size_t i = 0; i < m_CollisionList.size(); i++)
		{
			CollisionData& cData = m_CollisionList[i];

			std::vector<System*>& vecA = entityManager.GetEntitySystems(cData.entityA);
			std::vector<System*>& vecB = entityManager.GetEntitySystems(cData.entityB);

			switch (cData.collisionType) {
			case CT_ENTER:
				for (size_t k = 0; k < vecA.size(); k++) {
					vecA[k]->OnCollisionEnter(FindByEntityId(cData.entityA), FindByEntityId(cData.entityB));
				}
				for (size_t j = 0; j < vecA.size(); j++) {
					vecA[j]->OnCollisionEnter(FindByEntityId(cData.entityB), FindByEntityId(cData.entityA));
				}
				break;
			case CT_LOOP:
				for (size_t k = 0; k < vecA.size(); k++) {
					vecA[k]->OnCollision(FindByEntityId(cData.entityA), FindByEntityId(cData.entityB));
				}
				for (size_t j = 0; j < vecA.size(); j++) {
					vecA[j]->OnCollision(FindByEntityId(cData.entityB), FindByEntityId(cData.entityA));
				}
				break;
			case CT_LEAVE:
				for (size_t k = 0; k < vecA.size(); k++) {
					vecA[k]->OnCollisionLeave(FindByEntityId(cData.entityA), FindByEntityId(cData.entityB));
				}
				for (size_t j = 0; j < vecA.size(); j++) {
					vecA[j]->OnCollisionLeave(FindByEntityId(cData.entityB), FindByEntityId(cData.entityA));
				}
				m_CollisionList.erase(m_CollisionList.begin() + i);
				i--;
				break;
			default:
				break;
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

		filterStrings.clear();
		fliterBitsSets.clear();

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
		btBody->setUserIndex(id);
		
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
		btBody->setUserIndex(id);

		int32_t collisionFinal;
		std::bitset<32> b, self;
		b.set();
		self[rigid_body.selfTag] = true;
		collisionFinal |= b.to_ulong();
		for (int i = 1; i < fliterBitsSets.size(); i++)
		{
			if (rigid_body.filterBits[i] == true)
			{
				collisionFinal &= ~fliterBitsSets.at(i).to_ulong();
				WI_INFO("{} --> {}", filterStrings.at(i).c_str(), fliterBitsSets.at(i).to_string());
			}
		}
		bin(collisionFinal);
		MyRigidBody* myBodyData = new MyRigidBody(*btBody, id);
		m_World->addRigidBody(btBody, self.to_ulong(), collisionFinal);
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
		btBody->setUserIndex(id);

		MyRigidBody* myBodyData = new MyRigidBody(*btBody, id);
		m_World->addRigidBody(btBody);
		m_Bodies.push_back(myBodyData);
		return true;
	}

	void PhysicsManager::SetBodyMass(MyRigidBody* body, float mass)
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		Rigidbody* rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>(body->id);

		rigidBody->mass = mass;

		//Remove the rigid body from the dynamics world
		m_World->removeRigidBody(body->btBody);

		btVector3 inertia;
		body->btBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
		body->btBody->setMassProps(mass, inertia);

		//Add the rigid body to the dynamics world
		m_World->addRigidBody(body->btBody);
	}

	void PhysicsManager::SetBodyGravity(MyRigidBody* body, btVector3 gravity)
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		Rigidbody* rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>(body->id);

		rigidBody->gravity = glm::vec3(gravity.x(), gravity.y(), gravity.z());
		body->btBody->setGravity(gravity);
	}

	void PhysicsManager::SetTrigger(MyRigidBody* body, bool isTrigger)
	{
		if (isTrigger == true)
			body->btBody->setCollisionFlags(body->btBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->btBody->setCollisionFlags(body->btBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	void PhysicsManager::SetStatic(MyRigidBody* body, bool isStatic)
	{
		if (isStatic == true)
		{
			SetBodyMass(body, 0.0f);
			body->btBody->setCollisionFlags(body->btBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		}
		else
		{
			SetBodyMass(body, 1.0f);
			body->btBody->setCollisionFlags(body->btBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
		}
	}

	MyRigidBody* PhysicsManager::FindByEntityId(size_t id)
	{
		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			if ((*item)->id == id)
				return *item;
		}
		return nullptr;
	}

	void PhysicsManager::UpdateCollisionType(size_t first, size_t second)
	{
		size_t collision_list_size = m_CollisionList.size();

		size_t i = collision_list_size;

		for (i = 0; i < collision_list_size; i++) {
			if (m_CollisionList[i].entityA == first && m_CollisionList[i].entityB == second) {
				break;
			}
		}

		if (i == collision_list_size) {
			m_CollisionList.push_back({ first, second, CT_ENTER });
		}
		else {
			m_CollisionList[i].collisionType = CT_LOOP;
		}
	}

	bool PhysicsManager::OnSave()
	{
		JSONDocument physics;
		int count = filterStrings.size();

		physics.AddMember("tags_count", count);
		std::string tag = "tag_";
		for (int i = 0; i < count; i++)
		{
			std::string newTag = tag;
			newTag += std::to_string(i);
			physics.AddMember(newTag.c_str(), filterStrings[i].c_str());
		}
		std::string path = "assets/Scenes/";
		path += SceneManager::getActiveScene()->getName();
		path += "_physics.json";
		physics.save_file(path.c_str());
		return true;
	}

	bool PhysicsManager::OnLoad()
	{
		std::string path = "assets/Scenes/";
		path += SceneManager::getActiveScene()->getName();
		path += "_physics.json";

		JSONDocument physics;
		if (!physics.load_file(path.c_str()))
			return false;

		for (int i = 1; i < filterStrings.size(); i++)
			RemoveFilterTag(i);

		int count = physics["tags_count"].get<int>();
		std::string tag = "tag_";
		for (int i = 1; i < count; i++)
		{
			std::string newTag = tag;
			newTag += std::to_string(i);
			AddFilterTag(physics[newTag.c_str()].get<const char*>());
		}
		return false;
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
		//WI_INFO("SCENE {} World has total of {} bodies", name, m_Bodies.size());
		int num = 0;
		for (std::list<MyRigidBody*>::iterator item = m_Bodies.begin(); item != m_Bodies.end(); item++)
		{
			
			/*int id = (*item)->getUserIndex();*/
			btVector3 pos = (*item)->btBody->getWorldTransform().getOrigin();
			/*const char* e_name = entityManager.GetEntityName(id); */
			//WI_INFO("Index {} is {} {} {}", num, pos.x(), pos.y(), pos.z()); 
			num++;
		}
		return true;
	}
	void PhysicsManager::DebugDrawWorld()
	{
		Camera* camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);
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

	bool PhysicsManager::AddFilterTag(const char* str)
	{
		if (filterStrings.size() == 32)
			return false;

		filterStrings.emplace_back(str);
		std::bitset<MAX_BITS> bset;
		bset.set(filterStrings.size(), true);
		fliterBitsSets.push_back(bset);
		return true;
	}
	void PhysicsManager::RemoveFilterTag(const int index)
	{
		filterStrings.erase(filterStrings.begin() + index);
		fliterBitsSets.erase(fliterBitsSets.begin() + index);
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
	//WI_INFO("Line from {} {} {} to {} {} {}", from.x(), from.y(), from.z(), to.x(), to.y(), to.z());
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
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

		m_filterCallback = new CustomFilterCallBack();

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
		delete m_filterCallback;
	}

	bool PhysicsManager::InitWorld()
	{
		m_Debug = true;
		m_HasBeenInit = true;

		m_World = new btCollisionWorld(m_Dispatcher,m_Broad_phase,m_Collision_conf);
		m_World->getPairCache()->setOverlapFilterCallback(m_filterCallback);

		m_Debug_draw->setDebugMode(m_Debug_draw->DBG_MAX_DEBUG_DRAW_MODE);
		m_World->setDebugDrawer(m_Debug_draw);
		//m_World->(GRAVITY);

		AddFilterTag("COLLISION_EVERYTHING");

		//WI_INFO("Physics Manager Init");
		
		return true;
	}

	bool PhysicsManager::StepSimulation()
	{
		// Set collision booleans to false
		for (size_t i = 0; i < m_CollisionList.size(); i++)
		{
			m_CollisionList[i].collisionType = CT_LEAVE;
		}

		//Step simulation
		float dt = 1.0f/60.0f;
		//m_World->stepSimulation(Wiwa::Time::GetDeltaTimeSeconds(), 6);
		UpdateObjects(Wiwa::Time::GetDeltaTimeSeconds());
		m_World->performDiscreteCollisionDetection();
		ResolveContacts();
		static int o = 0;
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		for (std::list<Object*>::iterator item = m_CollObjects.begin(); item != m_CollObjects.end(); item++)
		{
			Transform3D* transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			//SetRotation((*item), glm::vec3(transform3d->localRotation.x, o, transform3d->localRotation.z));
			o++;
		}
		
		// Commented because it causes clipping :(
		/*for (int i = 0; i < SUB_STEPS; i++)
		{
			UpdateObjects(dt / SUB_STEPS);
			m_World->performDiscreteCollisionDetection();
			ResolveContacts();
		}*/
		
		return true;
	}

	bool PhysicsManager::ResolveContacts()
	{
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
				//WI_INFO("normal : {} {} {}", contactManifold->getContactPoint(0).m_normalWorldOnB.x(), contactManifold->getContactPoint(0).m_normalWorldOnB.y(), contactManifold->getContactPoint(0).m_normalWorldOnB.z());
				//WI_INFO("c2 : {}", contactManifold->getContactPoint(0).m_contactMotion2);
				btVector3 toSubstract = contactManifold->getContactPoint(0).m_normalWorldOnB * contactManifold->getContactPoint(0).getDistance();

				ResolveContactA(obA, toSubstract, (obA->getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT), (obA->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE), (obB->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE));
				ResolveContactB(obB, toSubstract, (obB->getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT), (obB->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE), (obA->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE));
				//WI_INFO("d : {}", contactManifold->getContactPoint(0).getDistance());
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
			Object* objA = FindByEntityId(cData.entityA);
			Object* objB = FindByEntityId(cData.entityB);

			switch (cData.collisionType) {
			case CT_ENTER:
				for (size_t k = 0; k < vecA.size(); k++) {
					vecA[k]->OnCollisionEnter(objA, objB);
				}
				for (size_t j = 0; j < vecB.size(); j++) {
					vecB[j]->OnCollisionEnter(objB, objA);
				}
				break;
			case CT_LOOP:
				if (objA->doContinuousCollision)
				{
					for (size_t k = 0; k < vecA.size(); k++) {
						vecA[k]->OnCollision(objA, objB);
					}
				}
				if (objB->doContinuousCollision)
				{
					for (size_t j = 0; j < vecB.size(); j++) {
						vecB[j]->OnCollision(objB, objA);
					}
				}
				break;
			case CT_LEAVE:
				for (size_t k = 0; k < vecA.size(); k++) {
					vecA[k]->OnCollisionExit(objA, objB);
				}
				for (size_t j = 0; j < vecB.size(); j++) {
					vecB[j]->OnCollisionExit(objB, objA);
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

	bool PhysicsManager::ResolveContactA(btCollisionObject* obj_a, const btVector3& vec, const bool is_static, const bool is_trigger_a, const bool is_trigger_b)
	{
		if (is_static)
			return false;
		if (is_trigger_a || is_trigger_b)
			return false;

		obj_a->getWorldTransform().setOrigin(obj_a->getWorldTransform().getOrigin() - vec);
		return true;
	}

	bool PhysicsManager::ResolveContactB(btCollisionObject* obj_a, const btVector3& vec, const bool is_static, const bool is_trigger_a, const bool is_trigger_b)
	{
		if (is_static)
			return false;
		if (is_trigger_a || is_trigger_b)
			return false;

		obj_a->getWorldTransform().setOrigin(obj_a->getWorldTransform().getOrigin() + vec);
		return true;
	}

	bool PhysicsManager::UpdateObjects(const float dt)
	{
		for (std::list<Object*>::iterator item = m_CollObjects.begin(); item != m_CollObjects.end(); item++)
		{
			if ((*item)->velocity.isZero())
				continue;

			btVector3 position = (*item)->collisionObject->getWorldTransform().getOrigin();
			position += (*item)->velocity * dt;
			(*item)->collisionObject->getWorldTransform().setOrigin(position);
		}
		return true;
	}

	bool PhysicsManager::UpdateEngineToPhysics()
	{
		// Set the position offset
		// Get the position from the engine
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		for (std::list<Object*>::iterator item = m_CollObjects.begin(); item != m_CollObjects.end(); item++)
		{
			Transform3D*transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			Rigidbody*rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>((*item)->id);

			// Get the position from the engine
			glm::vec3 posEngine = transform3d->localPosition;
			//glm::quat rotEngine = glm::quat(transform3d->localMatrix); // Old version where you cannot rotate on more than one axis at a time
			glm::quat rotEngine = glm::quat(glm::radians(transform3d->localRotation)); // Newer version where you can do that but still have gimble lock in Y Axis 
			//glm::quat rotEngine = entityData->transform3d->rotation; old

			// Get the offset
			glm::vec3 finalOffset = rotEngine * rigidBody->positionOffset;
			glm::vec3 finalPosBullet = posEngine + finalOffset;

			// Apply the offset because offset, it is internal only(collider wise)
			btTransform offsettedCollider;
			offsettedCollider.setFromOpenGLMatrix(glm::value_ptr(transform3d->localMatrix));
			offsettedCollider.setOrigin(btVector3(finalPosBullet.x, finalPosBullet.y, finalPosBullet.z));
			offsettedCollider.setRotation(btQuaternion(rotEngine.x, rotEngine.y, rotEngine.z, rotEngine.w));

			(*item)->collisionObject->getCollisionShape()->setLocalScaling((btVector3(rigidBody->scalingOffset.x, rigidBody->scalingOffset.y, rigidBody->scalingOffset.z)));
			(*item)->collisionObject->setWorldTransform(offsettedCollider);
		}
		return true;
	}

	bool PhysicsManager::UpdatePhysicsToEngine()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		// Physics to Engine
		for (std::list<Object*>::iterator item = m_CollObjects.begin(); item != m_CollObjects.end(); item++)
		{
			Transform3D* transform3d = entityManager.GetComponent<Wiwa::Transform3D>((*item)->id);
			Rigidbody* rigidBody = entityManager.GetComponent<Wiwa::Rigidbody>((*item)->id);

			btTransform bulletTransform((*item)->collisionObject->getWorldTransform());

			// Get the transform from physics world
			btVector3 posBullet = bulletTransform.getOrigin();
			btQuaternion rotationBullet = bulletTransform.getRotation();

			glm::quat rotationEngine = glm::quat(rotationBullet.w(), rotationBullet.x(), rotationBullet.y(), rotationBullet.z());
			glm::vec3 finalOffset = rotationEngine * rigidBody->positionOffset;
			glm::vec3 posEngine = glm::vec3(posBullet.x() - finalOffset.x, posBullet.y() - finalOffset.y, posBullet.z() - finalOffset.z);

			// Remove the offset because offset is internal only(collider wise)
			transform3d->localPosition = posEngine;
			bulletTransform.setOrigin(btVector3(posEngine.x, posEngine.y, posEngine.z));
			//bulletTransform.setRotation(rotationBullet);

			glm::vec3 eulerAngles;
			bulletTransform.getRotation().normalized().getEulerZYX(eulerAngles.z, eulerAngles.y, eulerAngles.x);
			//WI_INFO("Y Axis : {}", glm::degrees(eulerAngles.y));
			//transform3d->localRotation = glm::degrees(eulerAngles);
			/*bulletTransform.getOpenGLMatrix(glm::value_ptr(entityData->transform3d->localMatrix));*/

			(*item)->collisionObject->getCollisionShape()->setLocalScaling((btVector3(rigidBody->scalingOffset.x, rigidBody->scalingOffset.y, rigidBody->scalingOffset.z)));
			m_World->updateSingleAabb((*item)->collisionObject);
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

		for (std::list<btCollisionShape*>::iterator item = m_Shapes.begin(); item != m_Shapes.end(); item++)
		{
			delete* item;
			*item = nullptr;
		}

		m_Shapes.clear();

		for (std::list<Object*>::iterator item = m_CollObjects.begin(); item != m_CollObjects.end(); item++)
		{
			m_World->removeCollisionObject((*item)->collisionObject);
			//delete (ObjectData*)(*item)->getUserPointer();
			delete (*item)->collisionObject;
			delete*item;
			*item = nullptr;
		}

		m_CollObjects.clear();
		m_BodiesToLog.clear();
		delete m_World;

		filterStrings.clear();
		fliterBitsSets.clear();

		m_HasBeenInit = false;
		return true;
	}

	bool PhysicsManager::DeleteBody(Object* body)
	{
		m_Shapes.remove(body->collisionObject->getCollisionShape());
		delete body->collisionObject->getCollisionShape();

		m_CollObjects.remove(body);
		m_BodiesToLog.remove(body);
		m_World->removeCollisionObject(body->collisionObject);
		//delete (ObjectData*)(body)->getUserPointer();
		delete body->collisionObject;
		delete body;
		return true;
	}

	bool PhysicsManager::AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		btCollisionShape* colShape = new btSphereShape(sphere.radius);
		m_Shapes.push_back(colShape);
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

		btCollisionObject* collisionObject = new btCollisionObject();
		collisionObject->setUserIndex(id); // id

		AddBodyInternal(id, collisionObject, colShape, rigid_body);
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

		btCollisionObject* collisionObject = new btCollisionObject();
		collisionObject->setUserIndex(id);

		AddBodyInternal(id, collisionObject, colShape, rigid_body);
		return true;
	}

	bool PhysicsManager::AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		btCollisionShape* colShape = new btCylinderShape(btVector3(cylinder.height * 0.5f, cylinder.radius, 0.0f));
		m_Shapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

		btCollisionObject* collisionObject = new btCollisionObject();
		collisionObject->setUserIndex(id);

		AddBodyInternal(id, collisionObject, colShape, rigid_body);
		return true;
	}

	bool PhysicsManager::AddBodyCapsule(size_t id, const Wiwa::ColliderCapsule& capsule, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body)
	{
		btCollisionShape* colShape = new btCapsuleShape(capsule.radius, capsule.radius);
		m_Shapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(transform.worldMatrix));

		btCollisionObject* collisionObject = new btCollisionObject();
		collisionObject->setUserIndex(id);

		AddBodyInternal(id, collisionObject, colShape, rigid_body);
		AddBodyToLog(FindByEntityId(id));
		return true;
	}

	bool PhysicsManager::AddBodyInternal(size_t id, btCollisionObject* collision_object, btCollisionShape* collision_shape, Wiwa::Rigidbody& rigid_body)
	{
		if (rigid_body.isTrigger)
			collision_object->setCollisionFlags(collision_object->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			collision_object->setCollisionFlags(collision_object->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);

		if (rigid_body.isStatic)
			collision_object->setCollisionFlags(collision_object->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		else
			collision_object->setCollisionFlags(collision_object->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);

		collision_object->setCollisionShape(collision_shape);
		Object* myObjData = new Object(*collision_object, id, rigid_body.doContinuousCollision);

		//collision_object->setCollisionFlags(rigid_body.)

		//m_World->addCollisionObject(collision_object, rigid_body.selfTag, rigid_body.filterBits);
		int bits = 0;
		bits |= (1 << rigid_body.selfTag);
		m_World->addCollisionObject(collision_object, bits, rigid_body.filterBits);
		m_CollObjects.push_back(myObjData);
		return true;
	}

	bool PhysicsManager::SetVelocity(Object* body, const glm::vec3 velocity)
	{
		body->velocity = btVector3(velocity.x, velocity.y, velocity.z);
		return true;
	}

	bool PhysicsManager::SetRotation(Object* body, const glm::vec3 euler_angles)
	{
		glm::quat newRot = glm::quat(glm::radians(euler_angles));;
		body->collisionObject->getWorldTransform().setRotation(btQuaternion(newRot.x, newRot.y, newRot.z, newRot.w));
		return true;
	}

	Object* PhysicsManager::FindByEntityId(size_t id)
	{
		for (std::list<Object*>::iterator item = m_CollObjects.begin(); item != m_CollObjects.end(); item++)
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

	void PhysicsManager::DebugDrawWorld()
	{
		Camera* camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);

		m_Debug_draw->lineDisplayShader->Bind();
		m_Debug_draw->lineDisplayShader->setUniformMat4(m_Debug_draw->lineDisplayShaderUniforms.Model, glm::mat4(1.0f));
		m_Debug_draw->lineDisplayShader->setUniformMat4(m_Debug_draw->lineDisplayShaderUniforms.View, camera->getView());
		m_Debug_draw->lineDisplayShader->setUniformMat4(m_Debug_draw->lineDisplayShaderUniforms.Projection, camera->getProjection());
		m_Debug_draw->lineDisplayShader->setUniformVec4(m_Debug_draw->lineDisplayShader->getUniformLocation("u_Color"), glm::vec4(1.0, 0.0f, 0.0f, 1.0f));

		m_World->debugDrawWorld();
		m_Debug_draw->lineDisplayShader->UnBind();
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

	void PhysicsManager::RayTest(const btVector3& ray_from_world, const btVector3& ray_to_world)
	{
		Camera* camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);
		m_Debug_draw->lineDisplayShader->Bind();
		m_Debug_draw->lineDisplayShader->setUniformMat4(m_Debug_draw->lineDisplayShaderUniforms.Model, glm::mat4(1.0f));
		m_Debug_draw->lineDisplayShader->setUniformMat4(m_Debug_draw->lineDisplayShaderUniforms.View, camera->getView());
		m_Debug_draw->lineDisplayShader->setUniformMat4(m_Debug_draw->lineDisplayShaderUniforms.Projection, camera->getProjection());
		m_Debug_draw->lineDisplayShader->setUniformVec4(m_Debug_draw->lineDisplayShader->getUniformLocation("u_Color"), glm::vec4(0.0, 0.0f, 1.0f, 1.0f));
		
		((DebugDrawer*)m_World->getDebugDrawer())->drawLine(ray_from_world, ray_to_world, btVector4(0, 0, 1, 1));
		btCollisionWorld::ClosestRayResultCallback closestHit = btCollisionWorld::ClosestRayResultCallback(ray_from_world, ray_to_world);
		m_World->rayTest(ray_from_world, ray_to_world, closestHit);
		if (closestHit.hasHit())
		{
			btVector3 p = ray_from_world.lerp(ray_to_world, closestHit.m_closestHitFraction);
			m_World->getDebugDrawer()->drawSphere(p, 0.1, btVector4(0, 0, 1, 1));
			m_World->getDebugDrawer()->drawLine(p, p + closestHit.m_hitNormalWorld, btVector4(0, 0, 1, 1));
		}

		camera->frameBuffer->Unbind();
	}

	bool PhysicsManager::AddBodyToLog(Object* body_to_log)
	{
		m_BodiesToLog.emplace_back(body_to_log);
		return true;
	}

	bool PhysicsManager::RemoveBodyFromLog(Object* body_to_log)
	{
		m_BodiesToLog.remove(body_to_log);
		return true;
	}

	bool PhysicsManager::LogBodies()
	{
		if (m_BodiesToLog.empty())
			return false;

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* name = Wiwa::SceneManager::getActiveScene()->getName();
		//WI_INFO("SCENE {} World has total of {} bodies", name, m_CollObjects.size());
		int num = 0;
		for (std::list<Object*>::iterator item = m_BodiesToLog.begin(); item != m_BodiesToLog.end(); item++)
		{
			btVector3 pos = (*item)->collisionObject->getWorldTransform().getOrigin();
			/*const char* e_name = entityManager.GetEntityName(id); */
			WI_INFO("Id {} is at {} {} {}", (*item)->id, pos.x(), pos.y(), pos.z()); 
			num++;
		}
		return true;
	}
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//WI_INFO("Line from {} {} {} to {} {} {}", from.x(), from.y(), from.z(), to.x(), to.y(), to.z());

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);
	GLfloat lineVertices[] = {
		from.x(), from.y(), from.z(),
		to.x(), to.y(), to.z()
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineVertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);
	GLuint elements[] = {
		0, 1,
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), elements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

//void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//	//WI_INFO("Line from {} {} {} to {} {} {}", from.x(), from.y(), from.z(), to.x(), to.y(), to.z());
//	//GLfloat lineVertices[] = {
//	//	from.x(), from.y(), from.z(),
//	//	to.x(), to.y(), to.z()
//	//};
//	//glEnableClientState(GL_VERTEX_ARRAY);
//	//glVertexPointer
//	/*glUseProgram(0);
//	glColor3f(255, 0, 0);
//	glLineWidth(3.0f);
//	glBegin(GL_LINES);
//	glVertex3f(from.getX(), from.getY(), from.getZ());
//	glVertex3f(to.getX(), to.getY(), to.getZ());
//	glEnd();
//	glLineWidth(1.0f);*/
//
//}

void DebugDrawer::drawContactPoint(const btVector3& point_onB, const btVector3& normal_onB, btScalar distance, int life_time, const btVector3& color)
{
	drawSphere(point_onB, 0.1, btVector4(0, 1, 1, 1));
	drawLine(point_onB, point_onB + normal_onB, btVector4(0, 1, 1, 1));
	/*glUseProgram(0);
	glColor3f(0, 0, 255);
	glPointSize(8.0f);
	glBegin(GL_POINT);
	glVertex3f(point_onB.getX(), point_onB.getY(), point_onB.getZ());
	glEnd();
	glPointSize(1.0f);*/
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

bool CustomFilterCallBack::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
{
	/*Wiwa::PhysicsManager::bin(proxy0->m_collisionFilterMask, "A filter mask");
	Wiwa::PhysicsManager::bin(proxy1->m_collisionFilterMask, "B filter mask");
	Wiwa::PhysicsManager::bin(proxy0->m_collisionFilterGroup, "A tag");
	Wiwa::PhysicsManager::bin(proxy1->m_collisionFilterGroup, "B tag");
	std::cout << std::endl;*/

	// Check if A group is inside B mask
	bool aCanCollide = (proxy0->m_collisionFilterMask & proxy1->m_collisionFilterGroup);
	//WI_INFO("A Can Collide {}", aCanCollide);
	// Check if B group is inside A mask
	bool bCanCollide = (proxy1->m_collisionFilterMask & proxy0->m_collisionFilterGroup);
	//WI_INFO("B Can Collide {}", bCanCollide);

	if (aCanCollide && bCanCollide)
		return true;

	bool everyThingA = ((proxy0->m_collisionFilterMask >> 0) & 1); // Check if collider 0 has EVERYTHING_FLAG in filter mask
	bool everyThingB = ((proxy1->m_collisionFilterMask >> 0) & 1); // Check if collider 1 has EVERYTHING_FLAG in filter mask
	/*WI_INFO("A Everything {}", aCanCollide);
	WI_INFO("B Everything {}", aCanCollide);*/

	if (everyThingA && everyThingB)
		return true;

	if (everyThingA)
		return (everyThingA && bCanCollide);
	else if (everyThingB)
		return (everyThingB && aCanCollide);
	return false;
}

#include <wipch.h>
#include "AI_RecastManager.h"
#include <Wiwa/scene/SceneManager.h>
#include <glew.h>
#include "Wiwa/ecs/components/ai/AINavMesh.h"
#include <Wiwa/ecs/systems/ai/AINavMeshLoadingSystem.h>
namespace Wiwa
{
	EntityId RecastManager::m_Id = -1;
	EntityManager::ComponentIterator RecastManager::m_MeshIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
	RecastCommon* RecastManager::m_RecastMesh = nullptr;
	InputGeom* RecastManager::m_Geom = nullptr;
	BuildContext RecastManager::ctx = BuildContext();
	bool RecastManager::ExecutePipeline()
	{
		return false;
	}

	bool RecastManager::CreateObj()
	{
		if (m_MeshIt.c_id == WI_INVALID_INDEX || m_MeshIt.c_index == WI_INVALID_INDEX)
			return false;

		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		Wiwa::Mesh* mesh = (Wiwa::Mesh*)em.GetComponentByIterator(m_MeshIt);
		if (mesh == nullptr)
		return false;

		Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
		mod->SaveModelAsOBJ(mod, mod->getModelPath());
		return true;
	}

	bool RecastManager::CreateRecast()
	{
		if (m_MeshIt.c_id == WI_INVALID_INDEX || m_MeshIt.c_index == WI_INVALID_INDEX)
			return false;

		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		Wiwa::Mesh* mesh = (Wiwa::Mesh*)em.GetComponentByIterator(m_MeshIt);
		if (mesh == nullptr)
			return false;

		if (m_Geom)
		{
			delete m_Geom;
			m_Geom = nullptr;
		}

		Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
		std::string path = " ";

		if (mod)
			path = mod->getModelPath();

		path = path.substr(0, path.find_last_of('.')) + ".obj";

		m_RecastMesh = new RecastSoloMesh();
		m_Geom = new InputGeom();
		
		if (!m_Geom->load(&ctx, path))
		{
			delete m_Geom;
			m_Geom = 0;

			// Destroy the sample if it already had geometry loaded, as we've just deleted it!
			if (m_RecastMesh && m_RecastMesh->getInputGeom())
			{
				delete m_RecastMesh;
				m_RecastMesh = 0;
			}
			//ctx.dumpLog("Geom load log %s:", meshName.c_str());
		}

		if (m_RecastMesh)
			m_RecastMesh->setContext(&ctx);

		if (m_RecastMesh && m_Geom)
		{
			m_RecastMesh->handleMeshChanged(m_Geom);
		}
	}

	bool RecastManager::Render()
	{

		Wiwa::Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(camera->getProjection()));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(camera->getView()));
		glEnable(GL_CULL_FACE);

		if (m_RecastMesh)
			m_RecastMesh->handleUpdate(1.0f/60.0f);

		if (m_RecastMesh)
			m_RecastMesh->handleRender();

		glEnd();
		camera->frameBuffer->Unbind();
		glDisable(GL_CULL_FACE);
		return true;
	}

	bool RecastManager::Build()
	{
		if (m_RecastMesh && m_Geom)
		{
			m_RecastMesh->handleBuild();
		}
		return true;
	}

	bool RecastManager::DebugSettings()
	{
		if (m_RecastMesh)
			m_RecastMesh->handleDebugMode();
		return true;
	}

	bool RecastManager::Save()
	{
		if (!m_RecastMesh)
		{
			WI_INFO("Coulnd't save navmesh, as there is no entity navmesh to save");
			return false;
		}

		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		// After all null checks save the file
		std::string path = "library\\navmesh\\";
		path += Wiwa::SceneManager::getActiveScene()->getName();
		path += ".winavmesh";
		m_RecastMesh->Save(path.c_str());

		// Create an entity and store the path to the file
		EntityId id = WI_INVALID_INDEX;
		id = em.GetEntityByName("NavMesh_Data");

		if (id != WI_INVALID_INDEX)
			em.DestroyEntity(id);

		id = em.CreateEntity("NavMesh_Data");
		Wiwa::AINavMesh navMesh;
		strcpy(navMesh.filePath, path.c_str());
		em.AddComponent<Wiwa::AINavMesh>(id, navMesh);
		em.ApplySystem<Wiwa::AINavMeshLoadingSystem>(id);
		return true;
	}

	bool RecastManager::Load()
	{
		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		EntityId id = em.GetEntityByName("NavMesh_Data");
		
		if (id == WI_INVALID_INDEX)
		{
			WI_INFO("Coulnd't load navmesh, as there is no entity NavMesh_Data");
			return false;
		}

		Wiwa::AINavMesh* navMesh = em.GetComponent<Wiwa::AINavMesh>(id);
		if (!navMesh)
		{
			WI_INFO("Coulnd't load navmesh, as there is no component navMesh");
			return false;
		}

		if (!m_RecastMesh)
		{
			m_RecastMesh = new RecastSoloMesh();
		}

		std::string path = navMesh->filePath;
		if (!m_RecastMesh->Load(path.c_str()))
		{
			WI_INFO("AI PANEL: Couldn't load the navmesh at path {}", path.c_str());
		}
		return true;
	}

	void RecastManager::CalcGridSize(const float* minBounds, const float* maxBounds, float cellSize, int* sizeX, int* sizeZ)
	{
		rcCalcGridSize(minBounds, maxBounds, cellSize, sizeX, sizeZ);
	}

	void RecastManager::Cleanup()
	{
		if (m_RecastMesh)
		{
			delete m_RecastMesh;
			m_RecastMesh = nullptr;
		}

		if (m_Geom)
		{
			delete m_Geom;
			m_Geom = nullptr;
		}
	}

}



#include <wipch.h>
#include "AI_RecastManager.h"
#include <Wiwa/scene/SceneManager.h>
#include <glew.h>

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

		if (m_RecastMesh)
			m_RecastMesh->handleRender();

		glEnd();
		camera->frameBuffer->Unbind();
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

	void RecastManager::Cleanup()
	{
		if (m_RecastMesh)
			delete m_RecastMesh;

		if (m_Geom)
			delete m_Geom;
	}

}



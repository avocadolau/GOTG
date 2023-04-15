#include "AIPanel.h"

#include "imgui.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"
#include "Wiwa/ecs/components/Mesh.h"
#include "../vendor/Glew/include/GL/glew.h"
#include "Wiwa/AI/AI_RecastManager.h"

//#include <Wiwa/audio/Audio.h>
//#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>
//#include <Wiwa/utilities/filesystem/FileSystem.h>
//#include <Wiwa/core/Resources.h>

//#include "glew.h"

AIPanel::AIPanel(EditorLayer* instance)
	: Panel("AI Panel", ICON_FK_MAGIC, instance)
{
	m_Id = -1;
	m_DebugDraw = true;
}

AIPanel::~AIPanel()
{
	//if (m_RecastMesh)
	//	delete m_RecastMesh;

	//if (m_Geom)
	//	delete m_Geom;
}

void AIPanel::Draw()
{
	//Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
	//mod = mod->getModelAt(mesh->modelIndex);
	ImGui::Begin(iconName.c_str(), &active);
	HandleDragAndDrop();

	if (ImGui::Button("Create OBJ file"))
		Wiwa::RecastManager::CreateObj();

	if (ImGui::Button("Setup Recast"))
		Wiwa::RecastManager::CreateRecast();

	if (ImGui::Button("Build Map"))
		Wiwa::RecastManager::Build();

	if (ImGui::Button("Clean up"))
		HandleCleanUp();

	HandleRender();
	HandleDebugSettings();

	ImGui::End();
}

void AIPanel::HandleDragAndDrop()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	Wiwa::Mesh* mesh = nullptr;

	ImGui::Text("Drag and drop from hierarchy");

	if (m_Id != -1)
	{
		mesh = (Wiwa::Mesh*)em.GetComponentByIterator(em.GetComponentIterator<Wiwa::Mesh>(m_Id));
	}

	Wiwa::Model* mod = nullptr;
	if (mesh == nullptr){
		AssetContainer("Drag an entity from the hierarchy");
	}
	else
	{
		mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
		if (mod != NULL){
			//mod = mod->getModelAt(m_Mesh->modelIndex);
			AssetContainer(std::filesystem::path(mod->getModelPath()).stem().string().c_str());
		}
	}

	if (ImGui::BeginDragDropTarget()){
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
		{
			const EntityId* id = (EntityId*)payload->Data;
			m_Id = *id;
		}
		ImGui::EndDragDropTarget();
	}

	RefreshManager();
}

void AIPanel::HandleDebugSettings()
{
	if (m_DebugDraw)
		Wiwa::RecastManager::DebugSettings();
}

void AIPanel::HandleRender()
{
	ImGui::Checkbox("Debug draw", &m_DebugDraw);

	if (m_DebugDraw)
		Wiwa::RecastManager::Render();
}

void AIPanel::HandleCleanUp()
{
	Wiwa::RecastManager::Cleanup();
	m_Id = -1;
}

//void AIPanel::HandleCreationObj()
//{
//	if (m_Mesh == nullptr)
//		return;
//
//	Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(m_Mesh->meshId);
//	mod->SaveModelAsOBJ(mod, mod->getModelPath());
//}

//void AIPanel::HandleCreationRecast()
//{
//	if (m_Geom)
//	{
//		delete m_Geom;
//		m_Geom = nullptr;
//	}
//
//	Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(m_Mesh->meshId);
//	std::string path = " ";
//
//	if (mod)
//		path = mod->getModelPath();
//
//	m_RecastMesh = new RecastSoloMesh();
//	m_Geom = new InputGeom();
//
//	if (!m_Geom->load(&ctx, path))
//	{
//		delete m_Geom;
//		m_Geom = 0;
//
//		// Destroy the sample if it already had geometry loaded, as we've just deleted it!
//		if (m_RecastMesh && m_RecastMesh->getInputGeom())
//		{
//			delete m_RecastMesh;
//			m_RecastMesh = 0;
//		}
//
//
//		//ctx.dumpLog("Geom load log %s:", meshName.c_str());
//	}
//
//	if (m_RecastMesh && m_Geom)
//	{
//		m_RecastMesh->handleMeshChanged(m_Geom);
//	}
//
//}

//void AIPanel::HandleRenderRecast()
//{
//	Wiwa::Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;
//
//	glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
//	camera->frameBuffer->Bind(false);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf(glm::value_ptr(camera->getProjection()));
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf(glm::value_ptr(camera->getView()));
//
//
//
//	glEnd();
//	camera->frameBuffer->Unbind();
//}

void AIPanel::RefreshManager()
{
	if (m_Id == -1)
		return;

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	Wiwa::RecastManager::m_Id = m_Id;
	Wiwa::RecastManager::m_MeshIt = em.GetComponentIterator<Wiwa::Mesh>(m_Id);
}

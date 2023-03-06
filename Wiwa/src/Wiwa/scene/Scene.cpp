#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/utilities/render/LightManager.h>

#include <Wiwa/Ui/UiManager.h>

namespace Wiwa
{
	Scene::Scene() : m_InstanceRenderer(40500)
	{
		mMaxTimeEntering = 0;
		mMaxTimeLeaving = 0;

		// Initialize instance renderer with shader
		m_InstanceRenderer.Init("resources/shaders/instanced_tex_color");

		m_GuiManager = new GuiManager();
		m_GuiManager->Init(this);

		m_EntityManager.SetScene(this);
		m_CameraManager = new CameraManager();
		m_LightManager = new LightManager();
		m_PhysicsManager = new PhysicsManager();

		m_PhysicsManager->InitWorld();
	}

	Scene::~Scene()
	{
		delete m_CameraManager;
		delete m_LightManager;
		delete m_GuiManager;

		// Clear entity manager
		m_EntityManager.Clear();

		// Clear physics world
		m_PhysicsManager->CleanWorld();
		delete m_PhysicsManager;
		m_PhysicsManager = nullptr;
	}

	void Scene::Start()
	{
	}

	void Scene::Awake()
	{
		m_EntityManager.SystemsAwake();
	}

	void Scene::Init()
	{
		m_EntityManager.SystemsInit();

		// WAY TO CREATE THE POSITION
		// Rect2i test;
		// test.x = 500;
		// test.y = 100;
		// test.width = 200;
		// test.height = 100;
		// m_GuiManager->CreateGuiControl(GuiControlType::BUTTON, 0, test, "assets/test.png", nullptr, {0,0,0,0});
	}

	void Scene::Update()
	{
		switch (m_CurrentState)
		{
		case Scene::SCENE_ENTERING:
			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			if (m_TransitionTimer >= mMaxTimeEntering)
				m_CurrentState = SCENE_LOOP;
			UpdateEnter();
			RenderEnter();
			break;
		case Scene::SCENE_LOOP:
			m_EntityManager.SystemsUpdate();
			m_GuiManager->Update();
			ProcessInput();
			UpdateLoop();
			RenderLoop();
			break;
		case Scene::SCENE_LEAVING:
			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			if (m_TransitionTimer >= mMaxTimeLeaving)
				SceneManager::ChangeScene(m_SceneToChange);
			UpdateLeave();
			RenderLeave();
			break;
		default:
			break;
		}
	}

	void Scene::ModuleUpdate()
	{
		m_CameraManager->Update();

		m_GuiManager->Draw();

		Wiwa::Renderer2D &r2d = Wiwa::Application::Get().GetRenderer2D();
		r2d.UpdateInstanced(this);

		m_EntityManager.Update();

		m_PhysicsManager->UpdateEngineToPhysics();

		if (SceneManager::IsPlaying())
		{
			m_PhysicsManager->StepSimulation();
			m_PhysicsManager->UpdatePhysicsToEngine();
			// m_PhysicsManager->LogBodies();
		}
		// m_PhysicsManager->LogBodies();

		if (!SceneManager::IsPlaying())
		{
			m_EntityManager.UpdateWhitelist();
		}
	}

	void Scene::Load()
	{
		m_TransitionTimer = 0;

		m_CurrentState = SCENE_ENTERING;
	}

	void Scene::Unload(bool unload_resources)
	{
		return;
		if (unload_resources)
		{
			Wiwa::Resources::UnloadAllResources();
		}
	}

	void Scene::ChangeScene(size_t scene)
	{
		m_SceneToChange = scene;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;
	}
}
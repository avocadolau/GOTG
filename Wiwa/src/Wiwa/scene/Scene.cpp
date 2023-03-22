#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/utilities/render/LightManager.h>

#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/audio/Audio.h>

namespace Wiwa
{
	Scene::Scene() : m_InstanceRenderer(40500)
	{
		mMaxTimeEntering = 300;
		mMaxTimeLeaving = 300;

		// Initialize instance renderer with shader
		m_InstanceRenderer.Init("resources/shaders/instanced_tex_color");

		m_GuiManager = new GuiManager();
		m_GuiManager->Init(this);

		m_EntityManager.SetScene(this);
		m_CameraManager = new CameraManager();
		m_LightManager = new LightManager();
		m_PhysicsManager = new PhysicsManager();

		uint32_t imgid = Resources::LoadNative<Wiwa::Image>("resources/images/transitions/transision.png");
		Wiwa::Image* img = Resources::GetResourceById<Wiwa::Image>(imgid);
		
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		m_TransitionInstance = r2d.CreateInstancedQuadTex(this, img->GetTextureId(), img->GetSize(), { 0,0 }, { 1920,1080 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		r2d.UpdateInstancedQuadTexColor(this, m_TransitionInstance, { 0.1f, 0.1f, 0.1f, 1.0f });

		r2d.DisableInstance(this, m_TransitionInstance);

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
	}

	void Scene::Update()
	{
		switch (m_CurrentState)
		{
		case Scene::SCENE_ENTERING:
			m_EntityManager.UpdateWhitelist();

			UpdateEnter();
			RenderEnter();

			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			
			if (m_TransitionTimer >= mMaxTimeEntering) {
				m_TransitionTimer = 0;

				m_CurrentState = SCENE_LOOP;
				
				Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

				r2d.DisableInstance(this, m_TransitionInstance);
			}
			break;
		case Scene::SCENE_LOOP:
			if(!pausedGame)m_EntityManager.SystemsUpdate();
			m_GuiManager->Update();
			ProcessInput();
			UpdateLoop();
			RenderLoop();
			break;
		case Scene::SCENE_LEAVING:
			m_EntityManager.UpdateWhitelist();

			UpdateLeave();
			RenderLeave();

			m_TransitionTimer += (size_t)Wiwa::Time::GetRealDeltaTime();
			
			if (m_TransitionTimer >= mMaxTimeLeaving) {
				m_TransitionTimer = 0;

				Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

				r2d.DisableInstance(this, m_TransitionInstance);

				SceneManager::LoadSceneByIndex(m_SceneToChange, m_SceneChangeFlags);
			}
			break;
		default:
			break;
		}
	}

	void Scene::ModuleUpdate()
	{
		m_CameraManager->Update();

		Wiwa::Renderer2D &r2d = Wiwa::Application::Get().GetRenderer2D();
		r2d.UpdateInstanced(this);

		m_GuiManager->Draw();

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
		Audio::StopAllEvents();
		
		// Sleep to wait till Audio thread stops all events
		Sleep(10);

		if (unload_resources)
		{
			// TODO: Fix unloading for the editor
			return;
			Wiwa::Resources::UnloadAllResources();
		}
	}

	void Scene::ChangeScene(size_t scene, int flags)
	{
		m_SceneToChange = scene;
		m_SceneChangeFlags = flags;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;
	}

	void Scene::UpdateEnter()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		r2d.EnableInstance(this, m_TransitionInstance);
	}

	void Scene::UpdateLeave()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		r2d.EnableInstance(this, m_TransitionInstance);
	}

	void Scene::RenderEnter()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		float perc = m_TransitionTimer / (float)mMaxTimeEntering;
		int w = 256 * (1.0f - perc);

		Wiwa::Size2i size = {
			1920 * (1.0f - perc),
			1080
		};

		r2d.UpdateInstancedQuadTexClip(this, m_TransitionInstance, { 256,256 }, { 0,0,w,256 });
		r2d.UpdateInstancedQuadTexSize(this, m_TransitionInstance, {0,0}, size, Wiwa::Renderer2D::Pivot::UPLEFT);
	}

	void Scene::RenderLeave()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		float perc = m_TransitionTimer / (float)mMaxTimeLeaving;
		int w = 256 * perc;

		Wiwa::Size2i size = {
			1920 * perc,
			1080
		};

		r2d.UpdateInstancedQuadTexClip(this, m_TransitionInstance, { 256,256 }, { 0,0,w,256 });
		r2d.UpdateInstancedQuadTexSize(this, m_TransitionInstance, { 0,0 }, size, Wiwa::Renderer2D::Pivot::UPLEFT);
	}
}
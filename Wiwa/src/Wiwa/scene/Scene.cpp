#include <wipch.h>
#include "Scene.h"
#include "SceneManager.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/utilities/render/LightManager.h>

#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/Dialog/DialogManager.h>
#include <Wiwa/audio/Audio.h>
#include <Wiwa/AI/AIMapGeneration.h>
#include <Wiwa/ecs/systems/game/gui/PlayerGUISystem.h>
namespace Wiwa
{
	Scene::Scene()
	{
		mMaxTimeEntering = 450;
		mMaxTimeLeaving = 450;

		m_GuiManager = new GuiManager();
		m_GuiManager->Init(this);
		m_DialogManager = new DialogManager();
		m_DialogManager->Init(this);

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
		delete m_DialogManager;

		// Clear entity manager
		//m_EntityManager.Clear();

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
		PlayerGUISystem* pgs;

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
			if(!pausedGame)
				m_EntityManager.SystemsUpdate();
			pgs = m_EntityManager.GetSystem<PlayerGUISystem>(Wiwa::GameStateManager::GetPlayerId());
			pgs->Update();
			m_GuiManager->Update();
			m_DialogManager->Update();
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

		Application::Get().GetRenderer3D().RenderSkybox();
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

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
		m_PhysicsManager->DebugDrawWorld();
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
		GameStateManager::s_PoolManager->UnloadAllPools();

		//Audio::StopAllEvents();
		m_EntityManager.Clear();
		
		// Sleep to wait till Audio thread stops entity events
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
		WI_INFO("ChangeScene() with scene: {}", scene);
		m_SceneToChange = scene;
		m_SceneChangeFlags = flags;
		m_CurrentState = SCENE_LEAVING;
		m_TransitionTimer = 0;

		SceneChangeEvent event(scene);
		Action<Event&> action = { &Application::OnEvent, &Application::Get() };
		action(event);
	}

	uint32_t Scene::CreateInstanceRenderer()
	{
		uint32_t id = m_InstanceRenderers.size();

		m_InstanceRenderers.emplace_back(40500);

		m_InstanceRenderers[id].Init("resources/shaders/instanced_tex_color");

		return id;
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
		int w = (int)(256.f * (1.0f - perc));

		Wiwa::Size2i size = {
			(int)(1920 * (1.0f - perc)),
			1080
		};

		r2d.UpdateInstancedQuadTexClip(this, m_TransitionInstance, { 256,256 }, { 0,0,w,256 });
		r2d.UpdateInstancedQuadTexSize(this, m_TransitionInstance, {0,0}, size, Wiwa::Renderer2D::Pivot::UPLEFT);
	}

	void Scene::RenderLeave()
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		float perc = m_TransitionTimer / (float)mMaxTimeLeaving;
		int w = (int)(256.f * perc);

		Wiwa::Size2i size = {
			(int)(1920 * perc),
			1080
		};

		r2d.UpdateInstancedQuadTexClip(this, m_TransitionInstance, { 256,256 }, { 0,0,w,256 });
		r2d.UpdateInstancedQuadTexSize(this, m_TransitionInstance, { 0,0 }, size, Wiwa::Renderer2D::Pivot::UPLEFT);
	}
}
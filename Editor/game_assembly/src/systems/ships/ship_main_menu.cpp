#include "ship_main_menu.h"
#include "../../components/ship/ship_main_menu_data.h"
#include <Wiwa/utilities/easings.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/core/Input.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/audio/Audio.h>

Wiwa::ShipMainMenu::ShipMainMenu()
{
}

Wiwa::ShipMainMenu::~ShipMainMenu()
{
}

void Wiwa::ShipMainMenu::OnAwake()
{
	m_Time = 0.f;
	m_StartPos = GetTransform()->position;
	m_ShipDataIt = GetComponentIterator<ShipMainMenuData>();
	m_SceneChange = false;
	m_GlFWeskk = false;
	m_GLFWeskk2 = false;
}

void Wiwa::ShipMainMenu::OnInit()
{
	ShipMainMenuData* data = GetComponentByIterator<ShipMainMenuData>(m_ShipDataIt);
	data->PanToCamera = false;
}

void Wiwa::ShipMainMenu::OnUpdate()
{
	ShipMainMenuData* data = GetComponentByIterator<ShipMainMenuData>(m_ShipDataIt);
	m_Time += Time::GetDeltaTimeSeconds();
	
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

	
	if (gm.getCurrentCanvas() == 1)
	{
		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
		{
			m_GLFWeskk2 = true;
		}
		if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB) && m_GLFWeskk2)
		{
			gm.canvas.at(1)->SwapActive();
			gm.canvas.at(0)->SwapActive();
			m_GLFWeskk2 = false;
		}
	}
	
	
	if (data->PanToCamera)
	{
		PanToCamera();
		m_TimerToChangeScreen += Time::GetDeltaTimeSeconds();

		if (m_TimerToChangeScreen >= 5.0f)
		{
			Wiwa::SceneManager::ChangeSceneByIndex(3);
		}
	}

	if (m_SceneChange)
	{
		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
		{
			m_GlFWeskk = true;
			WI_INFO("PRESSED");
		}
		if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadA) && m_GlFWeskk)
		{
			Wiwa::SceneManager::ChangeSceneByIndex(4);
			m_GlFWeskk = false;
		}
	}
	float posY = GetTransform()->position.y;
	float move = sin(m_Time) * data->VelocityMove * Time::GetDeltaTimeSeconds();
	float rot = cos(m_Time) * data->VelocityRot * Time::GetDeltaTimeSeconds();
	GetTransform()->localPosition.y += move;
	GetTransform()->localRotation.x += rot;
}

void Wiwa::ShipMainMenu::PanToCamera()
{
	ShipMainMenuData* data = GetComponentByIterator<ShipMainMenuData>(m_ShipDataIt);
	PhysicsSystem* physics = m_Scene->GetEntityManager().GetSystem<PhysicsSystem>(m_EntityId);
	physics->getBody()->velocity = Math::ToBulletVector3(Math::CalculateForward(GetTransform()) * data->PanVelocity);

	Camera* camera = m_Scene->GetCameraManager().getActiveCamera();
	glm::vec3 rot =
	{	
		camera->getRotation().x + data->PanRotationVelocity * Time::GetDeltaTimeSeconds(),
		camera->getRotation().y,
		camera->getRotation().z
	};
	camera->setRotation(rot);
}

void Wiwa::ShipMainMenu::SetPanToCamera(bool ret)
{
	ShipMainMenuData* data = GetComponentByIterator<ShipMainMenuData>(m_ShipDataIt);

	data->PanToCamera = ret;
}

void Wiwa::ShipMainMenu::OnCollisionEnter(Object* body1, Object* body2)
{
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
	std::string playerTag = "PLAYER";

	if (body1->id == m_EntityId && body2->selfTagStr == playerTag)
	{
		gm.canvas.at(9)->SwapActive();
		m_SceneChange = true;
	}
}

void Wiwa::ShipMainMenu::OnCollisionExit(Object* body1, Object* body2)
{
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
	std::string playerTag = "PLAYER";

	if (body1->id == m_EntityId && body2->selfTagStr == playerTag)
	{
		gm.canvas.at(9)->SwapActive();
		m_SceneChange = false;
	}
}

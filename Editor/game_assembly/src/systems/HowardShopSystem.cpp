#include "HowardShopSystem.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/core/Input.h>
#include <Wiwa/Ui/UiManager.h>


namespace Wiwa
{

	Wiwa::HowardShopSystem::HowardShopSystem()
	{

	}

	HowardShopSystem::~HowardShopSystem()
	{
	}

	void HowardShopSystem::OnAwake()
	{
	}

	void HowardShopSystem::OnInit()
	{
		Wiwa::OzzAnimationSystem* animator = m_Scene->GetEntityManager().GetSystem<Wiwa::OzzAnimationSystem>(m_EntityId);

		animator->PlayAnimation("idle");
		m_ReturnToBaseHUD = false;
		m_ActiveSecondPage = false;
		m_FirstShopActive = false;
		m_SecondShopActive = false;
	}

	void HowardShopSystem::OnUpdate()
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
		Wiwa::Character* character = Wiwa::GameStateManager::GetPlayerCharacterComp();
		if (m_FirstShopActive)
		{
			character->CanMove = false;
			if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
			{
				m_ReturnToBaseHUD = true;
			}
			if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB) && m_ReturnToBaseHUD)
			{
				gm.canvas.at(7)->SwapActive();
				gm.canvas.at(0)->SwapActive();
				character->CanMove = true;
				m_ReturnToBaseHUD = false;
				m_FirstShopActive = false;
			}

			if (Wiwa::Input::IsKeyPressed(Key::Backslash))
			{
				m_ReturnToBaseHUD = true;
			}
			if (Wiwa::Input::IsKeyReleased(Key::Backslash) && m_ReturnToBaseHUD)
			{
				gm.canvas.at(7)->SwapActive();
				gm.canvas.at(0)->SwapActive();
				character->CanMove = true;
				m_ReturnToBaseHUD = false;
				m_FirstShopActive = false;
			}

			if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadRigthBumper))
			{
				m_ActiveSecondPage = true;
			}
			if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadRigthBumper) && m_ActiveSecondPage)
			{
				gm.canvas.at(7)->SwapActive();
				gm.canvas.at(8)->SwapActive();
				m_ActiveSecondPage = false;
				m_FirstShopActive = false;
				m_SecondShopActive = true;
			}

			if (Wiwa::Input::IsKeyPressed(Key::Right))
			{
				m_ActiveSecondPage = true;
			}
			if (Wiwa::Input::IsKeyReleased(Key::Right) && m_ActiveSecondPage)
			{
				gm.canvas.at(7)->SwapActive();
				gm.canvas.at(8)->SwapActive();
				m_ActiveSecondPage = false;
				m_FirstShopActive = false;
				m_SecondShopActive = true;
			}
		}

		if (m_SecondShopActive)
		{
			character->CanMove = false;

			if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadLeftBumper))
			{
				m_ActiveSecondPage = true;
			}
			if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadLeftBumper) && m_ActiveSecondPage)
			{
				gm.canvas.at(8)->SwapActive();
				gm.canvas.at(7)->SwapActive();
				m_ActiveSecondPage = false;
				m_SecondShopActive = false;
				m_FirstShopActive = true;

			}

			if (Wiwa::Input::IsKeyPressed(Key::Left))
			{
				m_ActiveSecondPage = true;
			}
			if (Wiwa::Input::IsKeyReleased(Key::Left) && m_ActiveSecondPage)
			{
				gm.canvas.at(8)->SwapActive();
				gm.canvas.at(7)->SwapActive();
				m_ActiveSecondPage = false;
				m_SecondShopActive = false;
				m_FirstShopActive = true;

			}

			if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
			{
				m_ReturnToBaseHUD = true;
			}
			if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB) && m_ReturnToBaseHUD)
			{
				gm.canvas.at(8)->SwapActive();
				gm.canvas.at(0)->SwapActive();
				character->CanMove = true;
				m_ReturnToBaseHUD = false;
				m_SecondShopActive = false;
			}

			if (Wiwa::Input::IsKeyPressed(Key::Backslash))
			{
				m_ReturnToBaseHUD = true;
			}
			if (Wiwa::Input::IsKeyReleased(Key::Backslash) && m_ReturnToBaseHUD)
			{
				gm.canvas.at(8)->SwapActive();
				gm.canvas.at(0)->SwapActive();
				character->CanMove = true;
				m_ReturnToBaseHUD = false;
				m_SecondShopActive = false;
			}
		}

	}

	void HowardShopSystem::OnDestroy()
	{
	}

	void HowardShopSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
		std::string playerStr = "PLAYER";
		if (body1->id == m_EntityId && playerStr == body2->selfTagStr)
		{
			gm.canvas.at(0)->SwapActive();
			gm.canvas.at(7)->SwapActive();
			m_FirstShopActive = true;
		}
	}
}
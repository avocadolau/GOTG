#include "wipch.h"
#include "PlayerGUISystem.h"
#include "Wiwa/ecs/components/game/items/Item.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/core/Input.h"

void Wiwa::PlayerGUISystem::OnUpdate()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	Character* character = Wiwa::GameStateManager::GetPlayerCharacterComp();
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	PlayerElements(gm, character);

	if (character->Health <= 0) DeathHud(gm);

	
}

void Wiwa::PlayerGUISystem::DeathHud(Wiwa::GuiManager& gm)
{
	Wiwa::SceneManager::PauseCurrentScene();
	gm.canvas.at(CanvasHUD)->SwapActive();
	//TODO: IMPLEMENT IT ONCE WE HAVE THE ARTgm.canvas.at(DeathHUD)->SwapActive();
}

void Wiwa::PlayerGUISystem::CooldownState(Ability** ability,Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
	for (int i = 0; i < 2; i++)
	{
		if (ability[i] != nullptr)
		{
			if (ability[i]->CurrentTime >= ability[i]->Cooldown)
			{
				ability[i]->CooldownState = CooldownState::FULLY_CHARGED;
			}
			else if (ability[i]->CurrentTime < ability[i]->Cooldown && ability[i]->CurrentTime >= ability[i]->Cooldown / 2)
			{
				ability[i]->CooldownState = CooldownState::MEDIUM_CHARGE;
			}
			else if (ability[i]->CurrentTime < ability[i]->Cooldown / 2 && ability[i]->CurrentTime > 0)
			{
				ability[i]->CooldownState = CooldownState::STARTING_CHARGE;
			}
			else if (ability[i]->CurrentTime < 0.0f)
			{
				ability[i]->CooldownState = CooldownState::NO_CHARGED;
			}
			//TODO: WHEN ART DONE WITH ABILITES ASSIGN THIS
			//gm.canvas.at(CanvasHUD)->controls.at(i+7)->SetNextFrame((int)ability[i]->CooldownState, &r2d);
		}
		
	}
}

void Wiwa::PlayerGUISystem::CooldownState(Buff** buff, Wiwa::GuiManager& gm)
{
	for (int i = 0; i < 2; i++)
	{
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		for (int i = 0; i < 2; i++)
		{
			if (buff[i] != nullptr)
			{
				if (buff[i]->CurrentTime >= buff[i]->Cooldown)
				{
					buff[i]->CooldownState = CooldownState::FULLY_CHARGED;
				}
				else if (buff[i]->CurrentTime < buff[i]->Cooldown && buff[i]->CurrentTime >= buff[i]->Cooldown / 2)
				{
					buff[i]->CooldownState = CooldownState::MEDIUM_CHARGE;
				}
				else if (buff[i]->CurrentTime < buff[i]->Cooldown / 2 && buff[i]->CurrentTime > 0)
				{
					buff[i]->CooldownState = CooldownState::STARTING_CHARGE;
				}
				else if (buff[i]->CurrentTime < 0.0f)
				{
					buff[i]->CooldownState = CooldownState::NO_CHARGED;
				}
				//TODO: WHEN ART DONE WITH BUFFS ASSIGN THIS
				//gm.canvas.at(CanvasHUD)->controls.at(i + 7)->SetNextFrame((int)buff[i]->CooldownState, &r2d);
			}
		}
	}
}

void Wiwa::PlayerGUISystem::PlayerElements(Wiwa::GuiManager& gm, Character* character)
{
	// Input
	const float rightTrigger = Input::GetAxis(Gamepad::GamePad1, Wiwa::Gamepad::RightTrigger);
	const float leftTrigger = Input::GetAxis(Gamepad::GamePad1, Gamepad::LeftTrigger);
	//Update hp & shield
	gm.canvas.at(CanvasHUD)->controls.at(1)->SetValueForUIbar(character->Health, character->MaxHealth);
	gm.canvas.at(CanvasHUD)->controls.at(2)->SetValueForUIbar(character->Shield, character->MaxShield);
	//TODO: Handle abilities
	abilitiesList = Wiwa::GameStateManager::GetPlayerInventory().GetAbilities();
	buffsList = Wiwa::GameStateManager::GetPlayerInventory().GetBuffs();
	CooldownState(abilitiesList, gm);
	CooldownState(buffsList, gm);

	gm.canvas.at(0)->controls.at(0)->ScaleGUIElement({ 900.0f,600.0f }, 100.0f,
		{ (float)gm.canvas.at(0)->controls.at(0)->position.width,(float)gm.canvas.at(0)->controls.at(0)->position.height },Wiwa::GuiControlEasing::EaseBounceInOut);

	if (leftTrigger >= 0.f)
	{
		
	}
	if (rightTrigger >= 0.f)
	{

	}
}


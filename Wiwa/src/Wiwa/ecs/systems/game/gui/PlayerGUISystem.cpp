#include "wipch.h"
#include "PlayerGUISystem.h"
#include "Wiwa/game/Items/ItemManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/core/Input.h"
#include "../CharacterInventory.h"

void Wiwa::PlayerGUISystem::OnUpdate()
{
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
	Character* character = Wiwa::GameStateManager::GetPlayerCharacterComp();
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
	CharacterInventory* characterInventory = m_Scene->GetEntityManager().GetSystem<CharacterInventory>(m_EntityId);

	PlayerElements(gm, character);
	CurrentHUD = gm.getCurrentCanvas();
	if (CurrentHUD == CanvasHUD)
	{
		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadStart))
		{
			pauseGame = true;
		}
		if (Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadStart) && pauseGame)
		{
			gm.canvas.at(CanvasHUD)->SwapActive();
			gm.canvas.at(PauseHUD)->SwapActive();
			pauseGame = false;
			m_Scene->SwapPauseActive();
		}
	}
	if (CurrentHUD == PauseHUD)
	{
		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
		{
			returnToHUD = true;
		}
		if (returnToHUD && Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB))
		{
			gm.canvas.at(CanvasHUD)->SwapActive();
			gm.canvas.at(PauseHUD)->SwapActive();
			returnToHUD = false;
			m_Scene->SwapPauseActive();
		}
	}
	if (CurrentHUD == OptionsHUD)
	{
		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
		{
			returnToPauseHUD = true;
		}
		if (returnToPauseHUD && Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB))
		{
			gm.canvas.at(OptionsHUD)->SwapActive();
			gm.canvas.at(PauseHUD)->SwapActive();
			returnToPauseHUD = false;
		}
	}
	if (character->Health <= 0 && !deathHud)
	{
		DeathHud(gm);
		deathHud = true;
	}
	if (characterInventory != NULL)
	{
		if (characterInventory->GetShopActive())
		{
			ShopElementsHUD(characterInventory->GetCurrentShopItem());
		}
	}
}

void Wiwa::PlayerGUISystem::DeathHud(Wiwa::GuiManager& gm)
{
	Wiwa::SceneManager::PauseCurrentScene();
	gm.canvas.at(CanvasHUD)->SwapActive();
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
			int index = i + 15;
			gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame((int)ability[i]->CooldownState, &r2d);
		}
		
	}
	
}

void Wiwa::PlayerGUISystem::CooldownState(Buff** buff, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	for (int i = 0; i < 2; i++)
	{
		
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
				int index = i + 13;
				gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame((int)buff[i]->CooldownState, &r2d);					
			}
		}
	}
	
}

void Wiwa::PlayerGUISystem::HandleActiveAbilities(Ability** ability, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

		for (int i = 0; i < 2; i++)
		{
			int index = i + 11;
			int index_pause = i + 7;
			int index_death = i + 4;
			if (ability[i] != nullptr)
			{
				switch (ability[i]->AbilityType)
				{
				case AbilityType::YONDUS_FIN:
					if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(3, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(3, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(3, &r2d);
					}
					else
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(4, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(4, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(4, &r2d);
					}
					break;
				case AbilityType::GROOTS_SEEDS:
					if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(5, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(5, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(5, &r2d);
					}
					else
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(6, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(6, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(6, &r2d);
					}
					break;
				case AbilityType::PHYLAS_QUANTUM_SWORD:
					if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(7, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(7, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(7, &r2d);
					}
					else
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(8, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(8, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(8, &r2d);
					}
					break;
				case AbilityType::STARHAWKS_BLAST:
					if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(1, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(1, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(1, &r2d);
					}
					else
					{
						gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(2, &r2d);
						gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(2, &r2d);
						gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(2, &r2d);
					}
					break;
				default:
					break;
				}
			}
			else
			{
				gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(0, &r2d);
				gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(0, &r2d);
				gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(0, &r2d);
			}
		}
	
}

void Wiwa::PlayerGUISystem::HandleActiveBuffs(Buff** buff, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	
	for (int i = 0; i < 2; i++)
	{
		int index = i + 9;
		int index_pause = i + 5;
		int index_death = i + 2;
		if (buff[i] != nullptr)
		{
			switch (buff[i]->buffType)
			{
			case BuffType::NIKKIS_TOUCH:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(1, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(1, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(1, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(2, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(2, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(2, &r2d);
				}
				break;
			case BuffType::MARTINEX_THERMOKINESIS:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(3, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(3, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(3, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(4, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(4, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(4, &r2d);
				}
				break;
			case BuffType::MAJOR_VICTORY_SHIELD:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(5, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(5, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(5, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(6, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(6, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(6, &r2d);
				}
				break;
			case BuffType::CHARLIE27_FIST:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(7, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(7, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(7, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(8, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(8, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(8, &r2d);
				}
				break;
			case BuffType::COSMOS_PAW:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(9, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(9, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(9, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(10, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(10, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(10, &r2d);
				}
				break;
			case BuffType::BUGS_LEGS:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(11, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(11, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(11, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(12, &r2d);
					gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(12, &r2d);
					gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(12, &r2d);
				}
				break;
			default:
				break;
			}

		}
		else
		{
			gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(0, &r2d);
			gm.canvas.at(PauseHUD)->controls.at(index_pause)->SetNextFrame(0, &r2d);
			gm.canvas.at(DeathHUD)->controls.at(index_death)->SetNextFrame(0, &r2d);
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
	
	abilitiesList = Wiwa::GameStateManager::GetPlayerInventory().GetAbilities();
	buffsList = Wiwa::GameStateManager::GetPlayerInventory().GetBuffs();
	
	if (lastCoins != Wiwa::GameStateManager::GetPlayerInventory().GetTokens())
	{
		Coins(Wiwa::GameStateManager::GetPlayerInventory().GetTokens(), gm);
	}

	CooldownState(abilitiesList, gm);
	CooldownState(buffsList, gm);
	HandleActiveAbilities(abilitiesList, gm);
	HandleActiveBuffs(buffsList, gm);

	lastCoins = Wiwa::GameStateManager::GetPlayerInventory().GetTokens();

}

void Wiwa::PlayerGUISystem::PauseElementsUpdate(Ability** ability, Buff** buff, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	
	for (int i = 0; i < 2; i++)
	{
		int index = i + 5;
		if (buff[i] != nullptr)
		{
			switch (buff[i]->buffType)
			{
			case BuffType::NIKKIS_TOUCH:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(1, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(2, &r2d);
				}
				break;
			case BuffType::MARTINEX_THERMOKINESIS:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(3, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(4, &r2d);
				}
				break;
			case BuffType::MAJOR_VICTORY_SHIELD:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(5, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(6, &r2d);
				}
				break;
			case BuffType::CHARLIE27_FIST:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(7, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(8, &r2d);
				}
				break;
			case BuffType::COSMOS_PAW:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(9, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(10, &r2d);
				}
				break;
			case BuffType::BUGS_LEGS:
				if (buff[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(11, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(12, &r2d);
				}
				break;
			default:
				break;
			}

		}
		else
		{
			gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(0, &r2d);
		}
	}
	

	for (int i = 0; i < 2; i++)
	{
		int index = i + 7;
		if (ability[i] != nullptr)
		{
			switch (ability[i]->AbilityType)
			{
			case AbilityType::YONDUS_FIN:
				if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(3, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(4, &r2d);
				}
				break;
			case AbilityType::GROOTS_SEEDS:
				if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(5, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(6, &r2d);
				}
				break;
			case AbilityType::PHYLAS_QUANTUM_SWORD:
				if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(7, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(8, &r2d);
				}
				break;
			case AbilityType::STARHAWKS_BLAST:
				if (ability[i]->CooldownState != CooldownState::FULLY_CHARGED)
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(1, &r2d);
				}
				else
				{
					gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(2, &r2d);
				}
				break;
			default:
				break;
			}
		}
		else
		{
			gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame(0, &r2d);
		}
	}
	
}

void Wiwa::PlayerGUISystem::Coins(uint32_t coins, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
	std::string my_string = std::to_string(coins);

	gm.canvas.at(CanvasHUD)->controls.at(5)->text = my_string.c_str();
	Text* newText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", my_string.c_str());
	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, newText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, newText->GetSize(), {0,0,512,512});
}

void Wiwa::PlayerGUISystem::ShopElementsHUD(Item* currentItem)
{
	if (currentItem->item_type == 0) //Ability
	{
		Ability* ability = Wiwa::ItemManager::GetAbility(currentItem->Name);
	}
	else if (currentItem->item_type == 2) //Buff
	{
		Buff* ability = Wiwa::ItemManager::GetBuff(currentItem->Name);
	}
}

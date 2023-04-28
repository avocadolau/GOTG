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
	if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadY))
	{
		Wiwa::GameStateManager::GetPlayerInventory().AddTokens(15);
	}
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
	if (CurrentHUD == ShopHUD)
	{
		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
		{
			buyItem = true;
		}
		if (buyItem && Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadA))
		{
			characterInventory->ShopElement(characterInventory->GetCurrentShopItem());
			buyItem = false;
		}

		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadB))
		{
			shopHUD = true;
		}
		if (shopHUD && Wiwa::Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadB))
		{
			gm.canvas.at(ShopHUD)->SwapActive();
			gm.canvas.at(CanvasHUD)->SwapActive();
			shopHUD = false;
		}
	}
	if (character->Health <= 0 && !deathHud)
	{
		DeathHud(gm);
		deathHud = true;
	}
	if (characterInventory != NULL)
	{
		if (shopActive)
		{
			ShopElementsHUD(characterInventory->GetCurrentShopItem(),gm);
			shopActive = false;
		}
		if (characterInventory->PlayerUpdated())
		{
			HandlePlayerStatistics(character->MaxHealth, character->MaxShield, 10.0f, character->Damage, character->Speed, character->RateOfFire, gm);
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
	gm.canvas.at(ShopHUD)->controls.at(2)->text = my_string.c_str();
	Text* newText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", my_string.c_str());
	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, newText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, newText->GetSize(), {0,0,512,512});

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(2)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(2)->id_quad_normal, newText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(2)->id_quad_normal, newText->GetSize(), { 0,0,512,512 });
}

void Wiwa::PlayerGUISystem::HandlePlayerStatistics(int max_Health, int max_Shield, float range, int attack, float speed, float rateOfFire, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	std::string maxHealthString = std::to_string(max_Health);
	gm.canvas.at(PauseHUD)->controls.at(5)->text = maxHealthString.c_str();

	std::string maxShieldString = std::to_string(max_Shield);
	gm.canvas.at(PauseHUD)->controls.at(5)->text = maxShieldString.c_str();

	std::string rangeString = std::to_string(range);
	gm.canvas.at(PauseHUD)->controls.at(5)->text = rangeString.c_str();

	std::string attackString = std::to_string(attack);
	gm.canvas.at(PauseHUD)->controls.at(5)->text = attackString.c_str();

	std::string speedString = std::to_string(speed);
	gm.canvas.at(PauseHUD)->controls.at(5)->text = speedString.c_str();

	std::string rateOfFireString = std::to_string(rateOfFire);
	gm.canvas.at(PauseHUD)->controls.at(5)->text = rateOfFireString.c_str();

	Text* maxHealthText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", maxHealthString.c_str());
	Text* maxShieldText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", maxShieldString.c_str());
	Text* rangeText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", rangeString.c_str());
	Text* attackText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", attackString.c_str());
	Text* speedText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", speedString.c_str());
	Text* rateOfFireText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", rateOfFireString.c_str());

	//PAUSE CANVAS
	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(PauseHUD)->controls.at(9)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(PauseHUD)->controls.at(9)->id_quad_normal, maxHealthText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(PauseHUD)->controls.at(9)->id_quad_normal, maxHealthText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(PauseHUD)->controls.at(10)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(PauseHUD)->controls.at(10)->id_quad_normal, maxShieldText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(PauseHUD)->controls.at(10)->id_quad_normal, maxShieldText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(PauseHUD)->controls.at(11)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(PauseHUD)->controls.at(11)->id_quad_normal, rangeText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(PauseHUD)->controls.at(11)->id_quad_normal, rangeText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(PauseHUD)->controls.at(12)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(PauseHUD)->controls.at(12)->id_quad_normal, attackText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(PauseHUD)->controls.at(12)->id_quad_normal, attackText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(PauseHUD)->controls.at(13)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(PauseHUD)->controls.at(13)->id_quad_normal, speedText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(PauseHUD)->controls.at(13)->id_quad_normal, speedText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(PauseHUD)->controls.at(14)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(PauseHUD)->controls.at(14)->id_quad_normal, rateOfFireText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(PauseHUD)->controls.at(14)->id_quad_normal, rateOfFireText->GetSize(), { 0,0,512,512 });

	// DEATH CANVAS
	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(DeathHUD)->controls.at(6)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(DeathHUD)->controls.at(6)->id_quad_normal, maxHealthText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(DeathHUD)->controls.at(6)->id_quad_normal, maxHealthText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(DeathHUD)->controls.at(7)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(DeathHUD)->controls.at(7)->id_quad_normal, maxShieldText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(DeathHUD)->controls.at(7)->id_quad_normal, maxShieldText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(DeathHUD)->controls.at(8)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(DeathHUD)->controls.at(8)->id_quad_normal, rangeText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(DeathHUD)->controls.at(8)->id_quad_normal, rangeText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(DeathHUD)->controls.at(9)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(DeathHUD)->controls.at(9)->id_quad_normal, attackText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(DeathHUD)->controls.at(9)->id_quad_normal, attackText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(DeathHUD)->controls.at(10)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(DeathHUD)->controls.at(10)->id_quad_normal, speedText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(DeathHUD)->controls.at(10)->id_quad_normal, speedText->GetSize(), { 0,0,512,512 });

	r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(DeathHUD)->controls.at(11)->id_quad_normal, 1);
	r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(DeathHUD)->controls.at(11)->id_quad_normal, rateOfFireText->GetTextureId());
	r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(DeathHUD)->controls.at(11)->id_quad_normal, rateOfFireText->GetSize(), { 0,0,512,512 });
}

void Wiwa::PlayerGUISystem::ShopElementsHUD(Item* currentItem, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
	gm.canvas.at(CanvasHUD)->SwapActive();
	gm.canvas.at(ShopHUD)->SwapActive();
	const char* tags[] =
	{
		"Attack",
		"Projectile",
		"Aoe",
		"Debuff",
		"Homing",
	};
	if (currentItem->item_type == 0) //Ability
	{
		Ability* ability = Wiwa::ItemManager::GetAbility(currentItem->Name);

		std::string ability_name = ability->Name;
		gm.canvas.at(ShopHUD)->controls.at(0)->text = ability_name.c_str();

		std::string ability_price = std::to_string(ability->Price);
		gm.canvas.at(ShopHUD)->controls.at(1)->text = ability_price.c_str();
		//TODO: tags
		std::string ability_tag_1 = tags[(int)ability->itemTag[0]];
		gm.canvas.at(ShopHUD)->controls.at(3)->text = ability_tag_1.c_str();

		std::string ability_tag_2 = tags[(int)ability->itemTag[1]];
		gm.canvas.at(ShopHUD)->controls.at(4)->text = ability_tag_1.c_str();

		std::string ability_damage = std::to_string(ability->Damage);
		gm.canvas.at(ShopHUD)->controls.at(5)->text = ability_damage.c_str();

		std::string ability_Range = std::to_string(ability->Range);
		gm.canvas.at(ShopHUD)->controls.at(6)->text = ability_Range.c_str();

		std::string ability_CD = std::to_string(ability->Cooldown);
		gm.canvas.at(ShopHUD)->controls.at(7)->text = ability_CD.c_str();

		std::string ability_Area = std::to_string(ability->Area);
		gm.canvas.at(ShopHUD)->controls.at(8)->text = ability_Area.c_str();

		//TODO: Do description with wrappedText
		Text* abilityNameText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_name.c_str());
		Text* abilityPriceText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_price.c_str());
		Text* abilityTag1 = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_tag_1.c_str());
		Text* abilityTag2 = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_tag_2.c_str());
		Text* abilityDamageText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_damage.c_str());
		Text* abilityRangeText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_Range.c_str());
		Text* abilityCDText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_CD.c_str());
		Text* AreaText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", ability_Area.c_str());

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(0)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(0)->id_quad_normal, abilityNameText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(0)->id_quad_normal, abilityNameText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(1)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(1)->id_quad_normal, abilityPriceText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(1)->id_quad_normal, abilityPriceText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(3)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(3)->id_quad_normal, abilityTag1->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(3)->id_quad_normal, abilityTag1->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(4)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(4)->id_quad_normal, abilityTag2->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(4)->id_quad_normal, abilityTag2->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(5)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(5)->id_quad_normal, abilityDamageText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(5)->id_quad_normal, abilityDamageText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(6)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(6)->id_quad_normal, abilityRangeText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(6)->id_quad_normal, abilityRangeText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(7)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(7)->id_quad_normal, abilityCDText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(7)->id_quad_normal, abilityCDText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(8)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(8)->id_quad_normal, AreaText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(8)->id_quad_normal, AreaText->GetSize(), { 0,0,512,512 });
	}
	else if (currentItem->item_type == 2) //Buff
	{
		Buff* buff = Wiwa::ItemManager::GetBuff(currentItem->Name);

		std::string buff_name = buff->Name;
		gm.canvas.at(ShopHUD)->controls.at(0)->text = buff_name.c_str();

		std::string buff_price = std::to_string(buff->Price);
		gm.canvas.at(ShopHUD)->controls.at(1)->text = buff_price.c_str();

		std::string buff_damage = std::to_string(buff->AttackDmgInc);
		gm.canvas.at(ShopHUD)->controls.at(5)->text = buff_damage.c_str();

		std::string buff_Range = std::to_string(buff->BuffPercent);
		gm.canvas.at(ShopHUD)->controls.at(6)->text = buff_Range.c_str();

		std::string buff_CD = std::to_string(buff->Cooldown);
		gm.canvas.at(ShopHUD)->controls.at(7)->text = buff_CD.c_str();

		std::string buff_Area = std::to_string(buff->RoFInc);
		gm.canvas.at(ShopHUD)->controls.at(8)->text = buff_Area.c_str();

		Text* buffNameText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", buff_name.c_str());
		Text* buffPriceText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", buff_price.c_str());
		Text* buffDamageText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", buff_damage.c_str());
		Text* buffRangeText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", buff_Range.c_str());
		Text* buffCDText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", buff_CD.c_str());
		Text* buffAreaText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", buff_Area.c_str());

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(0)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(0)->id_quad_normal, buffNameText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(0)->id_quad_normal, buffNameText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(1)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(1)->id_quad_normal, buffPriceText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(1)->id_quad_normal, buffPriceText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(5)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(5)->id_quad_normal, buffDamageText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(5)->id_quad_normal, buffDamageText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(6)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(6)->id_quad_normal, buffRangeText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(6)->id_quad_normal, buffRangeText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(7)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(7)->id_quad_normal, buffCDText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(7)->id_quad_normal, buffCDText->GetSize(), { 0,0,512,512 });

		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(ShopHUD)->controls.at(8)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(ShopHUD)->controls.at(8)->id_quad_normal, buffAreaText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(ShopHUD)->controls.at(8)->id_quad_normal, buffAreaText->GetSize(), { 0,0,512,512 });
	}
}

void Wiwa::PlayerGUISystem::OnCollisionEnter(Object* body1, Object* body2)
{
	std::string tag_item = "ITEM";

	if (body1->id == m_EntityId && tag_item == body2->selfTagStr)
	{
		Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		Wiwa::EntityManager& em = _scene->GetEntityManager();
		Item* item = em.GetComponent<Item>(body2->id);
		if (!item)
		{
			WI_CORE_ERROR("Item component can't be find");
			return;
		}
		if (item->item_type == 0)//ABILITY
		{
			shopActive = true;
		}
		else if (item->item_type == 2)//BUFF
		{
			shopActive = true;
		}
	}
}

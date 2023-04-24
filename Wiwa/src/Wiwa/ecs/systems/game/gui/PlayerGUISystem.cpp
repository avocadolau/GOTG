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

	if (character->Health <= 0 && !deathHud)
	{
		DeathHud(gm);
		deathHud = true;
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
			int index_death = i + 5;
			gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame((int)ability[i]->CooldownState, &r2d);
			gm.canvas.at(deathHud)->controls.at(index_death)->SetNextFrame((int)ability[i]->CooldownState, &r2d);

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
				int index_death = i + 3;
				gm.canvas.at(CanvasHUD)->controls.at(index)->SetNextFrame((int)buff[i]->CooldownState, &r2d);
				gm.canvas.at(deathHud)->controls.at(index_death)->SetNextFrame((int)buff[i]->CooldownState, &r2d);
					
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

void Wiwa::PlayerGUISystem::HandleActiveBuffs(Buff** buff, Wiwa::GuiManager& gm)
{
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	
	for (int i = 0; i < 2; i++)
	{
		int index = i + 9;
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
		//Coins(Wiwa::GameStateManager::GetPlayerInventory().GetTokens(), gm);
	}

	CooldownState(abilitiesList, gm);
	CooldownState(buffsList, gm);
	HandleActiveAbilities(abilitiesList, gm);
	HandleActiveBuffs(buffsList, gm);


	PauseElementsUpdate(abilitiesList, buffsList, gm);

	lastCoins = Wiwa::GameStateManager::GetPlayerInventory().GetTokens();

	if (leftTrigger >= 0.f)
	{
		
	}
	if (rightTrigger >= 0.f)
	{

	}
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

	Wiwa::Text* newText = new Wiwa::Text;
	const char* newWord = my_string.c_str();
	gm.canvas.at(CanvasHUD)->controls.at(5)->text = newWord;
	newText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", newWord);
	r2d.UpdateInstancedQuadTexClip(Wiwa::SceneManager::getActiveScene(), gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, newText->GetSize(), {0,0,512,512});
	r2d.UpdateInstancedQuadTexTexture(Wiwa::SceneManager::getActiveScene(), gm.canvas.at(CanvasHUD)->controls.at(5)->id_quad_normal, newText->GetTextureId());
}


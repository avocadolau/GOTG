#include "CharacterSelector.h"
void Wiwa::CharacterSelector::OnAwake()
{
	m_Activated = false;
	m_CanInteract = false;
}
void Wiwa::CharacterSelector::OnInit()
{
	m_CanInteract = Wiwa::GameStateManager::s_GameProgression->IsRocketUnlocked();
}
void Wiwa::CharacterSelector::OnUpdate()
{
}
void Wiwa::CharacterSelector::OnCollisionEnter(Object* obj1, Object* obj2)
{
	if (!m_CanInteract)
		return;
	//TODO: Alejandro pop the image here
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

	gm.canvas.at(10)->SwapActive();
}

void Wiwa::CharacterSelector::OnCollision(Object* obj1, Object* obj2)
{
	if (!m_CanInteract)
		return;
	if (obj1 != obj2)
	{
		if (m_Activated)
			return;

		if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadY)
			|| Wiwa::Input::IsKeyPressed(Key::Enter))
		{
			Wiwa::SceneManager::LoadSceneByIndex(3);
			Wiwa::GameStateManager::s_CurrentCharacter = Wiwa::GameStateManager::s_CurrentCharacter == STARLORD ? ROCKET : STARLORD;
			m_Activated = true;
		}
	}
}

void Wiwa::CharacterSelector::OnCollisionExit(Object* obj1, Object* obj2)
{
	if (!m_CanInteract)
		return;
	// TODO: Alejandro hide the image here
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

	gm.canvas.at(10)->SwapActive();
}

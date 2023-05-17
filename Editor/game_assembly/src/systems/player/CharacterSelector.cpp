#include "CharacterSelector.h"
#include <Wiwa/ecs/components/ParticleEmitterComponent.h>
void Wiwa::CharacterSelector::OnAwake()
{
	m_Activated = false;
	m_CanInteract = false;
}
void Wiwa::CharacterSelector::OnInit()
{
	m_CanInteract = Wiwa::GameStateManager::s_GameProgression->IsRocketUnlocked();

	Wiwa::EntityManager& em = m_Scene->GetEntityManager();

	if (Wiwa::GameStateManager::s_CurrentCharacter == STARLORD)
	{
		EntityId m_hologramId = em.GetChildByName(m_EntityId, "p_hologram");

		ParticleEmitterComponent* particle = em.GetComponent<ParticleEmitterComponent>(m_hologramId);

		if (!particle)return;

		strcpy(particle->m_meshPath, "library/models/starlord/sk_rocket01b.wimodel");
		particle->m_meshChanged = true;
	}
	else {
		EntityId m_hologramId = em.GetChildByName(m_EntityId, "p_hologram");

		ParticleEmitterComponent* particle = em.GetComponent<ParticleEmitterComponent>(m_hologramId);

		if (!particle)return;
		
		strcpy(particle->m_meshPath, "library/characters/rocket/sm_starlord_01.wimodel");
		particle->m_meshChanged = true;
	}
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

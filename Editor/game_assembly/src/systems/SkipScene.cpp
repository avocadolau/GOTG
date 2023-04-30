#include "SkipScene.h"

void Wiwa::SkipScene::OnInit()
{
	m_SceneChange = false;
}

void Wiwa::SkipScene::OnUpdate()
{
	if (Input::IsKeyPressed(Key::Space)
	||	Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
	{
		m_SceneChange = true;
	}
	if (Input::IsKeyReleased(Key::Space) || Input::IsButtonReleased(Gamepad::GamePad1, Key::GamepadA) && m_SceneChange)
	{
		SceneManager::ChangeSceneByIndex(SceneManager::getActiveSceneId() + 1);
		m_SceneChange = false;
	}
}

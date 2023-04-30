#include "SkipScene.h"

void Wiwa::SkipScene::OnUpdate()
{
	if (Input::IsKeyPressed(Key::Space)
	||	Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
	{
		SceneManager::ChangeSceneByIndex(SceneManager::getActiveSceneId() + 1);
	}
}

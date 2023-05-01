#include "SkipIntro.h"

void Wiwa::SkipIntro::OnUpdate()
{
	if (Input::IsKeyPressed(Key::Space) || Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA))
	{
		SceneManager::ChangeSceneByIndex(1);
	}
}

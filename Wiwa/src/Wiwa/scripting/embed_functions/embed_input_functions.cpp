#include "wipch.h"
#include "embed_input_functions.h"
#include <Wiwa/core/Input.h>

bool IsKeyDownIntr(Wiwa::KeyCode keycode)
{
	return Wiwa::Input::IsKeyPressed(keycode);
}

bool IsMouseButtonPressedIntr(int button)
{
	return Wiwa::Input::IsMouseButtonPressed(button);
}

float GetMouseXIntr()
{
	return Wiwa::Input::GetMouseX();
}

float GetMouseYIntr()
{
	return Wiwa::Input::GetMouseY();
}

float GetMouseXDeltaIntr()
{
	return Wiwa::Input::GetMouseXDelta();
}

float GetMouseYDeltaIntr()
{
	return Wiwa::Input::GetMouseYDelta();
}

bool IsButtonPressedIntr(Wiwa::GamepadNum gamepad, Wiwa::KeyCode button)
{
	return Wiwa::Input::IsButtonPressed(gamepad, button);
}

float GetAxisIntr(Wiwa::GamepadNum gamepad, Wiwa::GamepadAxis axis, float deadzone)
{
	return Wiwa::Input::GetAxis(gamepad, axis, deadzone);
}

float GetRawAxisIntr(Wiwa::GamepadNum gamepad, Wiwa::GamepadAxis axis, float deadzone)
{
	return Wiwa::Input::GetRawAxis(gamepad, axis, deadzone);
}

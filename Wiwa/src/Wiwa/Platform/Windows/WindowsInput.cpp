#include "wipch.h"
#include "WindowsInput.h"

#include <Wiwa/core/KeyCodes.h>
#include <Wiwa/core/Application.h>

#include <GLFW/glfw3.h>

namespace Wiwa
{
	Input* Input::s_Instance = new WindowsInput();
	float Input::MouseXDelta = 0;
	float Input::MouseYDelta = 0;
	float Input::MouseX = 0;
	float Input::MouseY = 0;
	float Input::prevMouseX = 0;
	float Input::prevMouseY = 0;
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		return {MouseX, MouseY};
	}
	float WindowsInput::GetMouseXImpl()
	{
		return MouseX;
	}
	float WindowsInput::GetMouseYImpl()
	{
		return MouseY;
	}

	float WindowsInput::GetMouseXDeltaImpl()
	{
		return MouseXDelta;
	}

	float WindowsInput::GetMouseYDeltaImpl()
	{
		return MouseYDelta;
	}

	void WindowsInput::UpdateImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		MouseX = (float)xpos;
		MouseY = (float)ypos;

		MouseXDelta = MouseX - prevMouseX;
		MouseYDelta = MouseY - prevMouseY;
		CLAMP(MouseXDelta, -1.0f, 1.0f);
		CLAMP(MouseYDelta, -1.0f, 1.0f);

		prevMouseX = MouseX;
		prevMouseY = MouseY;
	}

	void WindowsInput::LockCursorImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowsInput::UnlockCursorImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
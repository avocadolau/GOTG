#include "wipch.h"
#include "WindowsInput.h"

#include <Wiwa/core/KeyCodes.h>
#include <Wiwa/core/Application.h>

#include <Wiwa/Platform/Windows/WindowsWindow.h>

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

	size_t Input::m_MaxKeys = GLFW_KEY_LAST + 1;
	Input::KeyState* Input::m_KeyStates = new Input::KeyState[Input::m_MaxKeys]{ Input::KeyState::KS_NONE };

	size_t Input::m_MaxButtons = GLFW_MOUSE_BUTTON_LAST + 1;
	Input::KeyState* Input::m_ButtonStates = new Input::KeyState[Input::m_MaxButtons]{ Input::KeyState::KS_NONE };

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		return m_KeyStates[keycode] == KeyState::KS_PRESS;
	}
	bool WindowsInput::IsKeyRepeatImpl(int keycode)
	{
		return m_KeyStates[keycode] == KeyState::KS_REPEAT;
	}
	bool WindowsInput::IsKeyReleasedImpl(int keycode)
	{
		return m_KeyStates[keycode] == KeyState::KS_RELEASE;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		return m_ButtonStates[button] == KeyState::KS_PRESS;
	}
	bool WindowsInput::IsMouseButtonReleasedImpl(int button)
	{
		return m_ButtonStates[button] == KeyState::KS_RELEASE;
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

	void WindowsInput::InitImpl()
	{
		Wiwa::Window& window = Wiwa::Application::Get().GetWindow();

		Action<Event&> ac = { &WindowsInput::OnWindowEvent, this };

		window.SetEventCallback(ac);
	}

	void WindowsInput::UpdateImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		// Calculate mouse coordinates
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

		// Reset inputs
		memset(m_KeyStates, Input::KeyState::KS_NONE, m_MaxKeys * sizeof(int));
		memset(m_ButtonStates, Input::KeyState::KS_NONE, m_MaxButtons * sizeof(int));
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
	float WindowsInput::GetAxisImpl(int gamepadIndx, int axis, float deadzone)
	{
		GLFWgamepadstate state;
		if (glfwGetGamepadState(gamepadIndx, &state))
		{
			float input = state.axes[axis];
			input = round(input * 10) / 10;
			if (input >= -1 && input <= -deadzone)
				return -1;
			else if (input >= deadzone && input <= 1)
				return 1;
		}
		return 0.f;
	}
	float WindowsInput::GetRawAxisImpl(int gamepadIndx, int axis, float deadzone)
	{
		GLFWgamepadstate state;
		if (glfwGetGamepadState(gamepadIndx, &state))
		{
			float input = state.axes[axis];
			input = round(input * 10) / 10;
			
			float absInput = abs(input);

			if (absInput >= deadzone)
				return input;
		}
		return 0.f;
	}
	bool WindowsInput::IsButtonPressedImpl(int gamepadIndx, int keycode)
	{
		GLFWgamepadstate state;
		if(glfwGetGamepadState(gamepadIndx, &state))
		{
			if (state.buttons[keycode] == GLFW_PRESS)
			{
				return true;
			}
		}
		return false;
	
	}

	bool WindowsInput::IsButtonReleasedImpl(int gamepadIndx, int keycode)
	{
		GLFWgamepadstate state;
		if (glfwGetGamepadState(gamepadIndx, &state))
		{
			if (state.buttons[keycode] == GLFW_RELEASE)
			{
				return true;
			}
		}
		return false;

	}

	void WindowsInput::OnWindowEvent(Event& ev)
	{
		Wiwa::EventType ev_type = ev.GetEventType();

		switch (ev_type) {
			case Wiwa::EventType::KeyPressed:
			{
				Wiwa::KeyPressedEvent& kpev = (Wiwa::KeyPressedEvent&)ev;

				if (kpev.IsRepeat()) m_KeyStates[kpev.GetKeyCode()] = KeyState::KS_REPEAT;
				else m_KeyStates[kpev.GetKeyCode()] = KeyState::KS_PRESS;
			}break;
			case Wiwa::EventType::KeyReleased:
			{
				Wiwa::KeyReleasedEvent& krev = (Wiwa::KeyReleasedEvent&)ev;

				m_KeyStates[krev.GetKeyCode()] = KeyState::KS_RELEASE;
			}break;
			case Wiwa::EventType::MouseButtonPressed:
			{
				Wiwa::MouseButtonPressedEvent& mpev = (Wiwa::MouseButtonPressedEvent&)ev;

				m_ButtonStates[mpev.GetMouseButton()] = KeyState::KS_PRESS;
			}break;
			case Wiwa::EventType::MouseButtonReleased:
			{
				Wiwa::MouseButtonReleasedEvent& mrev = (Wiwa::MouseButtonReleasedEvent&)ev;

				m_ButtonStates[mrev.GetMouseButton()] = KeyState::KS_RELEASE;
			}break;
		default:
			break;
		}
	}
}
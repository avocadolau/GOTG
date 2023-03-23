#pragma once

#include <Wiwa/core/Core.h>

namespace Wiwa
{
	class WI_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }	
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static bool IsMouseButtonReleased(int button) { return s_Instance->IsMouseButtonReleasedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static float GetMouseXDelta() { return s_Instance->GetMouseXDeltaImpl(); }
		inline static float GetMouseYDelta() { return s_Instance->GetMouseYDeltaImpl(); }
		inline static void LockCursor() { s_Instance->LockCursorImpl(); }
		inline static void UnlockCursor() { s_Instance->UnlockCursorImpl(); }
		inline static float GetAxis(int gamepadIndx, int axis, float deadzone = 0.6) { return s_Instance->GetAxisImpl(gamepadIndx, axis, deadzone); }
		inline static float GetRawAxis(int gamepadIndx, int axis, float deadzone) { return s_Instance->GetRawAxisImpl(gamepadIndx, axis, deadzone); }
		inline static bool IsButtonPressed(int gamepadIndx,int keycode) { return s_Instance->IsButtonPressedImpl(gamepadIndx, keycode); }

		inline static void Update() { return s_Instance->UpdateImpl(); }
		inline static void OverrideMouseInputs(float mx, float my, float px, float py, float dx, float dy) 
		{
			MouseX = mx;
			MouseY = my;
			prevMouseX = px;
			prevMouseY = py;
			MouseXDelta = dx;
			MouseYDelta = dy;
		}
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual bool IsMouseButtonReleasedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual float GetMouseXDeltaImpl() = 0;
		virtual float GetMouseYDeltaImpl() = 0;
		virtual void UpdateImpl() = 0;
		virtual void LockCursorImpl() = 0;
		virtual void UnlockCursorImpl() = 0;
		virtual float GetAxisImpl(int gamepadIndx, int axis, float deadzone) = 0;
		virtual float GetRawAxisImpl(int gamepadIndx, int axis, float deadzone) = 0;
		virtual bool IsButtonPressedImpl(int gamepadIndx, int keycode) = 0;
	private:
		static Input* s_Instance;
	protected:
		static float MouseXDelta;
		static float MouseYDelta;

		static float MouseX;
		static float MouseY;

		static float prevMouseX;
		static float prevMouseY;
	};
}
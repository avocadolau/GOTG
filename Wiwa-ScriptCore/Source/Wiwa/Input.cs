namespace Wiwa
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keyCode)
        {
            return InternalCalls.IsKeyDownIntr(keyCode);
        }
        public static bool IsMouseKeyDown(int button)
        {
            return InternalCalls.IsMouseButtonPressedIntr(button);
        }
        public static float GetMouseX()
        {
            return InternalCalls.GetMouseXIntr();
        }
        public static float GetMouseY()
        {
            return InternalCalls.GetMouseYIntr();
        }
        public static float GetMouseXDelta()
        {
            return InternalCalls.GetMouseXDeltaIntr();
        }
        public static float GetMouseYDelta()
        {
            return InternalCalls.GetMouseYDeltaIntr();
        }
        public static bool IsButtonPressed(Gamepad gamepad, KeyCode button)
        {
            return InternalCalls.IsButtonPressedIntr(gamepad, button);
        }
        public static float GetAxis(Gamepad gamepad, GamepadAxis axis, float deadzone = 0.6f)
        {
            return InternalCalls.GetAxisIntr(gamepad, axis, deadzone);
        }
        public static float GetRawAxis(Gamepad gamepad, GamepadAxis axis, float deadzone)
        {
            return InternalCalls.GetRawAxisIntr(gamepad, axis, deadzone);
        }
    }
}

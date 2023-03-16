namespace Wiwa
{
    public static class Screen
    {
        public static bool fullscreen { get; private set; }
        public static bool resizable { get; private set; }
        public static bool vsync { get; private set; }
        public static int width { get; private set; }
        public static int height { get; private set; }

        static Screen()
        {
            fullscreen = InternalCalls.GetFullscreenIntr();
            resizable = InternalCalls.GetResizableIntr();
            vsync = InternalCalls.GetVsyncIntr();
            width = InternalCalls.GetWidthIntr();
            height = InternalCalls.GetHeightIntr();
        }

        public static void SetFullscreen(bool value)
        {
            fullscreen = value;
            InternalCalls.SetFullscreenIntr(value);
        }

        public static void SetResolution(int _width, int _height)
        {
            width = _width;
            height = _height;
            //TODO Implement in C++
            InternalCalls.SetResolutionIntr(_width, _height);
        }
        public static void SetResizable(bool value)
        {
            resizable = value;
            InternalCalls.SetResizableIntr(value);
        }

        public static void SetVsync(bool value)
        {
            vsync = value;
            InternalCalls.SetVsyncIntr(value);
        }
    }
}

namespace Wiwa
{
    public class GUI
    {
        public static void ActivateGuiCanvas(int id)
        {
            InternalCalls.ActivateGuiCanvas(id);
        }
        public static void SwapToNewTexture(string path,int id_canvas,int id_control)
        {
            InternalCalls.SwapToNewTexture(path,id_canvas,id_control);
        }
        public static void SwapText(string word, int id_canvas, int id_control)
        {
            InternalCalls.SwapText(word, id_canvas, id_control);
        }
        public static void SetValeForUIbar(float valueHealth,float valueMaxHealth, int id_canvas, int id_control)
        {
            InternalCalls.SetValueForUIbar(valueHealth,valueMaxHealth, id_canvas, id_control);
        }

        public static void SetNextFrame(int value, int id_canvas, int id_control)
        {
            InternalCalls.SetNextFrame(value, id_canvas, id_control);
        }
    }
}

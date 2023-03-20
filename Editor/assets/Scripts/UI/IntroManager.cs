using Wiwa;
namespace Game
{
    [Component]
    public struct IntroData
    {
        public float timeToWait;
    }
    public class IntroManager : Behaviour
    {
        float time = 0;

        ComponentIterator introData;
        //Called the first frame
        void Awake()
        {
            introData = GetComponentIterator<IntroData>();
        }
        //Called after Awake()
        void Init()
        {
        }
        //Called every frame
        void Update()
        {
            IntroData data = GetComponentByIterator<IntroData>(introData);
            time += Time.DeltaTime();

            if (time >= data.timeToWait
                || Input.IsKeyDown(KeyCode.Space)
                || Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.A))
            {
                SceneManager.LoadScene(1);
            }
        }
    }
}

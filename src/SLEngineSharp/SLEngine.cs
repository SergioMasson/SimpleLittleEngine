namespace SLEngine
{
    public static class SLEngine
    {
        public static void RunApplication<T>() where T : IGameApp, new()
        {
            var game = new T();
            GameAppInterop.StartGame(game);
        }
    }
}
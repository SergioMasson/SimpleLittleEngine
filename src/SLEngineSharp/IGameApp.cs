namespace SLEngine
{
    /// <summary>
    /// Base interface for a game application.
    /// </summary>
    public interface IGameApp
    {
        void Startup();

        void Cleanup();

        // Decide if you want the app to exit.  By default, app continues until the 'ESC' key is pressed.
        bool IsDone();

        // The update method will be invoked once per frame AFTER the physics run. Both state updating and scene
        // rendering should be handled by this method.
        void Update(float deltaT, float totalTime);

        // The update method will be invoked once per frame BEFORE all physics run. Both state updating and scene
        // rendering should be handled by this method.
        void FixedUpdate(float deltaT, float totalTime);

        void Resize(int width, int height);
    }
}
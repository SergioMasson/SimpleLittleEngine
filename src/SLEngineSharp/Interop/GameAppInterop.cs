using System;
using System.Runtime.InteropServices;

namespace SLEngine
{
    internal static class GameAppInterop
    {
        private static IGameApp _currentGame;

        private static IntPtr _nativeGameInstance;

        public static void StartGame(IGameApp game)
        {
            _nativeGameInstance = Instantiate();
            _currentGame = game;

            SubscribeStartup(_nativeGameInstance, startupCallback);
            SubscribeCleanup(_nativeGameInstance, cleanupCallback);
            SubscribeUpdate(_nativeGameInstance, updateCallback);
            SubscribeFixedUpdate(_nativeGameInstance, fixedUpdateCallback);
            SubscribeIsDone(_nativeGameInstance, isDoneCallback);
            SubscribeResize(_nativeGameInstance, resizeCallback);

            Run(_nativeGameInstance);
        }

        #region delegates

        private delegate void StartupDelegate();

        private delegate void CleanupDelegate();

        private delegate bool IsDoneDelegate();

        private delegate void UpdateDelegate(float deltaT, float totalTime);

        private delegate void FixedUpdateDelegate(float deltaT, float totalTime);

        private delegate void ResizeDelegate(int width, int height);

        #endregion delegates

        #region delegate instances

        private static StartupDelegate startupCallback = StartupCallback;

        private static CleanupDelegate cleanupCallback = CleanupCallback;

        private static IsDoneDelegate isDoneCallback = IsDoneCallback;

        private static UpdateDelegate updateCallback = UpdateCallback;

        private static FixedUpdateDelegate fixedUpdateCallback = FixedUpdateCallback;

        private static ResizeDelegate resizeCallback = ResizeCallback;

        #endregion delegate instances

        #region callbacks

        private static void StartupCallback()
        {
            if (_currentGame != null)
                _currentGame.Startup();
        }

        private static void CleanupCallback()
        {
            if (_currentGame != null)
                _currentGame.Cleanup();
        }

        private static bool IsDoneCallback()
        {
            if (_currentGame != null)
                return _currentGame.IsDone();

            return true;
        }

        private static void UpdateCallback(float deltaT, float totalTime)
        {
            if (_currentGame != null)
                _currentGame.Update(deltaT, totalTime);
        }

        private static void FixedUpdateCallback(float deltaT, float totalTime)
        {
            if (_currentGame != null)
                _currentGame.FixedUpdate(deltaT, totalTime);
        }

        private static void ResizeCallback(int width, int height)
        {
            if (_currentGame != null)
                _currentGame.Resize(width, height);
        }

        #endregion callbacks

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr Instantiate();

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr Run(IntPtr handle);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr SubscribeStartup(IntPtr handle, StartupDelegate callback);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr SubscribeCleanup(IntPtr handle, CleanupDelegate callback);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr SubscribeIsDone(IntPtr handle, IsDoneDelegate callback);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr SubscribeUpdate(IntPtr handle, UpdateDelegate callback);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr SubscribeFixedUpdate(IntPtr handle, FixedUpdateDelegate callback);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern IntPtr SubscribeResize(IntPtr handle, ResizeDelegate callback);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        private static extern void Destroy(IntPtr instance);
    }
}
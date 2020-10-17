using System.Runtime.InteropServices;

namespace SLEngine
{
    internal static class InputInterop
    {
        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern bool IsAnyPressed();

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern bool TryGetLastKeyboardType(out char key);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern bool IsPressed(KeyCode di);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern bool IsFirstPressed(KeyCode di);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern bool IsReleased(KeyCode di);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern bool IsFirstReleased(KeyCode di);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern float GetDurationPressed(KeyCode di);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern float GetAnalogInput(AnalogInput ai);

        [DllImport(NativeUtils.dllPath, CallingConvention = NativeUtils.EngineCallingConvention, CharSet = CharSet.Ansi, EntryPoint = "sleGameAppInstantiate")]
        public static extern float GetTimeCorrectedAnalogInput(AnalogInput ai);
    }
}
using System.Runtime.InteropServices;

namespace SLEngine
{
    internal static class NativeUtils
    {
        public const string dllPath = "SLEngine";

        public const CallingConvention EngineCallingConvention = CallingConvention.StdCall;
    }
}
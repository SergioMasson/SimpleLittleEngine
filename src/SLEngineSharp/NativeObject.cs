using System;

namespace SLEngine
{
    public abstract class NativeObject : IDisposable
    {
        protected IntPtr _nativePtr;

        public abstract void Dispose();
    }
}
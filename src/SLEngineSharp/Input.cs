using System;
using System.Collections.Generic;
using System.Text;

namespace SLEngine
{
    public static class Input
    {
        public static bool IsAnyPressed()
        {
            return InputInterop.IsAnyPressed();
        }

        public static bool TryGetLastKeyboardType(out char key)
        {
            return InputInterop.TryGetLastKeyboardType(out key);
        }

        public static bool IsPressed(KeyCode di)
        {
            return InputInterop.IsPressed(di);
        }

        public static bool IsFirstPressed(KeyCode di)
        {
            return InputInterop.IsFirstPressed(di);
        }

        public static bool IsReleased(KeyCode di)
        {
            return InputInterop.IsReleased(di);
        }

        public static bool IsFirstReleased(KeyCode di)
        {
            return InputInterop.IsFirstReleased(di);
        }

        public static float GetDurationPressed(KeyCode di)
        {
            return InputInterop.GetDurationPressed(di);
        }

        public static float GetAnalogInput(AnalogInput ai)
        {
            return InputInterop.GetAnalogInput(ai);
        }

        public static float GetTimeCorrectedAnalogInput(AnalogInput ai)
        {
            return InputInterop.GetTimeCorrectedAnalogInput(ai);
        }
    }
}
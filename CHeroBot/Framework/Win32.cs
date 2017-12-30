using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace CHeroBot.Framework
{
    internal static class Win32
    {
        private static IntPtr m_Module;
        private static TogglePauseDelegate m_TogglePause;
        private static StartBotDelegate m_StartBot;
        private static StopBotDelegate m_StopBot;
        private static SetUpgradeIntervalDelegate m_SetUpgradeInterval;
        private static ToggleLevelsDelegate m_ToggleLevels;
        private static ToggleUpgradesDelegate m_ToggleUpgrades;
        private static SetLevelDelayDelegate m_SetLevelDelay;
        private static SetOldBossDelayDelegate m_SetOldBossDelay;
        private static SetRollbackCountDelegate m_SetRollbackCount;
        private static bool m_Loaded;
        private static bool m_Installed;

        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(string dllToLoad);

        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll")]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll")]
        private static extern bool FreeLibrary(IntPtr hModule);

        [SuppressUnmanagedCodeSecurity]
        [DllImport("user32.dll", SetLastError = true, EntryPoint = "FindWindowA")]
        public static extern IntPtr FindWindow(string className, string windowTitle);

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate bool StartBotDelegate(IntPtr botWindow, IntPtr gameWindow);

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate bool StopBotDelegate();

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate bool TogglePauseDelegate();

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void SetUpgradeIntervalDelegate(UInt32 interval);

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate bool ToggleLevelsDelegate();

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate bool ToggleUpgradesDelegate();

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void SetLevelDelayDelegate(UInt32 delay);

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void SetOldBossDelayDelegate(UInt32 delay);

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void SetRollbackCountDelegate(UInt32 levelCount);

        public static bool TogglePause()
        {
            if (Win32.m_TogglePause == null)
                return false;

            return Win32.m_TogglePause();
        }

        public static bool StartBot(IntPtr botWindow, IntPtr gameWindow)
        {
            if (Win32.m_StartBot == null)
                return false;

            if (Win32.m_Installed)
                return true;

            Win32.m_Installed = Win32.m_StartBot(botWindow, gameWindow);
            return Win32.m_Installed;
        }

        public static bool StopBot()
        {
            if (Win32.m_StopBot == null || !Win32.m_Installed)
                return true;

            Win32.m_Installed = false;

            return Win32.m_StopBot();
        }

        public static void SetRollbackCount(UInt32 levelCount)
        {
            if (Win32.m_SetRollbackCount == null || !Win32.m_Installed)
                return;

            Win32.m_SetRollbackCount(levelCount);
        }

        public static void SetOldBossDelay(UInt32 delay)
        {
            if (Win32.m_SetOldBossDelay == null || !Win32.m_Installed)
                return;

            Win32.m_SetOldBossDelay(delay);
        }

        public static void SetLevelDelay(UInt32 delay)
        {
            if (Win32.m_SetLevelDelay == null || !Win32.m_Installed)
                return;

            Win32.m_SetLevelDelay(delay);
        }

        public static bool ToggleUpgrades()
        {
            if (Win32.m_ToggleUpgrades == null || !Win32.m_Installed)
                return true;

            return Win32.m_ToggleUpgrades();
        }

        public static bool ToggleLevels()
        {
            if (Win32.m_ToggleLevels == null || !Win32.m_Installed)
                return true;

            return Win32.m_ToggleLevels();
        }

        public static void SetUpgradeInterval(UInt32 interval)
        {
            if (Win32.m_SetUpgradeInterval == null || !Win32.m_Installed)
                return;

            Win32.m_SetUpgradeInterval(interval);
        }

        public static bool LoadBotLibrary()
        {
            Win32.m_Module = Win32.LoadLibrary("clickerbot.dll");
            if (Win32.m_Module == IntPtr.Zero)
                return false;

            var func1 = Win32.GetProcAddress(Win32.m_Module, "StartBot");
            var func2 = Win32.GetProcAddress(Win32.m_Module, "StopBot");
            var func3 = Win32.GetProcAddress(Win32.m_Module, "TogglePause");
            var func4 = Win32.GetProcAddress(Win32.m_Module, "SetUpgradeInterval");
            var func5 = Win32.GetProcAddress(Win32.m_Module, "ToggleLevels");
            var func6 = Win32.GetProcAddress(Win32.m_Module, "ToggleUpgrades");
            var func7 = Win32.GetProcAddress(Win32.m_Module, "SetLevelDelay");
            var func8 = Win32.GetProcAddress(Win32.m_Module, "SetOldBossDelay");
            var func9 = Win32.GetProcAddress(Win32.m_Module, "SetRollbackCount");

            if (func1 == IntPtr.Zero || func2 == IntPtr.Zero || func3 == IntPtr.Zero || func4 == IntPtr.Zero ||
                func5 == IntPtr.Zero || func6 == IntPtr.Zero || func7 == IntPtr.Zero || func8 == IntPtr.Zero || func9 == IntPtr.Zero)
            {
                Win32.FreeLibrary(Win32.m_Module);
                Win32.m_Loaded = false;
            }
            else
            {
                Win32.m_StartBot = (Win32.StartBotDelegate)Marshal.GetDelegateForFunctionPointer(func1, typeof(Win32.StartBotDelegate));
                Win32.m_StopBot = (Win32.StopBotDelegate)Marshal.GetDelegateForFunctionPointer(func2, typeof(Win32.StopBotDelegate));
                Win32.m_TogglePause = (Win32.TogglePauseDelegate)Marshal.GetDelegateForFunctionPointer(func3, typeof(Win32.TogglePauseDelegate));
                Win32.m_SetUpgradeInterval = (Win32.SetUpgradeIntervalDelegate)Marshal.GetDelegateForFunctionPointer(func4, typeof(Win32.SetUpgradeIntervalDelegate));
                Win32.m_ToggleLevels = (Win32.ToggleLevelsDelegate)Marshal.GetDelegateForFunctionPointer(func5, typeof(Win32.ToggleLevelsDelegate));
                Win32.m_ToggleUpgrades = (Win32.ToggleUpgradesDelegate)Marshal.GetDelegateForFunctionPointer(func6, typeof(Win32.ToggleUpgradesDelegate));
                Win32.m_SetLevelDelay = (Win32.SetLevelDelayDelegate)Marshal.GetDelegateForFunctionPointer(func7, typeof(Win32.SetLevelDelayDelegate));
                Win32.m_SetOldBossDelay = (Win32.SetOldBossDelayDelegate)Marshal.GetDelegateForFunctionPointer(func8, typeof(Win32.SetOldBossDelayDelegate));
                Win32.m_SetRollbackCount = (Win32.SetRollbackCountDelegate)Marshal.GetDelegateForFunctionPointer(func9, typeof(Win32.SetRollbackCountDelegate));
            }

            return Win32.m_Loaded;
        }

        public static void FreeBotLibrary()
        {
            if (!Win32.m_Loaded)
                return;

            Win32.FreeLibrary(Win32.m_Module);

            Win32.m_StartBot = null;
            Win32.m_StopBot = null;
            Win32.m_TogglePause = null;
            Win32.m_SetUpgradeInterval = null;
            Win32.m_ToggleLevels = null;
            Win32.m_ToggleUpgrades = null;
            Win32.m_SetLevelDelay = null;
            Win32.m_SetOldBossDelay = null;
            Win32.m_SetRollbackCount = null;

            Win32.m_Loaded = false;
        }
    }
}

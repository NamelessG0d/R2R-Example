using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace R2R_Example.Lib
{
    public class UnamangedExports
    {
        public enum DllReason : uint {
            DllProcessDetach = 0,
            DllProcessAttach = 1,
            DllThreadAttach = 2,
            DllThreadDetach = 3 
        }

        [UnmanagedCallersOnly(EntryPoint = "DllMain")]
        public static bool DllMain(nint module, DllReason reason, nint reserved)
        {
            switch (reason)
            {
                case DllReason.DllProcessAttach:
                    Console.WriteLine($"Managed DllMain called with reason {reason}");
                    break;
                case DllReason.DllThreadAttach: break;
                case DllReason.DllThreadDetach: break;

                //Warning, the CLR will be unloaded already so it's unhandled behaviour!
                case DllReason.DllProcessDetach: break;
            }
            return true;
        }

        [UnmanagedCallersOnly(EntryPoint = "WriteLineA")]
        public static void UnmanagedWriteLineA(IntPtr text)
            => Console.WriteLine(Marshal.PtrToStringAnsi(text));

        [UnmanagedCallersOnly(EntryPoint = "WriteLineW")]
        public static void UnmanagedWriteLineW(IntPtr text)
            => Console.WriteLine(Marshal.PtrToStringUni(text));

        [UnmanagedCallersOnly(EntryPoint = "SetForegroundColor")]
        public static void UnmanagedSetConsoleForeground(int color)
            => Console.ForegroundColor = (ConsoleColor)color;

        [UnmanagedCallersOnly(EntryPoint = "SetBackgroundColor")]
        public static void UnmanagedSetConsoleBackground(int color)
            => Console.BackgroundColor = (ConsoleColor)color;

        [UnmanagedCallersOnly(EntryPoint = "ClearConsole")]
        public static void UnmanagedClearConsole()
            => Console.Clear();

        [UnmanagedCallersOnly(EntryPoint = "ReadLineW")]
        public static void UnmanagedReadLineW(IntPtr buffer, ulong bufferSize)
        {
            try
            {
                string? input = Console.ReadLine();

                if (input != null)
                {
                    byte[] unicodeBytes = Encoding.Unicode.GetBytes(input);
                    int bytesToCopy = Math.Min((int)unicodeBytes.Length, (int)(bufferSize - sizeof(char)));
                    Marshal.Copy(unicodeBytes, 0, buffer, bytesToCopy);
                    Marshal.WriteInt16(buffer, bytesToCopy, 0);
                }
            }
            catch
            {
                if (buffer != IntPtr.Zero)
                {
                    Marshal.WriteInt16(buffer, 0, 0);
                }
            }
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public delegate void Callback(string message);
        public static Callback TestCallback = default!;

        [UnmanagedCallersOnly(EntryPoint = "SetMessageCallbackA")]
        public unsafe static void UnmanagedSetCallback(delegate* unmanaged[Cdecl]<string, void> callback)
        {
            TestCallback = Marshal.GetDelegateForFunctionPointer<Callback>((IntPtr)callback);
        }

        [UnmanagedCallersOnly(EntryPoint = "CallMessageCallback")]
        public unsafe static void UnmanagedCallCallback()
        {
            TestCallback("Hello from Managed code!");
        }
    }
}

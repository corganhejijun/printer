using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ManagedExchange
{
    public class ClassDisplay
    {
        [DllImport("printerDisplay.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr glInit(IntPtr hWnd);

        [DllImport("printerDisplay.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static int paint(IntPtr hWnd, IntPtr param);

        [DllImport("printerDisplay.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static int resize(IntPtr hWnd, IntPtr param);

        [DllImport("printerDisplay.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static int glDispose(IntPtr param);

        [DllImport("printerDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int displayDxf(IntPtr param, string fileName);

        [DllImport("printerDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int drawEntity(IntPtr hWnd, IntPtr param);

        [DllImport("printerDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int slowDrawEntity(IntPtr hWnd, IntPtr param, int sleepMillisecond);
    }
}

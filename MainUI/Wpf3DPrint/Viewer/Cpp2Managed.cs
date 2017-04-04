using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Wpf3DPrint.Viewer
{
    class Cpp2Managed
    {
        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ImportStep(IntPtr theFileName, ref int cnt, IntPtr shapes, bool isSlice, IntPtr slice);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool deleteShape(IntPtr shapes, int cnt);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void getShapeBoundary(IntPtr pt, int index, ref double Zmin, ref double Zmax, ref double Ymin, ref double Ymax, ref double Xmin, ref double Xmax);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr SliceShape(IntPtr pt, int index, double Zmax, double Zmin, double height);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteSlice(IntPtr pt);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getSliceFromShape(IntPtr pt, int index);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool exportStep(IntPtr fileName, IntPtr[] slices, int length);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr create2D(IntPtr hWnd);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int displaySlice(IntPtr device2D, IntPtr slice, int sliceNum);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void cleanScreen(IntPtr device2D);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void resizeWindow(IntPtr device2D);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void delete2DSlice(IntPtr slice, int count);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void release2D(IntPtr device2D);

        [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void reset2D(IntPtr device);
    }
}

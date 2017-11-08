using System;
using System.Runtime.InteropServices;

namespace Wpf3DPrint.Viewer
{
    class Cpp2Managed
    {
        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ImportStep(IntPtr theFileName, ref int cnt, IntPtr shapes, bool isSlice, IntPtr slice);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ImportStl(IntPtr theFileName, IntPtr shapes);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool deleteShape(IntPtr shapes, int cnt);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void getShapeBoundary(IntPtr pt, int index, ref double Zmin, ref double Zmax, ref double Ymin, ref double Ymax, ref double Xmin, ref double Xmax);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr SliceShape(IntPtr pt, int index, double height);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void combineShapes(IntPtr pt, IntPtr pt1, IntPtr pt2);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getLocatPlane(IntPtr pt, int index, ref int count);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getShapeContainer(IntPtr pt, int index, ref double height);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteSlice(IntPtr pt);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getSliceFromShape(IntPtr pt, int index, ref double height);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool exportStep(IntPtr fileName, IntPtr[] slices, int length);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool exportTransformStep(IntPtr fileName, IntPtr slices, int length);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr exportSlice(IntPtr ss, int layerNum, IntPtr current, ref int type, ref double x, ref double y, ref double z, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)]double[] p);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool exportBspline(IntPtr current, int index, ref double x, ref double y);


        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool rotateShape(IntPtr shape, IntPtr rotateResult, int count, double x, double y, double z);

        [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool moveShape(IntPtr shape, IntPtr moveResult, int count, double x, double y, double z);

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

using System;
using System.Runtime.InteropServices;

namespace Wpf3DPrint.Viewer
{
    public class Cpp2Managed
    {
        public enum EdgeType { unknown, line, circle, bSplice };
        [StructLayout(LayoutKind.Sequential)]
        public struct Slice
        {
            public EdgeType type;
            public IntPtr data;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct Point
        {
            public double x;
            public double y;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct Line
        {
            public Point start;
            public Point end;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct Circle
        {
            public Point center;
            public double startAngle;
            public double endAngle;
            public double radius;
            public Point start;
            public Point end;
        }
        public struct BSpline
        {
            public Point start;
            public Point end;
            public Point[] poles;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct BSpline4Dll
        {
            public Point start;
            public Point end;
            public int polesCnt;
            public IntPtr polesPt;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct BoundBox
        {
            public double left;
            public double right;
            public double top;
            public double bottom;
        };

        public class Shape3D
        {
            public delegate void OnGetShape(IntPtr shape);
            public delegate void OnGetEdge(IntPtr shape, EdgeType type, [MarshalAs(UnmanagedType.LPArray, SizeConst = 256)]double[] data, int length);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr ImportStl(IntPtr theFileName);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern bool ImportStep(IntPtr theFileName, OnGetShape getShape);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern bool ImportSlice(IntPtr theFileName, OnGetEdge getEdge);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void del(IntPtr shape);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void getBoundary(IntPtr shape, ref double Zmin, ref double Zmax, ref double Ymin, ref double Ymax, ref double Xmin, ref double Xmax);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr rotate(IntPtr shape, double x, double y, double z);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr move(IntPtr shape, double x, double y, double z);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr scale(IntPtr shape, double ratio, double centerX, double centerY, double centerZ);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr slice(IntPtr shape, double height);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr combine(IntPtr shape1, IntPtr shape2);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void getLocatPlane(IntPtr shape, OnGetShape getShape);

            [DllImport("OCCTProxy_D3D.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern bool exportStep(IntPtr fileName, IntPtr[] shapeList, int length);
        }

        public class Slice2D
        {
            public delegate IntPtr OnGetSliceData(int index);

            [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern IntPtr create(IntPtr hWnd);

            [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern int displaySlice(IntPtr device2D, BoundBox box, int sliceNum, OnGetSliceData getSlice);

            [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void cleanScreen(IntPtr device2D);

            [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void resizeWindow(IntPtr device2D);

            [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void release(IntPtr device2D);

            [DllImport("SliceDisplay.dll", CallingConvention = CallingConvention.Cdecl)]
            public static extern void reset(IntPtr device);
        }
    }
}

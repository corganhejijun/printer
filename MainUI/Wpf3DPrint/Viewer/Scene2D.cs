using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Wpf3DPrint.Viewer
{
    class Scene2D : IDisposable
    {
        IntPtr device;
        public Scene2D(Panel panel)
        {
            _slice = null;
            device = Cpp2Managed.Slice2D.create(panel.Handle);
            if (device == IntPtr.Zero)
            {
                MessageBox.Show("初始化二维切片显示界面失败");
                return;
            }
            deleGetSliceData = new Cpp2Managed.Slice2D.OnGetSliceData(onGetSliceData);
            toDelList = new ArrayList();
        }

        public void resetView(Cpp2Managed.BoundBox box)
        {
            if (device == IntPtr.Zero)
                return;
            Cpp2Managed.Slice2D.move(device, (float)(box.right - box.left) / 2, (float)(box.bottom - box.top) / 2);
            Cpp2Managed.Slice2D.fitScreen(device, (float)(box.right - box.left), (float)(box.bottom - box.top));
        }

        public void onResize(Cpp2Managed.BoundBox box)
        {
            if (device == IntPtr.Zero)
                return;
            Cpp2Managed.Slice2D.fitScreen(device, (float)(box.right - box.left), (float)(box.bottom - box.top));
            Cpp2Managed.Slice2D.resizeWindow(device);
        }

        public void clearWindow()
        {
            if (device == IntPtr.Zero)
                return;
            Cpp2Managed.Slice2D.cleanScreen(device);
        }

        Cpp2Managed.Slice2D.OnGetSliceData deleGetSliceData;
        Slice.OneSlice _slice;
        public void drawSlice(Slice.OneSlice slice)
        {
            if (device == IntPtr.Zero)
                return;
            _slice = slice;
            Cpp2Managed.Slice2D.displaySlice(device, slice.data.Count, deleGetSliceData);
            foreach (IntPtr s in toDelList)
            {
                Marshal.FreeHGlobal(s);
            }
            toDelList.Clear();
        }

        ArrayList toDelList;
        IntPtr onGetSliceData(int index)
        {
            if (device == IntPtr.Zero)
                return IntPtr.Zero;
            IntPtr slicePt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Slice)));
            object obj = _slice.data[index];
            Cpp2Managed.Slice slice = new Cpp2Managed.Slice();
            Cpp2Managed.BSpline4Dll b4 = new Cpp2Managed.BSpline4Dll();
            if (obj is Cpp2Managed.BSpline)
            {
                Cpp2Managed.BSpline b = (Cpp2Managed.BSpline)obj;
                b4.start.x = b.start.x;
                b4.start.y = b.start.y;
                b4.end.x = b.end.x;
                b4.end.y = b.end.y;
                b4.polesCnt = b.poles.Length;
                b4.polesPt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Point))*b4.polesCnt);
                for (int i = 0; i < b.poles.Length; i++)
                {
                    IntPtr pt = new IntPtr(b4.polesPt.ToInt64() + Marshal.SizeOf(typeof(Cpp2Managed.Point)) * i);
                    Marshal.StructureToPtr(b.poles[i], pt, false);
                }
                toDelList.Add(b4.polesPt);
                slice.type = Cpp2Managed.EdgeType.bSplice;
                slice.data = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.BSpline)));
                Marshal.StructureToPtr(b4, slice.data, false);
            }
            else if (obj is Cpp2Managed.Line)
            {
                Cpp2Managed.Line line = (Cpp2Managed.Line)obj;
                slice.type = Cpp2Managed.EdgeType.line;
                slice.data = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Line)));
                Marshal.StructureToPtr(line, slice.data, false);
            }
            else if (obj is Cpp2Managed.Circle)
            {
                Cpp2Managed.Circle circle = (Cpp2Managed.Circle)obj;
                slice.type = Cpp2Managed.EdgeType.circle;
                slice.data = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Circle)));
                Marshal.StructureToPtr(circle, slice.data, false);
            }
            Marshal.StructureToPtr(slice, slicePt, false);
            toDelList.Add(slicePt);
            toDelList.Add(slice.data);
            return slicePt;
        }

        public void Dispose()
        {
            if (device == IntPtr.Zero)
                return;
            closeSlice();
            Cpp2Managed.Slice2D.release(device);
        }

        public void closeSlice()
        {
            if (device == IntPtr.Zero)
                return;
            Cpp2Managed.Slice2D.reset(device);
        }
    }
}

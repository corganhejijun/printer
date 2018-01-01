using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Windows.Media;

namespace Wpf3DPrint.Viewer
{
    class Scene2D : IDisposable
    {
        IntPtr device;
        IntPtr m_slice;
        int sliceCount;
        public Scene2D(Panel panel)
        {
            device = Cpp2Managed.Slice2D.create(panel.Handle);
        }

        public void onResize()
        {
            Cpp2Managed.Slice2D.resizeWindow(device);
        }

        public void clearWindow()
        {
            Cpp2Managed.Slice2D.cleanScreen(device);
        }

        public void slice(IntPtr slice, int sliceCnt)
        {
            m_slice = slice;
            sliceCount = sliceCnt;
        }

        public void drawSlice(int number)
        {
            Cpp2Managed.Slice2D.displaySlice(device, m_slice, number);
        }

        public void Dispose()
        {
            closeSlice();
            Cpp2Managed.Slice2D.release(device);
        }

        public void closeSlice()
        {
            //Cpp2Managed.Slice2D.delete2DSlice(m_slice, sliceCount);
            Marshal.FreeHGlobal(m_slice);
            m_slice = IntPtr.Zero;
            Cpp2Managed.Slice2D.reset(device);
        }
    }
}

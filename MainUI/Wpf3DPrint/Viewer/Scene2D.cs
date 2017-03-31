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
            device = Cpp2Managed.create2D(panel.Handle);
        }

        public void onResize()
        {
            Cpp2Managed.resizeWindow(device);
        }

        public void clearWindow()
        {
            Cpp2Managed.cleanScreen(device);
        }

        public void slice(IntPtr slice, int sliceCnt)
        {
            m_slice = slice;
            sliceCount = sliceCnt;
        }

        public void drawSlice(int number)
        {
            Cpp2Managed.displaySlice(device, m_slice, number);
        }

        public void Dispose()
        {
            Cpp2Managed.delete2DSlice(m_slice, sliceCount);
            Marshal.FreeHGlobal(m_slice);
            Cpp2Managed.release2D(device);
        }
    }
}

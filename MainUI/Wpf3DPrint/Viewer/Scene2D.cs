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

        public void drawSlice(Slice.OneSlice slice)
        {
            //Cpp2Managed.Slice2D.displaySlice(device, type, sliceData, slice.data.Count);
        }

        public void Dispose()
        {
            closeSlice();
            Cpp2Managed.Slice2D.release(device);
        }

        public void closeSlice()
        {
            Cpp2Managed.Slice2D.reset(device);
        }
    }
}

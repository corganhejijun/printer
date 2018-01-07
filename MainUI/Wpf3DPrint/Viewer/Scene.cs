using System;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;

namespace Wpf3DPrint.Viewer
{
    class Scene
    {
        IntPtr d3DRender;
        D3DImage d3DImage = new D3DImage();
        IntPtr d3DColorSurface;
        bool deviceInitFail = false;
        OCCTProxyD3D occtProxy;
        Setting setting;

        public D3DImage Image
        {
            get { return d3DImage; }
        }

        public OCCTProxyD3D Proxy
        {
            get { return occtProxy; }
        }

        public Setting Setting
        {
            get { return setting; }
        }

        public Scene()
        {
            setting = new Setting();
            d3DImage.IsFrontBufferAvailableChanged += new DependencyPropertyChangedEventHandler(onFrontBufferChange);
            occtProxy = new OCCTProxyD3D();
            occtProxy.InitOCCTProxy();
            beginRender();
            Resize(1, 1);
        }

        private void onFrontBufferChange(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (d3DImage.IsFrontBufferAvailable)
            {
                beginRender();
            }
            else
            {
                stopRender();
            }
        }

        private void beginRender()
        {
            if (deviceInitFail || !d3DImage.IsFrontBufferAvailable)
                return;
            IntPtr hWnd;
            IntPtr d3DDevice;
            d3DRender = Direct3DProxy.InitRender(out hWnd, out d3DDevice);
            if (!occtProxy.InitViewer(hWnd, d3DDevice))
            {
                MessageBox.Show("Device init failed!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                deviceInitFail = true;
                if (d3DRender != IntPtr.Zero)
                    Direct3DProxy.ReleaseRender(ref d3DRender);
                return;
            }
            CompositionTarget.Rendering += onRender;
        }

        private void onRender(object sender, EventArgs e)
        {
            updateScene();
        }

        private void stopRender()
        {
            CompositionTarget.Rendering -= onRender;
            if (d3DRender != IntPtr.Zero)
                Direct3DProxy.ReleaseRender(ref d3DRender);
            d3DColorSurface = IntPtr.Zero;
        }

        private WndSize proxyWndSize = new WndSize(1, 1);

        private void updateScene()
        {
            if (deviceInitFail || !d3DImage.IsFrontBufferAvailable || d3DColorSurface == IntPtr.Zero)
                return;
            occtProxy.SetBackgroundColor(255, 255, 255);
            d3DImage.Lock();
            {
                occtProxy.RedrawView();
                d3DImage.AddDirtyRect(new Int32Rect(0, 0, proxyWndSize.cx, proxyWndSize.cy));
            }
            d3DImage.Unlock();
        }

        public void Resize(int width, int height)
        {
            if (deviceInitFail || !d3DImage.IsFrontBufferAvailable)
                return;
            proxyWndSize = new WndSize(width, height);
            IntPtr colorSurf;
            Direct3DProxy.ResizeWindow(ref d3DRender, ref proxyWndSize, out d3DColorSurface, out colorSurf);
            d3DImage.Lock();
            {
                d3DImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, d3DColorSurface);
            }
            d3DImage.Unlock();
            occtProxy.ResizeBridgeFBO(proxyWndSize.cx, proxyWndSize.cy, d3DColorSurface, colorSurf);
        }

        public bool displayShape(IntPtr shape)
        {
            if (shape == IntPtr.Zero)
                return false;
            occtProxy.SetDisplayMode(1);
            occtProxy.displayShape(shape, 0, setting.entityColor.R, setting.entityColor.G, setting.entityColor.B);
            return true;
        }

        public bool displaySelectShape(IntPtr shape)
        {
            occtProxy.SetDisplayMode(1);
            occtProxy.displayShape(shape, 0, setting.selectEntityColor.R, setting.selectEntityColor.G, setting.selectEntityColor.B);
            return true;
        }

        public void zoom(int delta)
        {
            occtProxy.Zoom(0, 0, delta / 8, 0);
        }

        public void displaySlice(IntPtr slice)
        {
            occtProxy.displaySlice(slice, setting.lineColor.R, setting.lineColor.G, setting.lineColor.B);
        }

        public void displaySliceCut(IntPtr shape, double height)
        {
            occtProxy.displaySliceCut(shape, height, setting.entityColor.R, setting.entityColor.G, setting.entityColor.B);
        }

        public void selectSlice(IntPtr slice)
        {
            occtProxy.selectSlice(slice);
        }

        public void displayAfterTransform(IntPtr shape)
        {
            occtProxy.cleanScene();
            occtProxy.displayShape(shape, 0, setting.entityColor.R, setting.entityColor.G, setting.entityColor.B);
            //occtProxy.ZoomAllView();
        }

        public IntPtr select(double x, double y)
        {
            return occtProxy.Select((int)x, (int)y);
        }
    }
}

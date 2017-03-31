using System;
using System.Collections;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;

namespace Wpf3DPrint.Viewer
{
    class Scene : IDisposable
    {
        private IntPtr d3DRender;
        private D3DImage d3DImage = new D3DImage();
        private IntPtr d3DColorSurface;
        private bool deviceInitFail = false;
        private OCCTProxyD3D occtProxy;
        private SceneThread sceneThread;

        public D3DImage Image
        {
            get { return d3DImage; }
        }

        public OCCTProxyD3D Proxy
        {
            get { return occtProxy; }
        }

        public SceneThread D3DThread
        {
            get { return sceneThread; }
        }

        public Scene()
        {
            sceneThread = new SceneThread();
            d3DImage.IsFrontBufferAvailableChanged += new DependencyPropertyChangedEventHandler(onFrontBufferChange);
            occtProxy = new OCCTProxyD3D();
            occtProxy.InitOCCTProxy();
            beginRender();
            Resize(1, 1);
        }

        public void Dispose()
        {
            sceneThread.stop();
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
    }
}

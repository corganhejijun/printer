using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Controls;

namespace Wpf3DPrint.Viewer
{
    class FileReader : IDisposable
    {
        Shape shape;
        SceneThread sceneThread;
        public FileReader()
        {
            deleGetShape = new Cpp2Managed.Shape3D.OnGetShape(getShape);
            deleGetEdge = new Cpp2Managed.Shape3D.OnGetEdge(getSliceEdge);
            deleGetLocatePlane = new Cpp2Managed.Shape3D.OnGetShape(getLocatePlane);
            sceneThread = new SceneThread();
            shape = new Shape();
        }

        public Shape Shape
        {
            get
            {
                return shape;
            }
        }

        public bool openStep(string fileName, SceneThread.afterFunction afterOpenStep)
        {
            ArrayList list = new ArrayList();
            list.Add(fileName);
            sceneThread.addWork(openStepWork, list, afterOpenStep);
            return true;
        }

        public bool importMoreStep(string fileName, SceneThread.afterFunction afterImport)
        {
            ArrayList list = new ArrayList();
            list.Add(fileName);
            sceneThread.addWork(openStepWork, list, afterImport);
            return true;
        }

        public static IntPtr NativeUtf8FromString(string managedString)
        {
            int len = Encoding.UTF8.GetByteCount(managedString);
            byte[] buffer = new byte[len + 1];
            Encoding.UTF8.GetBytes(managedString, 0, managedString.Length, buffer, 0);
            IntPtr nativeUtf8 = Marshal.AllocHGlobal(buffer.Length);
            Marshal.Copy(buffer, 0, nativeUtf8, buffer.Length);
            return nativeUtf8;
        }

        Cpp2Managed.Shape3D.OnGetShape deleGetShape;
        void getShape(IntPtr shapePt)
        {
            shape.setShape(shapePt);
        }

        private object openStepWork(object args)
        {
            ArrayList list = (ArrayList)args;
            string fileName = (string)list[0];
            IntPtr fileNameSpace = NativeUtf8FromString(fileName);
            bool result = false;
            list.Clear();
            if (fileName.EndsWith(".stp") || fileName.EndsWith(".step"))
            {
                result = Cpp2Managed.Shape3D.ImportStep(fileNameSpace, getShape);
                if (result)
                    shape.fileName = fileName;
            }
            else if (fileName.EndsWith(".stl") || fileName.EndsWith("ast"))
            {
                IntPtr shapePt = Cpp2Managed.Shape3D.ImportStl(fileNameSpace);
                if (shapePt != IntPtr.Zero)
                {
                    shape.setShape(shapePt);
                    shape.fileName = fileName;
                    result = true;
                }
            }
            Marshal.FreeHGlobal(fileNameSpace);
            list.Add(result);
            return list;
        }

        public bool openSlice(string fileName, SceneThread.afterFunction afterOpenSlice)
        {
            ArrayList list = new ArrayList();
            list.Add(fileName);
            sceneThread.addWork(openSliceWork, list, afterOpenSlice);
            return true;
        }

        Cpp2Managed.Shape3D.OnGetEdge deleGetEdge;
        void getSliceEdge(Cpp2Managed.EdgeType type, double[] data, int length)
        {

        }
        object openSliceWork(object args)
        {
            ArrayList list = (ArrayList)args;
            string fileName = (string)list[0];
            IntPtr fileNameSpace = NativeUtf8FromString(fileName);
            bool result = false;
            list.Clear();
            if (fileName.EndsWith(".slc"))
            {
                result = Cpp2Managed.Shape3D.ImportSlice(fileNameSpace, deleGetEdge);
            }
            Marshal.FreeHGlobal(fileNameSpace);
            list.Add(result);
            return list;
        }

        /*
        public bool displayStep(object workResult, bool combine)
        {
            ArrayList list = (ArrayList)workResult;
            bool result = (bool)list[0];
            if (!result)
            {
                return result;
            }
            Shape shape = (Shape)list[1];
            if (combine)
            {
                combineShapes(shape);
                shape = Shape;
            }
            else
            {
                Cpp2Managed.getShapeBoundary(shape.shape, 0, ref shape.Zmin, ref shape.Zmax, ref shape.Ymin, ref shape.Ymax, ref shape.Xmin, ref shape.Xmax);
                shapeList.Add(shape);
            }
            return displayShape(shape);
        }
        */

        public void saveSlice(string path)
        {
            IntPtr[] slices = new IntPtr[shape.slice.sliceList.Count];
            int i = 0;
            foreach (Slice.OneSlice slice in shape.slice.sliceList)
            {
                slices[i++] = slice.slice;
            }
            IntPtr fileName = NativeUtf8FromString(path);
            Cpp2Managed.Shape3D.exportStep(fileName, slices, shape.slice.sliceList.Count);
            Marshal.FreeHGlobal(fileName);
        }

        public void saveStep(string path)
        {
            IntPtr fileName = NativeUtf8FromString(path);
            IntPtr[] shapeList = new IntPtr[1];
            shapeList[0] = shape.getShape();
            Cpp2Managed.Shape3D.exportStep(fileName, shapeList, 1);
        }

        public void sliceShape(Control control, bool locatePlane, bool gradientShape, bool noDelay, SceneThread.afterFunction afterSlice, SceneThread.onFunction onSlice)
        {
            ArrayList args = new ArrayList();
            args.Add(control);
            args.Add(onSlice);
            args.Add(locatePlane);
            args.Add(gradientShape);
            args.Add(noDelay);
            sceneThread.addWork(sliceShapeWork, args, afterSlice);
        }

        delegate bool DisplayOneShape(IntPtr shape, double height, SceneThread.onFunction onSlice);
        Cpp2Managed.Shape3D.OnGetShape deleGetLocatePlane;
        void getLocatePlane(IntPtr locatePlanePt)
        {
            double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, Zmin = 0, Zmax = 0;
            Cpp2Managed.Shape3D.getBoundary(locatePlanePt, ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
            Slice.OneSlice slice = new Slice.OneSlice(locatePlanePt, Zmin, true);
            shape.slice.locateList.Add(slice);
        }
        private object sliceShapeWork(object args)
        {
            ArrayList list = (ArrayList)args;
            Control control = (Control)list[0];
            SceneThread.onFunction onslice = (SceneThread.onFunction)list[1];
            bool locatePlane = (bool)list[2];
            bool gradientShape = (bool)list[3];
            bool noDelay = (bool)list[4];
            // 确定定位面，与定位面等高的切割线要特殊处理
            Cpp2Managed.Shape3D.getLocatPlane(shape.getShape(), deleGetLocatePlane);
            double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, Zmin = 0, Zmax = 0;
            Cpp2Managed.Shape3D.getBoundary(shape.getShape(), ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
            double height = Zmin;
            for (int i = 1; height < Zmax; i++)
            {
                // 从1开始，跳过0高度切割
                height = Zmin + (double)i * shape.slice.sliceThick;
                bool skip = false;
                foreach (Slice.OneSlice s in shape.slice.locateList)
                {
                    // 与定位面高度重复的不能进行普通切割操作
                    if (Math.Abs(s.height - height) < 0.0001)
                    {
                        skip = true;
                        onGetSlice(s.slice, s.height, control, onslice);
                        shape.slice.sliceList.Add(s);
                    }
                    // 显示对应高度的定位面切割线
                    else if (locatePlane && s.height < height && s.height > height - shape.slice.sliceThick)
                    {
                        onGetSlice(s.slice, s.height, control, onslice);
                        shape.slice.sliceList.Add(s);
                    }
                }
                if (skip)
                    continue;
                IntPtr slice = Cpp2Managed.Shape3D.slice(shape.getShape(), height);
                if (slice == IntPtr.Zero)
                    continue;
                shape.slice.sliceList.Add(new Slice.OneSlice(slice, height, false));
                onGetSlice(slice, height, control, onslice);
                if (!noDelay)
                    System.Threading.Thread.Sleep(100);
            }
            shape.slice.sortSliceList();
            return null;
        }

        private void onGetSlice(IntPtr slice, double height, Control control, SceneThread.onFunction onSlice)
        {
            if (slice != IntPtr.Zero)
            {
                control.Dispatcher.Invoke(new DisplayOneShape(displayOnSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { slice, height, onSlice});
            }
        }

        private bool displayOnSlice(IntPtr slice, double height, SceneThread.onFunction onSlice)
        {
            ArrayList list = new ArrayList();
            list.Add(slice);
            list.Add(height);
            onSlice(list);
            return true;
        }

        public void releaseShape()
        {
            releaseTransform();
            if (shape.slice.sliceList.Count > 0)
            {
                foreach (Slice.OneSlice slice in shape.slice.sliceList)
                {
                    //Cpp2Managed.Shape3D.deleteSlice(slice.slice);
                }
            }
            else
                Cpp2Managed.Shape3D.del(shape.getShape());
        }

        public void releaseTransform()
        {
            if (shape.transform != IntPtr.Zero)
            {
                Cpp2Managed.Shape3D.del(shape.transform);
                Marshal.FreeHGlobal(shape.transform);
                shape.transform = IntPtr.Zero;
            }
        }

        public void applyTransform()
        {
            if (shape.transform == IntPtr.Zero)
                return;
            Cpp2Managed.Shape3D.del(shape.getShape());
            shape.setShape(shape.transform);
            shape.transform = IntPtr.Zero;
        }

        public void Dispose()
        {
            sceneThread.Dispose(null);
        }
    }
}
using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Windows.Controls;

namespace Wpf3DPrint.Viewer
{
    class FileReader
    {
        Scene scene;
        ArrayList shapeList;
        string fileName;
        public FileReader(Scene scene)
        {
            this.scene = scene;
            shapeList = new ArrayList();
        }

        public Shape Shape
        {
            get
            {
                return (Shape)shapeList[0];
            }
        }

        public bool HasFile
        {
            get
            {
                return shapeList.Count > 0;
            }
        }

        public string FileName
        {
            get
            {
                return fileName;
            }
        }

        public bool openStep(string fileName, SceneThread.afterFunction afterOpenStep, bool isSlice)
        {
            ArrayList list = new ArrayList();
            list.Add(fileName);
            list.Add(isSlice);
            scene.D3DThread.addWork(openStepWork, list, afterOpenStep);
            this.fileName = fileName;
            return true;
        }

        private object openStepWork(object args)
        {
            ArrayList list = (ArrayList)args;
            string fileName = (string)list[0];
            bool isSlice = (bool)list[1];
            int count = 500;
            IntPtr shapePt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * count);
            IntPtr slice = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)));
            bool result = Cpp2Managed.ImportStep(Marshal.StringToHGlobalAnsi(fileName), ref count, shapePt, isSlice, slice);
            list.Clear();
            list.Add(result);
            Shape shape = new Shape(shapePt, count);
            if (isSlice)
                shape.stepSlice = slice;
            list.Add(shape);
            return list;
        }

        public bool displayStep(object workResult)
        {
            ArrayList list = (ArrayList)workResult;
            bool result = (bool)list[0];
            if (!result)
            {
                return result;
            }
            Shape shape = (Shape)list[1];
            shapeList.Add(shape);
            Cpp2Managed.getShapeBoundary(shape.shape, 0, ref shape.Zmin, ref shape.Zmax, ref shape.Ymin, ref shape.Ymax, ref shape.Xmin, ref shape.Xmax);
            return displayShape(shape);
        }

        private bool displayShape(Shape shape)
        {
            scene.Proxy.SetDisplayMode(1);
            scene.Proxy.displayShape(shape.shape, 0, 0.7);
            return true;
        }

        public unsafe void saveSlice(string path)
        {
            Shape shape = (Shape)shapeList[0];
            IntPtr[] slices = new IntPtr[shape.sliceList.Count];
            int i = 0;
            foreach (Shape.Slice slice in shape.sliceList)
            {
                slices[i++] = slice.slice;
            }
            Cpp2Managed.exportStep(Marshal.StringToHGlobalAnsi(path), slices, shape.sliceList.Count);
        }

        public void sliceShape(Control control, SceneThread.afterFunction afterSlice, SceneThread.onFunction onSlice)
        {
            ArrayList args = new ArrayList();
            args.Add(control);
            args.Add(onSlice);
            scene.D3DThread.addWork(sliceShapeWork, args, afterSlice);
        }

        delegate bool DisplayOneShape(IntPtr shape, SceneThread.onFunction onSlice, ArrayList args);
        private object sliceShapeWork(object args)
        {
            ArrayList list = (ArrayList)args;
            Control control = (Control)list[0];
            SceneThread.onFunction onslice = (SceneThread.onFunction)list[1];
            foreach (Shape shape in shapeList)
            {
                double height = shape.Zmin;
                int i = 0;
                for (i = 0; height < shape.Zmax - 0.000001; i++)
                {
                    height = shape.Zmin + (double)i * shape.sliceThick;
                    IntPtr slice = Cpp2Managed.SliceShape(shape.shape, 0, height);
                    shape.sliceList.Add(new Shape.Slice(slice, height));
                    onGetSlice(slice, shape, i, control, onslice);
                }
                int sliceCnt = i;
                int count = 0;
                IntPtr containers = Cpp2Managed.getLocatPlane(shape.shape, 0, ref count);
                for (i = 0; i < count; i++)
                {
                    IntPtr slice = Cpp2Managed.getShapeContainer(containers, i, ref height);
                    bool skip = false;
                    foreach (Shape.Slice s in shape.sliceList)
                    {
                        // 高度重复的跳过
                        if (Math.Abs(s.height - height) < 0.0001)
                        {
                            skip = true;
                            break;
                        }
                    }
                    if (skip)
                        continue;
                    shape.sliceList.Add(new Shape.Slice(slice, height));
                    onGetSlice(slice, shape, sliceCnt + i, control, onslice);
                }
                shape.sortSliceList();
            }
            return null;
        }

        private void onGetSlice(IntPtr slice, Shape shape, int i, Control control, SceneThread.onFunction onSlice)
        {
            if (slice != IntPtr.Zero)
            {
                ArrayList onArgs = new ArrayList();
                onArgs.Add(shape);
                onArgs.Add(i);
                control.Dispatcher.Invoke(new DisplayOneShape(displaySlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { slice, onSlice, onArgs });
            }
        }

        private bool displaySlice(IntPtr slice, SceneThread.onFunction onSlice, ArrayList onArgs)
        {
            scene.Proxy.displaySlice(slice);
            onSlice(onArgs);
            return true;
        }

        public void sceneZoom(int delta)
        {
            scene.Proxy.Zoom(0, 0, delta / 8, 0);
        }

        public int afterOpenSlice(object workResult)
        {
            ArrayList list = (ArrayList)workResult;
            bool result = (bool)list[0];
            if (!result)
            {
                return -1;
            }
            Shape shape = (Shape)list[1];
            shapeList.Add(shape);
            for (int i = 0; i < shape.count; i++)
            {
                double height = 0;
                IntPtr slice = Cpp2Managed.getSliceFromShape(shape.shape, i, ref height);
                shape.sliceList.Add(new Shape.Slice(slice, height));
            }
            return shape.count;
        }

        public void displaySlice()
        {
            foreach(Shape.Slice slice in Shape.sliceList)
            {
                scene.Proxy.displaySlice(slice.slice);
            }
            scene.Proxy.ZoomAllView();
        }

        public void selectSlice(int i)
        {
            scene.Proxy.selectSlice(((Shape.Slice)(Shape.sliceList[i])).slice);
        }

        public void rebuildSlice()
        {
            scene.Proxy.removeObjects();
            foreach (Shape.Slice slice in Shape.sliceList)
            {
                scene.Proxy.strechSlice(slice.slice, Shape.sliceThick);
            }
        }

        public void releaseShape()
        {
            foreach (Shape shape in shapeList)
            {
                foreach (Shape.Slice slice in shape.sliceList)
                {
                    Cpp2Managed.deleteSlice(slice.slice);
                }
                Cpp2Managed.deleteShape(shape.shape, shape.count);
                Marshal.FreeHGlobal(shape.shape);
            }
            shapeList.Clear();
            scene.Proxy.removeObjects();
        }

        public void afterOpenFile()
        {
            scene.Proxy.ZoomAllView();
        }
    }
}
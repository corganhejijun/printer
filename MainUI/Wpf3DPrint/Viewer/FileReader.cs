using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Windows.Controls;

namespace Wpf3DPrint.Viewer
{
    class FileReader
    {
        Scene scene;
        Scene sliceScene;
        ArrayList shapeList;
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

        public void setSliceScene(Scene sliceSene)
        {
            this.sliceScene = sliceSene;
        }

        public bool openStep(string fileName, SceneThread.afterFunction afterOpenStep)
        {
            ArrayList list = new ArrayList();
            list.Add(fileName);
            scene.D3DThread.addWork(openStepWork, list, afterOpenStep);
            return true;
        }

        private object openStepWork(object args)
        {
            ArrayList list = (ArrayList)args;
            string fileName = (string)list[0];
            int count = 500;
            IntPtr shapePt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * count);
            bool result = Cpp2Managed.ImportStep(Marshal.StringToHGlobalAnsi(fileName), ref count, shapePt);
            list.Clear();
            list.Add(result);
            Shape shape = new Shape(shapePt, count);
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

        private bool displaySlice(IntPtr slice, SceneThread.onFunction onSlice, ArrayList onArgs)
        {
            scene.Proxy.displaySlice(slice);
            displaySlice(slice);
            onSlice(onArgs);
            return true;
        }

        private void displaySlice(IntPtr slice)
        {
            sliceScene.Proxy.EraseObjects();
            sliceScene.Proxy.displaySlice(slice);
            sliceScene.Proxy.ZoomAllView();
        }

        public unsafe void saveSlice(string path)
        {
            sliceScene.Proxy.ExportStep((sbyte*)Marshal.StringToHGlobalAnsi(path));
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
                for (int i = 0; i < shape.sliceCnt; i++)
                {
                    double height = shape.Zmin + (double)i / shape.sliceCnt * (shape.Zmax - shape.Zmin);
                    IntPtr slice = Cpp2Managed.SliceShape(shape.shape, 0, shape.Zmax, shape.Zmin, height);
                    shape.sliceList.Add(slice);
                    if (slice != IntPtr.Zero)
                    {
                        ArrayList onArgs = new ArrayList();
                        onArgs.Add(shape);
                        onArgs.Add(i);
                        control.Dispatcher.Invoke(new DisplayOneShape(displaySlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { slice, onslice, onArgs });
                    }
                }
            }
            return null;
        }

        public void displaySlice(int index)
        {
            Shape shape = (Shape)shapeList[0];
            IntPtr slice = (IntPtr)shape.sliceList[index];
            displaySlice(slice);
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
                shape.sliceCnt = shape.count;
                shape.sliceList.Add(Cpp2Managed.getSliceFromShape(shape.shape, i));
            }
            displaySlice(0);
            return shape.count;
        }

        public void releaseShape()
        {
            foreach (Shape shape in shapeList)
            {
                foreach (IntPtr slice in shape.sliceList)
                {
                    Cpp2Managed.deleteSlice(slice);
                }
                Cpp2Managed.deleteShape(shape.shape, shape.count);
                Marshal.FreeHGlobal(shape.shape);
            }
            shapeList.Clear();
            scene.Proxy.removeObjects();
            sliceScene.Proxy.removeObjects();
        }

        public void afterOpenFile()
        {
            scene.Proxy.ZoomAllView();
            sliceScene.Proxy.TopView();
        }
    }
}
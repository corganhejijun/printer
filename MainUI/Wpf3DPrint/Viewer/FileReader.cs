using System;
using System.Collections;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows.Controls;

namespace Wpf3DPrint.Viewer
{
    class FileReader
    {
        class Shape
        {
            public IntPtr shape;
            public int count;
            public Shape(IntPtr shape, int count)
            {
                this.shape = shape;
                this.count = count;
            }
        }

        Scene scene;
        ArrayList shapeList;
        public FileReader(Scene scene)
        {
            this.scene = scene;
            shapeList = new ArrayList();
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
            int count = 100;
            IntPtr shapePt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * count);
            bool result = Cpp2Managed.ImportStep(Marshal.StringToHGlobalAnsi(fileName), ref count, shapePt);
            list.Clear();
            list.Add(result);
            list.Add(shapePt);
            list.Add(count);
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
            IntPtr shapePt = (IntPtr)list[1];
            int count = (int)list[2];
            Shape shape = new Shape(shapePt, count);
            shapeList.Add(shape);
            return displayShape(shapePt, 0);
        }

        private bool displayShape(IntPtr shapePt, int index)
        {
            scene.Proxy.SetDisplayMode(1);
            scene.Proxy.displayShape(shapePt, index, 0.7);
            return true;
        }

        private bool displayOneShape(IntPtr shapePt)
        {
            scene.Proxy.displayShape(shapePt);
            return true;
        }

        public void sliceShape(Control control, SceneThread.afterFunction afterSlice)
        {
            ArrayList args = new ArrayList();
            args.Add(control);
            scene.D3DThread.addWork(sliceShapeWork, args, afterSlice);
        }

        delegate bool DisplayOneShape(IntPtr shape);
        private object sliceShapeWork(object args)
        {
            ArrayList list = (ArrayList)args;
            Control control = (Control)list[0];
            foreach (Shape shape in shapeList)
            {
                int count = 50;
                for (int i = 0; i <= count; i++)
                {
                    double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
                    Xmin = Xmax = Ymin = Ymax = Zmin = Zmax = 0;
                    Cpp2Managed.getShapeBoundary(shape.shape, 0, ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
                    double height = 0;
                    if (i == count)
                        height = Zmax;
                    else
                        height = Zmin + (double)i / count * (Zmax - Zmin);
                    IntPtr slice = Cpp2Managed.SliceShape(shape.shape, 0, Zmax, Zmin, height);
                    control.Dispatcher.Invoke(new DisplayOneShape(displayOneShape), System.Windows.Threading.DispatcherPriority.Normal, new object[] { slice });
                }
            }
            return null;
        }

        public void releaseShape()
        {
            foreach (Shape shape in shapeList)
            {
                Cpp2Managed.deleteShape(shape.shape, shape.count);
                Marshal.FreeHGlobal(shape.shape);
            }
        }

        public void afterOpenFile()
        {
            scene.Proxy.ZoomAllView();
        }
    }
}
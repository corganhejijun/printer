using System;
using System.Collections;

namespace Wpf3DPrint.Viewer
{
    public class Shape
    {
        public IntPtr shape;
        public IntPtr stepSlice;
        public int count;   // shape 指向的 shape container 内包含的 shape 数
        public double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
        public double sliceThick;
        public ArrayList sliceList;
        public Shape(IntPtr shape, int count)
        {
            stepSlice = IntPtr.Zero;
            this.shape = shape;
            this.count = count;
            Xmin = Xmax = Ymin = Ymax = Zmin = Zmax = 0;
            sliceThick = 1;
            sliceList = new ArrayList();
        }
    }
}

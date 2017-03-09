using System;
using System.Collections;

namespace Wpf3DPrint.Viewer
{
    public class Shape
    {
        public IntPtr shape;
        public int count;   // shape 指向的 shape container 内包含的 shape 数
        public double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
        public int sliceCnt;
        public ArrayList sliceList;
        public Shape(IntPtr shape, int count)
        {
            this.shape = shape;
            this.count = count;
            Xmin = Xmax = Ymin = Ymax = Zmin = Zmax = 0;
            sliceCnt = 10;
            sliceList = new ArrayList();
        }
    }
}

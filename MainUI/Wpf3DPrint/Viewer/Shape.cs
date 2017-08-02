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
        public int locateCount;
        public bool countLocate;
        public ArrayList sliceList;
        public IntPtr transform;

        public class SliceCompare : IComparer {
            public int Compare(object x, object y)
            {
                return Comparer.Default.Compare(((Slice)x).height, ((Slice)y).height);
            }
        }

        public class Slice
        {
            public IntPtr slice;
            public double height;
            public Slice(IntPtr s, double h)
            {
                slice = s;
                height = h;
            }
        }
        public Shape(IntPtr shape, int count)
        {
            stepSlice = IntPtr.Zero;
            this.shape = shape;
            this.count = count;
            Xmin = Xmax = Ymin = Ymax = Zmin = Zmax = 0;
            sliceThick = 1;
            sliceList = new ArrayList();
            countLocate = true;
            transform = IntPtr.Zero;
        }
        public void sortSliceList()
        {
            sliceList.Sort(new SliceCompare());
            sliceList.Reverse();
        }
    }
}

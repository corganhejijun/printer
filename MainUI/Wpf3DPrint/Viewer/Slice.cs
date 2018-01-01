using System;
using System.Collections;

namespace Wpf3DPrint.Viewer
{
    public class Slice
    {
        public double sliceThick;
        public ArrayList sliceList;
        public ArrayList locateList;

        public class OneSlice
        {
            public IntPtr slice;
            public double height;
            public bool isLocatePlane;
            public OneSlice(IntPtr s, double h, bool locate)
            {
                slice = s;
                height = h;
                isLocatePlane = locate;
            }
        }

        public Slice()
        {
            sliceThick = 1.0;
            sliceList = new ArrayList();
            locateList = new ArrayList();
        }

        public class SliceCompare : IComparer
        {
            public int Compare(object x, object y)
            {
                return Comparer.Default.Compare(((OneSlice)x).height, ((OneSlice)y).height);
            }
        }

        public void sortSliceList()
        {
            sliceList.Sort(new SliceCompare());
            sliceList.Reverse();
        }
    }
}

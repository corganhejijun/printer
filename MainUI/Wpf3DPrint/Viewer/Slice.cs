using System;
using System.Collections;

namespace Wpf3DPrint.Viewer
{
    public partial class Slice
    {
        public double sliceThick;
        public ArrayList sliceList;
        public string fileName;

        public Slice()
        {
            sliceThick = 1.0;
            sliceList = new ArrayList();
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

        public void combine()
        {
            for (int i = 0; i < sliceList.Count; i++)
            {
                for (int j = i + 1; j < sliceList.Count; j++)
                {
                    Slice.OneSlice slice1 = (Slice.OneSlice)sliceList[i];
                    Slice.OneSlice slice2 = (Slice.OneSlice)sliceList[j];
                    if (Math.Abs(slice1.height - slice2.height) < 0.0001)
                    {
                        IntPtr result = Cpp2Managed.Shape3D.combine(slice1.slice, slice2.slice);
                        Cpp2Managed.Shape3D.del(slice1.slice);
                        Cpp2Managed.Shape3D.del(slice2.slice);
                        slice1.slice = result;
                        foreach (object obj in slice2.data)
                            slice1.data.Add(obj);
                        sliceList.RemoveAt(j);
                        i--;
                        break;
                    }
                }
            }
        }

        public void base0SliceList()
        {
            double Zmin = double.MaxValue, Zmax = double.MinValue;
            foreach (OneSlice slice in sliceList)
            {
                if (slice.slice == IntPtr.Zero)
                    continue;
                double xmin = double.MaxValue, xmax = double.MinValue, ymin = double.MaxValue, ymax = double.MinValue, zmin = double.MaxValue, zmax = double.MinValue;
                Cpp2Managed.Shape3D.getBoundary(slice.slice, ref zmin, ref zmax, ref ymin, ref ymax, ref xmin, ref xmax);
                if (zmin < Zmin) Zmin = zmin;
                if (zmax > Zmax) Zmax = zmax;
            }
            if (Zmin < 0.0001 && Zmin > -0.0001)
                return;
            for (int i = 0; i < sliceList.Count; i++)
            {
                OneSlice slice = (OneSlice)sliceList[i];
                if (slice.slice == IntPtr.Zero)
                    continue;
                IntPtr move = Cpp2Managed.Shape3D.move(slice.slice, 0, 0, -Zmin);
                Cpp2Managed.Shape3D.del(slice.slice);
                slice.slice = move;
            }
        }

        public void base0XyCenter()
        {
            double Xmin = double.MaxValue, Xmax = double.MinValue, Ymin = double.MaxValue, Ymax = double.MinValue, Zmin = double.MaxValue, Zmax = double.MinValue;
            foreach (OneSlice slice in sliceList)
            {
                double xmin = double.MaxValue, xmax = double.MinValue, ymin = double.MaxValue, ymax = double.MinValue, zmin = double.MaxValue, zmax = double.MinValue;
                Cpp2Managed.Shape3D.getBoundary(slice.slice, ref zmin, ref zmax, ref ymin, ref ymax, ref xmin, ref xmax);
                if (zmin < Zmin) Zmin = zmin;
                if (zmax > Zmax) Zmax = zmax;
                if (xmin < Xmin) Xmin = xmin;
                if (xmax > Xmax) Xmax = xmax;
                if (ymin < Ymin) Ymin = ymin;
                if (ymax > Ymax) Ymax = ymax;
            }
            double centerX = Xmin + (Xmax - Xmin) / 2;
            double centerY = Ymin + (Ymax - Ymin) / 2;
            for (int i = 0; i < sliceList.Count; i++)
            {
                OneSlice slice = (OneSlice)sliceList[i];
                if (slice.slice == IntPtr.Zero)
                    continue;
                IntPtr move = Cpp2Managed.Shape3D.move(slice.slice, -centerX, -centerY, 0);
                Cpp2Managed.Shape3D.del(slice.slice);
                slice.slice = move;
            }
        }

        public Cpp2Managed.BoundBox getBound()
        {
            Cpp2Managed.BoundBox box = new Cpp2Managed.BoundBox();
            box.left = double.MaxValue;
            box.right = double.MinValue;
            box.top = double.MinValue;
            box.bottom = double.MaxValue;
            foreach (OneSlice slice in sliceList)
            {
                Cpp2Managed.BoundBox oneBox = new Cpp2Managed.BoundBox();
                oneBox.left = double.MaxValue;
                oneBox.right = double.MinValue;
                oneBox.top = double.MinValue;
                oneBox.bottom = double.MaxValue;
                slice.GetBound(ref oneBox);
                if (oneBox.left < box.left) box.left = oneBox.left;
                if (oneBox.right > box.right) box.right = oneBox.right;
                if (oneBox.top > box.top) box.top = oneBox.top;
                if (oneBox.bottom < box.bottom) box.bottom = oneBox.bottom;
            }
            return box;
        }

        public void release()
        {
            if (sliceList.Count > 0)
            {
                foreach (Slice.OneSlice slice in sliceList)
                {
                    Cpp2Managed.Shape3D.del(slice.slice);
                }
                sliceList.Clear();
            }
        }
    }
}

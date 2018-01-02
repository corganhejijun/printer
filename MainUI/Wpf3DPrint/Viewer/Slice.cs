using System;
using System.Collections;

namespace Wpf3DPrint.Viewer
{
    public class Slice
    {
        public double sliceThick;
        public ArrayList sliceList;

        public class OneSlice
        {
            public IntPtr slice;
            public double height;
            public bool isLocatePlane;
            public OneSlice(IntPtr s, double h)
            {
                slice = s;
                height = h;
                isLocatePlane = false;
            }

            object data;
            public void getSliceData(Cpp2Managed.EdgeType type, double[] dataList, int length)
            {
                switch (type)
                {
                    case Cpp2Managed.EdgeType.bSplice:
                        data = getBSplice(dataList, length);
                        break;
                    case Cpp2Managed.EdgeType.circle:
                        data = getCircle(dataList);
                        break;
                    case Cpp2Managed.EdgeType.line:
                        data = getLine(dataList);
                        break;
                    default:
                        break;
                }
            }

            Cpp2Managed.BSpline getBSplice(double[] dataList, int length)
            {
                Cpp2Managed.BSpline bSpline = new Cpp2Managed.BSpline();
                bSpline.start.x = dataList[1];
                bSpline.start.y = dataList[2];
                bSpline.end.x = dataList[3];
                bSpline.end.y = dataList[4];
                int poleCount = (length - 5) / 2;
                bSpline.poles = new Cpp2Managed.Point[(length - 5)/2];
                for (int i = 0; i < poleCount; i++)
                {
                    bSpline.poles[i].x = dataList[4 + i * 2 + 1];
                    bSpline.poles[i].y = dataList[4 + i * 2 + 2];
                }
                return bSpline;
            }

            Cpp2Managed.Line getLine(double[] dataList)
            {
                Cpp2Managed.Line line = new Cpp2Managed.Line();
                line.start.x = dataList[1];
                line.start.y = dataList[2];
                line.end.x = dataList[3];
                line.end.y = dataList[4];
                return line;
            }

            Cpp2Managed.Circle getCircle(double[] dataList)
            {
                Cpp2Managed.Circle circle = new Cpp2Managed.Circle();
                circle.center.x = dataList[1];
                circle.center.y = dataList[2];
                circle.startAngle = dataList[3];
                circle.endAngle = dataList[4];
                circle.radius = dataList[5];
                circle.start.x = dataList[6];
                circle.start.y = dataList[7];
                circle.end.x = dataList[8];
                circle.end.y = dataList[9];
                return circle;
            }
        }

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
    }
}

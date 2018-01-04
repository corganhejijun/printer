﻿using System;
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
            public ArrayList data;
            public OneSlice(IntPtr s, double h)
            {
                slice = s;
                height = h;
                data = new ArrayList();
                isLocatePlane = false;
            }

            public void getSliceData(Cpp2Managed.EdgeType type, double[] dataList, int length)
            {
                switch (type)
                {
                    case Cpp2Managed.EdgeType.bSplice:
                        data.Add(getBSplice(dataList, length));
                        break;
                    case Cpp2Managed.EdgeType.circle:
                        data.Add(getCircle(dataList));
                        break;
                    case Cpp2Managed.EdgeType.line:
                        data.Add(getLine(dataList));
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
                bSpline.poles = new Cpp2Managed.Point[(length - 5) / 2];
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

            void setBoundBox(ref Cpp2Managed.BoundBox box, double top, double bottom, double left, double right)
            {
                double t = top < bottom ? top : bottom;
                double b = top < bottom ? bottom : top;
                double l = left < right ? left : right;
                double r = left < right ? right : left;
                if (box.left > l)
                    box.left = l;
                if (box.right < r)
                    box.right = r;
                if (box.top > t)
                    box.top = t;
                if (box.bottom < b)
                    box.bottom = b;
            }

            void getBSplineBoundBox(ref Cpp2Managed.BoundBox box, Cpp2Managed.BSpline spline)
            {
                setBoundBox(ref box, spline.start.y, spline.end.y, spline.start.x, spline.end.x);
                for (int i = 0; i < spline.poles.Length - 1; i += 2)
                    setBoundBox(ref box, spline.poles[i].y, spline.poles[i + 1].y, spline.poles[i].x, spline.poles[i + 1].x);
            }

            public void GetBound(ref Cpp2Managed.BoundBox box)
            {
                foreach(object d in data)
                {
                    if (d is Cpp2Managed.BSpline)
                    {
                        Cpp2Managed.BSpline b = (Cpp2Managed.BSpline)d;
                        getBSplineBoundBox(ref box, b);
                    }
                    else if (d is Cpp2Managed.Line)
                    {
                        Cpp2Managed.Line line = (Cpp2Managed.Line)d;
                        setBoundBox(ref box, line.start.y, line.end.y, line.start.x, line.end.x);
                    }
                    else if (d is Cpp2Managed.Circle)
                    {
                        Cpp2Managed.Circle circle = (Cpp2Managed.Circle)d;
                        setBoundBox(ref box, circle.center.y - circle.radius, circle.center.y + circle.radius, circle.center.x - circle.radius, circle.center.x + circle.radius);
                    }
                }
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

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

            public class Outline
            {
                public ArrayList edges; // 存储data index
                public ArrayList child;
                public Outline parent;
                public bool close;  // 曲线是否闭合

                public Outline()
                {
                    child = new ArrayList();
                    parent = null;
                    edges = new ArrayList();
                    close = false;
                }
            }

            public ArrayList getOutlineList()
            {
                ArrayList outlineList = new ArrayList();
                int[] found = new int[data.Count];
                for (int i = 0; i < found.Length; i++)
                    found[i] = -1;
                bool finish = false;
                Cpp2Managed.Point startPoint = new Cpp2Managed.Point();
                Cpp2Managed.Point endPoint = new Cpp2Managed.Point();
                Outline currentOutline = null;
                while (!finish)
                {
                    int index = 0;
                    if (currentOutline == null)
                    {
                        for (int i = 0; i < found.Length; i++)
                        {
                            if (found[i] == -1)
                            {
                                index = i;
                                break;
                            }
                        }
                        object item = data[index];
                        found[index] = 1;
                        Outline outline = new Outline();
                        currentOutline = outline;
                        if (item is Cpp2Managed.BSpline)
                        {
                            Cpp2Managed.BSpline bs = (Cpp2Managed.BSpline)item;
                            outline.edges.Add(bs);
                            if (Cpp2Managed.Equal(bs.end.x, bs.start.x) && Cpp2Managed.Equal(bs.end.y, bs.start.y))
                            {
                                outline.close = true;
                                currentOutline = null;
                            }
                            else
                            {
                                startPoint.x = bs.start.x;
                                startPoint.y = bs.start.y;
                                endPoint.x = bs.end.x;
                                endPoint.y = bs.end.y;
                            }
                            outlineList.Add(outline);
                        }
                        else if (item is Cpp2Managed.Line)
                        {
                            Cpp2Managed.Line line = (Cpp2Managed.Line)item;
                            startPoint.x = line.start.x;
                            startPoint.y = line.start.y;
                            endPoint.x = line.end.x;
                            endPoint.y = line.end.y;
                            outline.edges.Add(line);
                            outlineList.Add(outline);
                        }
                        else if (item is Cpp2Managed.Circle)
                        {
                            Cpp2Managed.Circle c = (Cpp2Managed.Circle)item;
                            outline.edges.Add(c);
                            if (Cpp2Managed.Equal(c.end.x, c.start.x) && Cpp2Managed.Equal(c.end.y, c.start.y))
                            {
                                outline.close = true;
                                currentOutline = null;
                            }
                            else
                            {
                                startPoint.x = c.start.x;
                                startPoint.y = c.start.y;
                                endPoint.x = c.end.x;
                                endPoint.y = c.end.y;
                            }
                            outlineList.Add(outline);
                        }
                    }
                    else
                    {
                        int i = 0;
                        for (; i < found.Length; i++)
                        {
                            if (found[i] != -1)
                                continue;
                            object t = data[i];
                            if (t is Cpp2Managed.BSpline)
                            {
                                Cpp2Managed.BSpline bs = (Cpp2Managed.BSpline)t;
                                if ((Cpp2Managed.Equal(bs.end.x, endPoint.x) && Cpp2Managed.Equal(bs.end.y, endPoint.y))
                                    || (Cpp2Managed.Equal(bs.start.x, startPoint.x) && Cpp2Managed.Equal(bs.start.y, startPoint.y)))
                                {
                                    bs = Cpp2Managed.reverse(bs);
                                }
                                if (Cpp2Managed.Equal(bs.start.x, endPoint.x) && Cpp2Managed.Equal(bs.start.y, endPoint.y))
                                {
                                    currentOutline.edges.Add(bs);
                                    endPoint.x = bs.end.x;
                                    endPoint.y = bs.end.y;
                                    found[i] = 1;
                                }
                                else if (Cpp2Managed.Equal(bs.end.x, startPoint.x) && Cpp2Managed.Equal(bs.end.y, startPoint.y))
                                {
                                    currentOutline.edges.Insert(0, bs);
                                    startPoint.x = bs.start.x;
                                    startPoint.y = bs.start.y;
                                    found[i] = 1;
                                }
                            }
                            else if (t is Cpp2Managed.Line)
                            {
                                Cpp2Managed.Line line = (Cpp2Managed.Line)t;
                                if ((Cpp2Managed.Equal(line.end.x, endPoint.x) && Cpp2Managed.Equal(line.end.y, endPoint.y))
                                    || (Cpp2Managed.Equal(line.start.x, startPoint.x) && Cpp2Managed.Equal(line.start.y, startPoint.y)))
                                {
                                    line = Cpp2Managed.reverse(line);
                                }
                                if (Cpp2Managed.Equal(line.start.x, endPoint.x) && Cpp2Managed.Equal(line.start.y, endPoint.y))
                                {
                                    currentOutline.edges.Add(line);
                                    endPoint.x = line.end.x;
                                    endPoint.y = line.end.y;
                                    found[i] = 1;
                                }
                                else if (Cpp2Managed.Equal(line.end.x, startPoint.x) && Cpp2Managed.Equal(line.end.y, startPoint.y))
                                {
                                    currentOutline.edges.Insert(0, line);
                                    startPoint.x = line.start.x;
                                    startPoint.y = line.start.y;
                                    found[i] = 1;
                                }
                            }
                            else if (t is Cpp2Managed.Circle)
                            {
                                Cpp2Managed.Circle c = (Cpp2Managed.Circle)t;
                                if ((Cpp2Managed.Equal(c.end.x, endPoint.x) && Cpp2Managed.Equal(c.end.y, endPoint.y))
                                    || (Cpp2Managed.Equal(c.start.x, startPoint.x) && Cpp2Managed.Equal(c.start.y, startPoint.y)))
                                {
                                    c = Cpp2Managed.reverse(c);
                                }
                                if (Cpp2Managed.Equal(c.start.x, endPoint.x) && Cpp2Managed.Equal(c.start.y, endPoint.y))
                                {
                                    currentOutline.edges.Add(c);
                                    endPoint.x = c.end.x;
                                    endPoint.y = c.end.y;
                                    found[i] = 1;
                                }
                                else if (Cpp2Managed.Equal(c.end.x, startPoint.x) && Cpp2Managed.Equal(c.end.y, startPoint.y))
                                {
                                    currentOutline.edges.Insert(0, c);
                                    startPoint.x = c.start.x;
                                    startPoint.y = c.start.y;
                                    found[i] = 1;
                                }
                            }
                            if (found[i] != -1)
                            {
                                if (Cpp2Managed.Equal(endPoint.x, startPoint.x) && Cpp2Managed.Equal(endPoint.y, startPoint.y))
                                {
                                    currentOutline.close = true;
                                    currentOutline = null;
                                }
                                break;
                            }
                        }
                        if (i == found.Length)  // 未找到下一段曲线，轮廓未闭合
                            currentOutline = null;
                    }
                    int count = 0;
                    foreach (int i in found)
                    {
                        if (i == -1)
                            break;
                        else
                            count++;
                    }
                    if (count == found.Length)
                        finish = true;
                }

                // 建立outline之间的包含关系
                for (int i = 0; i < outlineList.Count; i++)
                {
                    Outline outline = (Outline)outlineList[i];
                    for (int j = i + 1; j < outlineList.Count; j++)
                    {
                        Outline o = (Outline)outlineList[j];
                        if (contain(outline, o))
                        {
                            outline.child.Add(o);
                            o.parent = outline;
                            outlineList.RemoveAt(j);
                            break;
                        }
                        if (contain(o, outline))
                        {
                            o.child.Add(outline);
                            outline.parent = o;
                            outlineList[i] = o;
                            outlineList.RemoveAt(j);
                            break;
                        }
                    }
                }
                return outlineList;
            }

            bool contain(Outline o1, Outline o2)
            {
                ArrayList intersercts = new ArrayList();
                Cpp2Managed.Point o2Point;
                object o2Obj = (object)o2.edges[0];
                if (o2Obj is Cpp2Managed.BSpline)
                    o2Point = ((Cpp2Managed.BSpline)o2Obj).start;
                else if (o2Obj is Cpp2Managed.Line)
                    o2Point = ((Cpp2Managed.Line)o2Obj).start;
                else if (o2Obj is Cpp2Managed.Circle)
                    o2Point = ((Cpp2Managed.Circle)o2Obj).start;
                else
                    return false;
                foreach (object obj in o1.edges)
                {
                    Cpp2Managed.Point start, end;
                    if (obj is Cpp2Managed.BSpline)
                    {
                        Cpp2Managed.BSpline bs = (Cpp2Managed.BSpline)obj;
                        start = bs.start;
                        end = bs.end;
                        int i = 0;
                        for (; i < bs.poles.Length; i++)
                        {
                            end = bs.poles[i];
                            if (Cpp2Managed.Equal(start.x, o2Point.x))
                                break;
                            if (Cpp2Managed.Equal(end.x, o2Point.x))
                                break;
                            if ((start.x > o2Point.x && end.x < o2Point.x)
                                || (start.x < o2Point.x && end.x > o2Point.x))
                                break;
                            start = end;
                        }
                        if (i == bs.poles.Length)
                            end = bs.end;
                    }
                    else if (obj is Cpp2Managed.Line)
                    {
                        start = ((Cpp2Managed.Line)obj).start;
                        end = ((Cpp2Managed.Line)obj).end;
                    }
                    else if (obj is Cpp2Managed.Circle)
                    {
                        Cpp2Managed.Circle c = (Cpp2Managed.Circle)obj;
                        start = c.start;
                        end = c.end;
                        double yc = c.start.y - Math.Sin(c.startAngle) * c.radius;
                        double xc = c.start.x - Math.Cos(c.startAngle) * c.radius;
                        double insecAngle1 = Math.Acos((o2Point.x - xc) / c.radius) / 180 * Math.PI;
                        double insecAngle2 = -insecAngle1;
                        if (insecAngle1 < 0)
                            insecAngle1 += Math.PI * 2;
                        if (insecAngle2 < 0)
                            insecAngle2 += Math.PI * 2;
                        if ((insecAngle1 > c.startAngle && insecAngle1 < c.endAngle)
                            || Cpp2Managed.Equal(insecAngle1, c.startAngle) || Cpp2Managed.Equal(insecAngle1, c.endAngle))
                        {
                            double y = Math.Sin(insecAngle1) * c.radius + yc;
                            intersercts.Add(y);
                        }
                        if ((insecAngle2 > c.startAngle && insecAngle2 < c.endAngle)
                            || Cpp2Managed.Equal(insecAngle2, c.startAngle) || Cpp2Managed.Equal(insecAngle2, c.endAngle))
                        {
                            double y = Math.Sin(insecAngle2) * c.radius + yc;
                            intersercts.Add(y);
                        }
                        continue;
                    }
                    else
                        continue;
                    if (Cpp2Managed.Equal(start.x, o2Point.x))
                        intersercts.Add(start.y);
                    else if (Cpp2Managed.Equal(end.x, o2Point.x))
                        intersercts.Add(end.y);
                    else if (start.x < o2Point.x && end.x < o2Point.x)
                        continue;
                    else if (start.x > o2Point.x && end.x > o2Point.x)
                        continue;
                    else
                    {
                        double k = (end.y - start.y) / (end.x - start.x);
                        double b = end.y - k * end.x;
                        double y = k * o2Point.x + b;
                        intersercts.Add(y);
                    }
                }

                int result = 0;
                foreach (double y in intersercts)
                {
                    if (Cpp2Managed.Equal(y, o2Point.y))        // 两轮廓相交
                        return false;
                    if (y < o2Point.y)
                    {
                        if (result == 1)                        // 简易处理，只要有一个点的左右均有交点，说明该点在轮廓内部，证明o1包含o2
                            return true;
                        result = -1;
                    }
                    if (y > o2Point.y)
                    {
                        if (result == -1)
                            return true;
                        result = 1;
                    }
                }
                return false;
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
            box.top = double.MaxValue;
            box.right = double.MinValue;
            box.bottom = double.MinValue;
            foreach (OneSlice slice in sliceList)
            {
                Cpp2Managed.BoundBox oneBox = new Cpp2Managed.BoundBox();
                slice.GetBound(ref oneBox);
                if (oneBox.left < box.left) box.left = oneBox.left;
                if (oneBox.top < box.top) box.top = oneBox.top;
                if (oneBox.right > box.right) box.right = oneBox.right;
                if (oneBox.bottom > box.bottom) box.bottom = oneBox.bottom;
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

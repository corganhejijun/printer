using System;
using System.Collections;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint
{
    class DxfWriter : IDisposable
    {
        enum Type
        {
            unknown, line, circle, bSplice
        }
        FileStream fileStream;
        StreamWriter __file;
        Shape __shape;
        string __fileName;
        public DxfWriter(string fileName, Shape shape)
        {
            __fileName = fileName;
            __shape = shape;
        }

        public void openFile()
        {
            fileStream = new FileStream(__fileName, FileMode.OpenOrCreate);
            __file = new StreamWriter(fileStream, Encoding.UTF8);
        }

        void writeMinMax(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
        {
            __file.WriteLine("  0");
            __file.WriteLine("SECTION");
            __file.WriteLine("  2");
            __file.WriteLine("HEADER");
            __file.WriteLine("  9");
            __file.WriteLine("$EXTMIN");
            __file.WriteLine(" 10");
            __file.WriteLine(Math.Round(minX, 4).ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(Math.Round(minY, 4).ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(Math.Round(minZ, 4).ToString());

            __file.WriteLine("  9");
            __file.WriteLine("$EXTMAX");
            __file.WriteLine(" 10");
            __file.WriteLine(Math.Round(maxX, 4).ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(Math.Round(maxY, 4).ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(Math.Round(maxZ, 4).ToString());
            __file.WriteLine("  0");
            __file.WriteLine("ENDSEC");
        }

        public void exportDxfSlice()
        {
            IntPtr fileNameSpace = FileReader.NativeUnicodeFromString(__fileName);
            IntPtr writer = Cpp2Managed.Shape3D.exportDxfBegin(fileNameSpace);
            Marshal.FreeHGlobal(fileNameSpace);

            foreach (Slice.OneSlice slice in __shape.slice.sliceList)
            {
                foreach (object data in slice.data)
                {
                    if (data is Cpp2Managed.Circle)
                    {
                        Cpp2Managed.Circle circle = (Cpp2Managed.Circle)data;
                        if (IsTwoPi(Math.Abs(circle.endAngle - circle.startAngle)))
                        {
                            Cpp2Managed.Shape3D.writeDxfCircle(writer, TrimZero(circle.center.x), TrimZero(circle.center.y), TrimZero(slice.height), TrimZero(circle.radius), 1);
                        }
                        else
                            Cpp2Managed.Shape3D.writerDxfArc(writer, circle.center.x, circle.center.y, slice.height, circle.radius, circle.startAngle, circle.endAngle, 1);
                    }
                    else if (data is Cpp2Managed.Line)
                    {
                        Cpp2Managed.Line line = (Cpp2Managed.Line)data;
                        Cpp2Managed.Shape3D.writeDxfLine(writer, line.start.x, line.start.y, line.end.x, line.end.y, slice.height);
                    }
                    else if (data is Cpp2Managed.BSpline)
                    {
                        Cpp2Managed.BSpline bs = (Cpp2Managed.BSpline)data;
                        exportDxfBSplice(writer, bs.poles, bs.start.x, bs.start.y, slice.height, bs.end.x, bs.end.y);
                    }
                }
            }


            Cpp2Managed.Shape3D.exportDxfEnd(writer);
        }


        void exportDxfBSplice(IntPtr writer, Cpp2Managed.Point[] polesList, double startX, double startY, double z, double endX, double endY)
        {
            ArrayList xList = new ArrayList();
            ArrayList yList = new ArrayList();
            xList.Add(startX);
            yList.Add(startY);
            foreach (Cpp2Managed.Point pole in polesList)
            {
                xList.Add(pole.x);
                yList.Add(pole.y);
            }
            xList.Add(endX);
            yList.Add(endY);
            for (int i = 1; i < xList.Count - 1; i++)
            {
                if (getDistance((double)xList[i - 1], (double)yList[i - 1], (double)xList[i], (double)yList[i], (double)xList[i + 1], (double)yList[i + 1]) < 0.001)
                {
                    xList.RemoveAt(i);
                    yList.RemoveAt(i);
                    i--;
                    continue;
                }
            }
            for (int i = 1; i < xList.Count; i++)
            {
                Cpp2Managed.Shape3D.writeDxfLine(writer, (double)xList[i - 1], (double)yList[i - 1], (double)xList[i], (double)yList[i], z);
            }
        }

        public void writeSlice()
        {
            double Xmin = double.MaxValue, Xmax = double.MinValue, Ymin = double.MaxValue, Ymax = double.MinValue, Zmin = double.MaxValue, Zmax = double.MinValue;
            foreach (Slice.OneSlice slice in __shape.slice.sliceList)
            {
                double xmin = double.MaxValue, xmax = double.MinValue, ymin = double.MaxValue, ymax = double.MinValue, zmin = double.MaxValue, zmax = double.MinValue;
                if(!Cpp2Managed.Shape3D.getBoundary(slice.slice, ref zmin, ref zmax, ref ymin, ref ymax, ref xmin, ref xmax))
                {
                    xmin = 0; xmax = 0; ymin = 0; ymax = 0; zmin = 0; zmax = 0;

                }
                if (Xmin > xmin) Xmin = xmin;
                if (Ymin > ymin) Ymin = ymin;
                if (Zmin > zmin) Zmin = zmin;
                if (Xmax < xmax) Xmax = xmax;
                if (Ymax < ymax) Ymax = ymax;
                if (Zmax < zmax) Zmax = zmax;
            }
            writeMinMax(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
            __file.WriteLine("  2");
            __file.WriteLine("ENTITIES");
            foreach (Slice.OneSlice slice in __shape.slice.sliceList)
            {
                foreach (object data in slice.data)
                {
                    if (data is Cpp2Managed.Circle)
                    {
                        Cpp2Managed.Circle circle = (Cpp2Managed.Circle)data;
                        getCircle(circle.center.x, circle.center.y, slice.height, circle.radius, circle.startAngle, circle.endAngle, circle.start.x, circle.start.y, circle.end.x, circle.end.y);
                    }
                    else if (data is Cpp2Managed.Line)
                    {
                        Cpp2Managed.Line line = (Cpp2Managed.Line)data;
                        writeLine(line.start.x, line.start.y, line.end.x, line.end.y, slice.height);
                    }
                    else if (data is Cpp2Managed.BSpline)
                    {
                        Cpp2Managed.BSpline bs = (Cpp2Managed.BSpline)data;
                        writeBSplice(bs.poles, bs.start.x, bs.start.y, slice.height, bs.end.x, bs.end.y);
                    }
                }
            }
            __file.WriteLine("  0");
            __file.WriteLine("ENDSEC");
        }

        void writeBSplice(Cpp2Managed.Point[] polesList, double startX, double startY, double z, double endX, double endY)
        {
            ArrayList xList = new ArrayList();
            ArrayList yList = new ArrayList();
            xList.Add(startX);
            yList.Add(startY);
            foreach(Cpp2Managed.Point pole in polesList)
            {
                xList.Add(pole.x);
                yList.Add(pole.y);
            }
            xList.Add(endX);
            yList.Add(endY);
            for (int i = 1; i < xList.Count - 1; i++)
            {
                if (getDistance((double)xList[i - 1], (double)yList[i - 1], (double)xList[i], (double)yList[i], (double)xList[i + 1], (double)yList[i + 1]) < 0.001)
                {
                    xList.RemoveAt(i);
                    yList.RemoveAt(i);
                    i--;
                    continue;
                }
            }
            for (int i = 1; i < xList.Count; i++) {
                writeLine((double)xList[i - 1], (double)yList[i - 1], (double)xList[i], (double)yList[i], z);
            }
        }

        double getDistance(double x1, double y1, double x2, double y2, double x3, double y3)
        {
            // y = ax + b
            double a = (y1 - y2) / (x1 - x2);
            double b = y1 - a * x1;
            // Ax + By + C = 0
            double A = a;
            double B = -1;
            double C = b;
            return Math.Abs(A * x3 + B * y3 + C) / Math.Sqrt(A * A + B * B);
        }

        bool IsTwoPi(double value)
        {
            return (Math.Abs(value - Math.PI * 2) < 0.0001);
        }

        double TrimZero(double value)
        {
            if (value < 0.0001 && value > -0.0001)
                return 0;
            return value;
        }

        void getCircle(double cx, double cy, double cz, double radius, double startAngle, double endAngle, double startX, double startY, double endX, double endY)
        {
            if (IsTwoPi(Math.Abs(endAngle - startAngle)))
            {
                writeCircle(TrimZero(cx), TrimZero(cy), TrimZero(cz), TrimZero(radius), 1);
                return;
            }
            writeArc(TrimZero(cx), TrimZero(cy), TrimZero(cz), TrimZero(radius), TrimZero(startAngle), TrimZero(endAngle), 1);
        }

        void writeCircle(double cx, double cy, double cz, double r, int normal)
        {
            __file.WriteLine("  0");
            __file.WriteLine("CIRCLE");
            __file.WriteLine(" 10");
            __file.WriteLine(Math.Round(cx, 4).ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(Math.Round(cy, 4).ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(Math.Round(cz, 4).ToString());
            __file.WriteLine(" 40");
            __file.WriteLine(Math.Round(r, 4).ToString());
            if (normal < 0)
            {
                __file.WriteLine("230");
                __file.WriteLine("-1.0");
            }
        }

        void writeArc(double cx, double cy, double cz, double r, double startAngle, double endAngle, int normal)
        {
            __file.WriteLine("  0");
            __file.WriteLine("ARC");
            __file.WriteLine(" 10");
            __file.WriteLine(Math.Round(cx, 4).ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(Math.Round(cy, 4).ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(Math.Round(cz, 4).ToString());
            __file.WriteLine(" 40");
            __file.WriteLine(Math.Round(r, 4).ToString());
            __file.WriteLine(" 50");
            __file.WriteLine(Math.Round((startAngle * 180 / Math.PI), 4).ToString());
            __file.WriteLine(" 51");
            __file.WriteLine(Math.Round((endAngle * 180 / Math.PI), 4).ToString());
            if (normal < 0)
            {
                __file.WriteLine("230");
                __file.WriteLine("-1.0");
            }
        }

        void writeLine(double xStart, double yStart, double xEnd, double yEnd, double z)
        {
            __file.WriteLine("  0");
            __file.WriteLine("LINE");
            __file.WriteLine(" 10");
            __file.WriteLine(Math.Round(xStart, 4).ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(Math.Round(yStart, 4).ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(Math.Round(z, 4).ToString());
            __file.WriteLine(" 11");
            __file.WriteLine(Math.Round(xEnd, 4).ToString());
            __file.WriteLine(" 21");
            __file.WriteLine(Math.Round(yEnd, 4).ToString());
        }

        public void fileClose()
        {
            __file.Close();
        }

        public void Dispose()
        {
        }
    }
}

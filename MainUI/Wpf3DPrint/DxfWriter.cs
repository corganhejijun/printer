using System;
using System.IO;
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
        public DxfWriter(string fileName, Shape shape)
        {
            fileStream = new FileStream(fileName, FileMode.OpenOrCreate);
            __file = new StreamWriter(fileStream, Encoding.UTF8);
            __shape = shape;
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
            __file.WriteLine(minX.ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(minY.ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(minZ.ToString());

            __file.WriteLine("  9");
            __file.WriteLine("$EXTMAX");
            __file.WriteLine(" 10");
            __file.WriteLine(maxX.ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(maxY.ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(maxZ.ToString());
            __file.WriteLine("  0");
            __file.WriteLine("ENDSEC");
        }

        public void writeSlice()
        {
            writeMinMax(__shape.Xmin, __shape.Ymin, __shape.Zmin, __shape.Xmax, __shape.Ymax, __shape.Zmax);
            __file.WriteLine("  2");
            __file.WriteLine("ENTITIES");
            IntPtr current = IntPtr.Zero;
            for (int i = 0; i < __shape.count; i++)
            {
                do
                {
                    int type = -1;
                    double x = 0, y = 0, z = 0;
                    double[] p = { 0, 0, 0, 0, 0, 0, 0 };
                    current = Cpp2Managed.exportSlice(__shape.stepSlice, i, current, ref type, ref x, ref y, ref z, p);
                    switch (type)
                    {
                        case (int)Type.circle:
                            getCircle(x, y, z, p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
                            break;
                        case (int)Type.line:
                            writeLine(x, y, p[0], p[1], z);
                            break;
                        case (int)Type.bSplice:
                            break;
                        default:
                            break;
                    }
                } while (current != IntPtr.Zero);
            }
            __file.WriteLine("  0");
            __file.WriteLine("ENDSEC");
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
            __file.WriteLine(cx.ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(cy.ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(cz.ToString());
            __file.WriteLine(" 40");
            __file.WriteLine(r.ToString());
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
            __file.WriteLine(cx.ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(cy.ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(cz.ToString());
            __file.WriteLine(" 40");
            __file.WriteLine(r.ToString());
            __file.WriteLine(" 50");
            __file.WriteLine(startAngle.ToString());
            __file.WriteLine(" 51");
            __file.WriteLine(endAngle.ToString());
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
            __file.WriteLine(xStart.ToString());
            __file.WriteLine(" 20");
            __file.WriteLine(yStart.ToString());
            __file.WriteLine(" 30");
            __file.WriteLine(z.ToString());
            __file.WriteLine(" 11");
            __file.WriteLine(xEnd.ToString());
            __file.WriteLine(" 21");
            __file.WriteLine(yEnd.ToString());
        }

        public void Dispose()
        {
            __file.Close();
        }
    }
}

using System;
using System.Collections;
using System.Drawing;
using System.Windows.Forms;

namespace Wpf3DPrint.Viewer
{
    class Scene2D : IDisposable
    {
        const int PAINT_MARGIN = 30;
        Panel m_panel;
        double m_scale;
        double m_top;
        double m_left;
        bool EQUAL(double a, double b)
        {
            if (a > b - 0.001 && a < b + 0.001)
                return true;
            return false;
        }
        public Scene2D(Panel panel)
        {
            m_panel = panel;
        }

        public void onResize(Cpp2Managed.BoundBox box)
        {
            float rcWidth = m_panel.Width;
            float rcHeight = m_panel.Height;
            double scaleX = (rcWidth - PAINT_MARGIN * 2) / (box.right - box.left);
            double scaleY = (rcHeight - PAINT_MARGIN * 2) / (box.top - box.bottom);
            m_scale = scaleX < scaleY ? scaleX : scaleY;
            m_top = box.top;
            m_left = box.left;
        }

        double coordinateXTrans(double x)
        {
            return (x - m_left) * m_scale + PAINT_MARGIN;
        }

        double coordinateYTrans(double y)
        {
            return (m_top - y) * m_scale + PAINT_MARGIN;
        }

        public void drawSlice(PaintEventArgs e, Slice.OneSlice slice)
        {
            Graphics g = e.Graphics;
            foreach (object obj in slice.data)
            {
                paintSlice(g, obj);
            }
        }

        void paintSlice(Graphics g, object obj)
        {
            if (obj is Cpp2Managed.BSpline)
            {
                Cpp2Managed.BSpline b = (Cpp2Managed.BSpline)obj;
                Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0));
                Point[] points = new Point[b.poles.Length + 2];
                points[0] = new Point((int)coordinateXTrans(b.start.x), (int)coordinateYTrans(b.start.y));
                for (int i = 0; i < b.poles.Length; i++)
                {
                    points[i + 1] = new Point((int)coordinateXTrans(b.poles[i].x), (int)coordinateYTrans(b.poles[i].y));
                }
                points[b.poles.Length + 1] = new Point((int)coordinateXTrans(b.end.x), (int)coordinateYTrans(b.end.y));
                g.DrawLines(pen, points);
            }
            else if (obj is Cpp2Managed.Line)
            {
                Cpp2Managed.Line line = (Cpp2Managed.Line)obj;
                Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0));
                g.DrawLine(pen, new Point((int)coordinateXTrans(line.start.x), (int)coordinateYTrans(line.start.y)), new Point((int)coordinateXTrans(line.end.x), (int)coordinateYTrans(line.end.y)));
            }
            else if (obj is Cpp2Managed.Circle)
            {
                Random ran = new Random();
                Cpp2Managed.Circle circle = (Cpp2Managed.Circle)obj;
                Pen pen = new Pen(Color.FromArgb(255, ran.Next(0, 255), ran.Next(0, 255), ran.Next(0, 255)));
                double x = circle.center.x - circle.radius;
                double y = circle.center.y + circle.radius;
                double width = 2 * circle.radius * m_scale;
                // DrawArc will draw clockwise but we defined arc angles anticlockwise
                // start and end angle may be wrong, recalculate angles by start and end points
                double dy = circle.start.y - circle.center.y;
                double dx = circle.start.x - circle.center.x;
                double startAngle = Math.Atan(dy / dx) / Math.PI * 180;
                if (startAngle < 0)
                    startAngle += 180;
                if (dy < 0)
                    startAngle += 180;
                double dy1 = circle.end.y - circle.center.y;
                double dx1 = circle.end.x - circle.center.x;
                double endAngle = Math.Atan(dy1 / dx1) / Math.PI * 180;
                if (endAngle < 0)
                    endAngle += 180;
                if (dy1 < 0)
                    endAngle += 180;
                if (EQUAL(endAngle, 0) && EQUAL(circle.endAngle, Math.PI * 2))
                    endAngle = 360;
                double sweepAngle = (circle.endAngle - circle.startAngle) / Math.PI * 180;
                if (endAngle > startAngle)
                    g.DrawArc(pen, (float)coordinateXTrans(x), (float)coordinateYTrans(y), (float)width, (float)width, (float)(360 - endAngle), (float)sweepAngle);
                else
                    g.DrawArc(pen, (float)coordinateXTrans(x), (float)coordinateYTrans(y), (float)width, (float)width, (float)(360 - startAngle), (float)sweepAngle);
            }
        }

        public void Dispose()
        {
        }
    }
}

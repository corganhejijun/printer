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

        public void clearWindow()
        {
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
                Cpp2Managed.Circle circle = (Cpp2Managed.Circle)obj;
                Pen pen = new Pen(Color.FromArgb(255, 0, 0, 0));
                double x = circle.center.x - circle.radius;
                double y = circle.center.y + circle.radius;
                double width = 2 * circle.radius * m_scale;
                // DrawArc will draw clockwise but we defined arc angles anticlockwise
                double startAngle = 360 - circle.startAngle / Math.PI * 180;
                double sweepAngle = 360 - circle.endAngle / Math.PI * 180 - startAngle;
                g.DrawArc(pen, (float)coordinateXTrans(x), (float)coordinateYTrans(y), (float)width, (float)width, (float)startAngle, (float)sweepAngle);
            }
        }

        public void Dispose()
        {
        }
    }
}

using System.Windows.Media;

namespace Wpf3DPrint.Viewer
{
    public class Setting
    {
        public Color entityColor;
        public Color lineColor;
        public Color selectEntityColor;
        public Color selectLineColor;

        public Setting()
        {
            entityColor = Color.FromArgb(255, 156, 156, 156);
            lineColor = Color.FromArgb(255, 50, 50, 50);
            selectEntityColor = Color.FromArgb(255, 255, 0, 0);
            selectLineColor = Color.FromArgb(255, 255, 0, 0);
        }
    }
}

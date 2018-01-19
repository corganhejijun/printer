using System.Windows;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for DialogUnit.xaml
    /// </summary>
    public partial class DialogUnit : Window
    {
        string unit;
        public string Unit
        {
            get
            {
                return unit;
            }
        }
        public DialogUnit(Shape shape)
        {
            double Xmin = double.MaxValue, Xmax = double.MinValue, Ymin = double.MaxValue, Ymax = double.MinValue, Zmin = double.MaxValue, Zmax = double.MinValue;
            if (!Cpp2Managed.Shape3D.getBoundary(shape.getShape(), ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax)) {
                Xmin = 0; Xmax = 0; Ymin = 0; Ymax = 0; Zmin = 0; Zmax = 0;
            }
            InitializeComponent();
            labelX.Content = "X高度：" + (Xmax - Xmin).ToString("0.00") + "; 范围：[" + Xmin.ToString("0.00") + "," + Xmax.ToString("0.00") + "]";
            labelY.Content = "Y高度：" + (Ymax - Ymin).ToString("0.00") + "; 范围：[" + Ymin.ToString("0.00") + "," + Ymax.ToString("0.00") + "]";
            labelZ.Content = "Z高度：" + (Zmax - Zmin).ToString("0.00") + "; 范围：[" + Zmin.ToString("0.00") + "," + Zmax.ToString("0.00") + "]";
            unit = comboBox.Text;
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            unit = comboBox.Text.Substring(0, comboBox.Text.IndexOf("("));
            this.DialogResult = true;
        }
    }
}

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
            InitializeComponent();
            labelX.Content = "X高度：" + (shape.Xmax - shape.Xmin).ToString("0.00") + "; 范围：[" + shape.Xmin.ToString("0.00") + "," + shape.Xmax.ToString("0.00") + "]";
            labelY.Content = "Y高度：" + (shape.Ymax - shape.Ymin).ToString("0.00") + "; 范围：[" + shape.Ymin.ToString("0.00") + "," + shape.Ymax.ToString("0.00") + "]";
            labelZ.Content = "Z高度：" + (shape.Zmax - shape.Zmin).ToString("0.00") + "; 范围：[" + shape.Zmin.ToString("0.00") + "," + shape.Zmax.ToString("0.00") + "]";
            unit = comboBox.Text;
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            unit = comboBox.Text;
            this.DialogResult = true;
        }
    }
}

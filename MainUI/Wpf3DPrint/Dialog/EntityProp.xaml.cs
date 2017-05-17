using System.Windows;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for EntityPropxaml.xaml
    /// </summary>
    public partial class EntityProp : Window
    {
        public EntityProp(Shape shape, string unit)
        {
            InitializeComponent();
            labelX.Content = "长度（X）：" + (shape.Xmax - shape.Xmin).ToString("0.00") + " " + unit 
                + "\n范围（X）： [" + shape.Xmin.ToString("0.00") + ", " + shape.Xmax.ToString("0.00") + "](" + unit + ")";
            labelY.Content = "长度（Y）：" + (shape.Ymax - shape.Ymin).ToString("0.00") + " " + unit
                + "\n范围（Y）： [" + shape.Ymin.ToString("0.00") + ", " + shape.Ymax.ToString("0.00") + "](" + unit + ")";
            labelZ.Content = "长度（Z）：" + (shape.Zmax - shape.Zmin).ToString("0.00") + " " + unit
                + "\n范围（Z）： [" + shape.Zmin.ToString("0.00") + ", " + shape.Zmax.ToString("0.00") + "](" + unit + ")";
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
    }
}

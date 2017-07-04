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
            textBoxX.Text = (shape.Xmax - shape.Xmin).ToString("0.00") + " " + unit;
            textBoxXMax.Text = shape.Xmax.ToString("0.00") + " " + unit;
            textBoxXMin.Text = shape.Xmin.ToString("0.00") + " " + unit;
            textBoxY.Text = (shape.Ymax - shape.Ymin).ToString("0.00") + " " + unit;
            textBoxYMax.Text = shape.Ymax.ToString("0.00") + " " + unit;
            textBoxYMin.Text = shape.Ymin.ToString("0.00") + " " + unit;
            textBoxZ.Text = (shape.Zmax - shape.Zmin).ToString("0.00") + " " + unit;
            textBoxZMax.Text = shape.Zmax.ToString("0.00") + " " + unit;
            textBoxZMin.Text = shape.Zmin.ToString("0.00") + " " + unit;
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
    }
}

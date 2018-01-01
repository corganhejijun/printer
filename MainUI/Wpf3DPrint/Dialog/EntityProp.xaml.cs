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
            double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, Zmin = 0, Zmax = 0;
            Cpp2Managed.Shape3D.getBoundary(shape.getShape(), ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
            InitializeComponent();
            textBoxX.Text = (Xmax - Xmin).ToString("0.00") + " " + unit;
            textBoxXMax.Text = Xmax.ToString("0.00") + " " + unit;
            textBoxXMin.Text = Xmin.ToString("0.00") + " " + unit;
            textBoxY.Text = (Ymax - Ymin).ToString("0.00") + " " + unit;
            textBoxYMax.Text = Ymax.ToString("0.00") + " " + unit;
            textBoxYMin.Text = Ymin.ToString("0.00") + " " + unit;
            textBoxZ.Text = (Zmax - Zmin).ToString("0.00") + " " + unit;
            textBoxZMax.Text = Zmax.ToString("0.00") + " " + unit;
            textBoxZMin.Text = Zmin.ToString("0.00") + " " + unit;
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
    }
}

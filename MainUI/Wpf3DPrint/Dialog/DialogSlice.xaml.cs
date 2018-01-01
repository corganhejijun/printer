using System;
using System.Windows;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for DialogSlice.xaml
    /// </summary>
    public partial class DialogSlice : Window
    {
        Viewer.Shape shape;
        double thickness;

        public bool locatePlane
        {
            get
            {
                return (bool)checkBoxLocatePlane.IsChecked;
            }
        }

        public bool gradientShape
        {
            get
            {
                return (bool)checkBoxGradient.IsChecked;
            }
        }

        public bool quickSlice
        {
            get
            {
                return (bool)checkBoxQuikSlice.IsChecked;
            }
        }

        public double outputRatio {
            get
            {
                try {
                    double ratio = double.Parse(textBoxRatio.Text);
                    if (ratio < 0)
                        return 1;
                    return ratio;
                }
                catch
                {
                    return 1;
                }
            }
        }

        double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, Zmin = 0, Zmax = 0;
        public DialogSlice(Viewer.Shape shape, string thick, string unit)
        {
            this.shape = shape;
            InitializeComponent();
            Cpp2Managed.Shape3D.getBoundary(shape.getShape(), ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
            textBoxX.Text = (Xmax - Xmin).ToString("0.00") + " " + unit;
            textBoxX2.Text = Xmin.ToString("0.00") + "~" + Xmax.ToString("0.00") + " " + unit;
            textBoxY.Text = (Ymax - Ymin).ToString("0.00") + " " + unit;
            textBoxY2.Text = Ymin.ToString("0.00") + "~" + Ymax.ToString("0.00") + " " + unit;
            textBoxZ.Text = (Zmax - Zmin).ToString("0.00") + " " + unit;
            textBoxZ2.Text = Zmin.ToString("0.00") + "~" + Zmax.ToString("0.00") + " " + unit;
            try
            {
                thickness = double.Parse(thick);
                textBoxThick.Text = thick;
                int cnt = (int)((Zmax - Zmin) / thickness);
                labelCnt.Content = "预计层数：" + cnt;
            }
            catch
            {
                textBoxThick.Text = "层厚数值错误";
                buttonOK.IsEnabled = false;
            }
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            try {
                thickness = double.Parse(textBoxThick.Text);
                int cnt = (int)((Zmax - Zmin) / thickness);
                float ratio = float.Parse(textBoxRatio.Text);
                labelCnt.Content = "预计层数：" + cnt;
                if (cnt < 2)
                {
                    throw new Exception();
                }
                shape.slice.sliceThick = thickness;
                this.DialogResult = true;
            }
            catch
            {
                e.Handled = false;
                textBoxThick.Text = "输入值错误";
            }
        }

        void calculateLayerNum()
        {
            try
            {
                thickness = double.Parse(textBoxThick.Text);
                int cnt = (int)((Zmax - Zmin) / thickness);
                labelCnt.Content = "预计层数：" + cnt;
            }
            catch
            {
                textBoxThick.Text = "输入值错误";
            }
        }

        private void textBoxThick_LostFocus(object sender, RoutedEventArgs e)
        {
            calculateLayerNum();
        }

        private void textBoxThick_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            calculateLayerNum();
        }
    }
}

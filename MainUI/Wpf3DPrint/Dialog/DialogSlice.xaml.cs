using System;
using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for DialogSlice.xaml
    /// </summary>
    public partial class DialogSlice : Window
    {
        Viewer.Shape shape;
        double thickness;
        public DialogSlice(Viewer.Shape shape, string thick, string unit)
        {
            this.shape = shape;
            InitializeComponent();
            textBoxX.Text = (shape.Xmax - shape.Xmin).ToString("0.00") + " " + unit;
            textBoxX2.Text = shape.Xmin.ToString("0.00") + "~" + shape.Xmax.ToString("0.00") + " " + unit;
            textBoxY.Text = (shape.Ymax - shape.Ymin).ToString("0.00") + " " + unit;
            textBoxY2.Text = shape.Ymin.ToString("0.00") + "~" + shape.Ymax.ToString("0.00") + " " + unit;
            textBoxZ.Text = (shape.Zmax - shape.Zmin).ToString("0.00") + " " + unit;
            textBoxZ2.Text = shape.Zmin.ToString("0.00") + "~" + shape.Zmax.ToString("0.00") + " " + unit;
            try
            {
                thickness = double.Parse(thick);
                textBoxThick.Text = thick;
                int cnt = (int)((shape.Zmax - shape.Zmin) / thickness);
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
                int cnt = (int)((shape.Zmax - shape.Zmin) / thickness);
                float ratio = float.Parse(textBoxRatio.Text);
                labelCnt.Content = "预计层数：" + cnt;
                if (cnt < 2)
                {
                    throw new Exception();
                }
                shape.sliceThick = thickness;
                this.DialogResult = true;
            }
            catch
            {
                e.Handled = false;
                textBoxThick.Text = "输入值错误";
            }
        }

        private void textBoxThick_LostFocus(object sender, RoutedEventArgs e)
        {
            try
            {
                thickness = double.Parse(textBoxThick.Text);
                int cnt = (int)((shape.Zmax - shape.Zmin) / thickness);
                labelCnt.Content = "预计层数：" + cnt;
            }
            catch
            {
                textBoxThick.Text = "输入值错误";
            }
        }
    }
}

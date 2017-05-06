using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for DialogSlice.xaml
    /// </summary>
    public partial class DialogSlice : Window
    {
        Viewer.Shape shape;
        double thickness;
        public DialogSlice(Viewer.Shape shape, string thick)
        {
            this.shape = shape;
            InitializeComponent();
            labelMaxMinInfo.Content = "Z向范围：" + shape.Zmin.ToString("F") + "~" + shape.Zmax.ToString("F");
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
                textBoxThick.Text = "层厚值错误";
            }
        }
    }
}

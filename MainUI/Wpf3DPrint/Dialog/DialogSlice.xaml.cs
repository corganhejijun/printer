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
                labelThickness.Content = "层厚：" + thick;
                int cnt = (int)((shape.Zmax - shape.Zmin) / thickness);
                labelCnt.Content = "预计层数：" + cnt;
            }
            catch
            {
                labelThickness.Content = "层厚数值错误";
                buttonOK.IsEnabled = false;
            }
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            try {
                shape.sliceThick = thickness;
                this.DialogResult = true;
            }
            catch
            { }
        }
    }
}

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
        public DialogSlice(Viewer.Shape shape)
        {
            this.shape = shape;
            InitializeComponent();
            labelMaxMinInfo.Content = "Z向范围：" + shape.Zmin.ToString("F") + "~" + shape.Zmax.ToString("F");
            textBoxHeight.Text = shape.sliceCnt.ToString();
        }

        private void textBoxHeight_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (labelThickness == null)
                return;
            try
            {
                labelThickness.Content = "层厚：" + ((shape.Zmax - shape.Zmin)/int.Parse(textBoxHeight.Text)).ToString("F");
            }
            catch
            {
                labelThickness.Content = "层厚计算错误";
            }
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            try {
                int value = int.Parse(textBoxHeight.Text);
                shape.sliceCnt = value;
                this.DialogResult = true;
            }
            catch
            { }
        }
    }
}

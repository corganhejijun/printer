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
    /// Interaction logic for Param2D.xaml
    /// </summary>
    public partial class Param2D : Window
    {
        public Param2D()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            try {
                float offset = float.Parse(textBoxOffset.Text);
                this.DialogResult = true;
            }
            catch
            {
                MessageBox.Show("请输入正确的参数");
                e.Handled = false;
            }
        }
    }
}

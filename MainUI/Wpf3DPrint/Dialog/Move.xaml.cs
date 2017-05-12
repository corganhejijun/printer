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
    /// Interaction logic for Move.xaml
    /// </summary>
    public partial class Move : Window
    {
        public Move()
        {
            InitializeComponent();
        }

        private void checkBoxMenual_Checked(object sender, RoutedEventArgs e)
        {
            checkBoxAuto.IsChecked = !checkBoxMenual.IsChecked;
        }

        private void checkBoxAuto_Checked(object sender, RoutedEventArgs e)
        {
            try
            {
                checkBoxMenual.IsChecked = !checkBoxAuto.IsChecked;
            }
            catch
            {
                return;
            }
        }

        private void buttonOk_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                float x = float.Parse(textBoxX.Text);
                float y = float.Parse(textBoxY.Text);
                float z = float.Parse(textBoxZ.Text);
                this.DialogResult = true;
            }
            catch
            {
                MessageBox.Show("请输入合法内容");
                e.Handled = false;
            }
        }
    }
}

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
    /// Interaction logic for Rotate.xaml
    /// </summary>
    public partial class Rotate : Window
    {
        public Rotate()
        {
            InitializeComponent();
        }

        public double XAngle
        {
            get
            {
                if (checkBoxX.IsChecked ?? false)
                    return float.Parse(textBoxX.Text);
                return -float.Parse(textBoxX.Text);
            }
        }

        public double YAngle
        {
            get
            {
                if (checkBoxY.IsChecked ?? false)
                    return float.Parse(textBoxY.Text);
                return -float.Parse(textBoxY.Text);
            }
        }

        public double ZAngle
        {
            get
            {
                if (checkBoxZ.IsChecked ?? false)
                    return float.Parse(textBoxZ.Text);
                return -float.Parse(textBoxZ.Text);
            }
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            try {
                testInput();
                this.DialogResult = true;
            }
            catch
            {
                e.Handled = false;
                MessageBox.Show("请输入合法数字");
            }
        }

        void testInput()
        {
            float x = float.Parse(textBoxX.Text);
            float y = float.Parse(textBoxY.Text);
            float z = float.Parse(textBoxZ.Text);
        }

        private void buttonPreview_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                testInput();
            }
            catch
            {
                MessageBox.Show("请输入合法数字");
                return;
            }
        }
    }
}

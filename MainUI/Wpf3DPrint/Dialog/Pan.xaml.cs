using System;
using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for Pan.xaml
    /// </summary>
    public partial class Pan : Window
    {
        MainWindow.TransformPreview preview;
        public Pan(string unit, MainWindow.TransformPreview preview)
        {
            this.preview = preview;
            InitializeComponent();
            labelX.Content = unit;
            labelY.Content = unit;
            labelZ.Content = unit;
        }

        public double X
        {
            get
            {
                return double.Parse(textBoxX.Text);
            }
        }

        public double Y
        {
            get
            {
                return double.Parse(textBoxY.Text);
            }
        }

        public double Z
        {
            get
            {
                return double.Parse(textBoxZ.Text);
            }
        }

        void testInput()
        {
            float x = float.Parse(textBoxX.Text);
            float y = float.Parse(textBoxY.Text);
            float z = float.Parse(textBoxZ.Text);
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                testInput();
                this.DialogResult = true;
            }
            catch
            {
                e.Handled = false;
                MessageBox.Show("请输入合法数字");
            }
            try
            {
                preview(X, Y, Z);
            }
            catch
            {
                e.Handled = false;
                MessageBox.Show("平移失败");
            }
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
            try
            {
                preview(X, Y, Z);
            }
            catch
            {
                MessageBox.Show("预览失败");
            }
        }
    }
}

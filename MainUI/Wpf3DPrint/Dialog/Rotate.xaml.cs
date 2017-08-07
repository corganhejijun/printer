using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for Rotate.xaml
    /// </summary>
    public partial class Rotate : Window
    {
        MainWindow.TransformPreview preview;
        public Rotate(MainWindow.TransformPreview preview)
        {
            this.preview = preview;
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
                preview(XAngle, YAngle, ZAngle);
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
                preview(XAngle, YAngle, ZAngle);
            }
            catch
            {
                MessageBox.Show("请输入合法数字");
                return;
            }
        }
    }
}

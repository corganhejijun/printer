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
            }
            catch
            {
                e.Handled = false;
                MessageBox.Show("请输入合法数字");
            }
            try
            {
                preview(XAngle, YAngle, ZAngle);
            }
            catch
            {
                e.Handled = false;
                MessageBox.Show("旋转失败");
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
            try
            {
                preview(XAngle, YAngle, ZAngle);
            }
            catch
            {
                MessageBox.Show("预览失败");
            }
        }

        private void buttonRotate1_Click(object sender, RoutedEventArgs e)
        {
            textBoxY.Text = "90";
            checkBoxY.IsChecked = true;
            textBoxX.Text = "0";
            textBoxZ.Text = "0";
        }

        private void buttonRotate2_Click(object sender, RoutedEventArgs e)
        {
            textBoxY.Text = "90";
            checkBoxY.IsChecked = false;
            textBoxX.Text = "0";
            textBoxZ.Text = "0";
        }

        private void buttonRotate3_Click(object sender, RoutedEventArgs e)
        {
            textBoxX.Text = "90";
            checkBoxX.IsChecked = true;
            textBoxY.Text = "0";
            textBoxZ.Text = "0";
        }

        private void buttonRotate4_Click(object sender, RoutedEventArgs e)
        {
            textBoxX.Text = "90";
            checkBoxX.IsChecked = false;
            textBoxY.Text = "0";
            textBoxZ.Text = "0";
        }

        private void buttonRotate5_Click(object sender, RoutedEventArgs e)
        {
            textBoxZ.Text = "90";
            checkBoxZ.IsChecked = true;
            textBoxY.Text = "0";
            textBoxX.Text = "0";
        }

        private void buttonRotate6_Click(object sender, RoutedEventArgs e)
        {
            textBoxZ.Text = "90";
            checkBoxZ.IsChecked = false;
            textBoxY.Text = "0";
            textBoxX.Text = "0";
        }

        private void buttonRotate7_Click(object sender, RoutedEventArgs e)
        {
            textBoxZ.Text = "180";
            textBoxY.Text = "0";
            textBoxX.Text = "0";
        }

        private void buttonRotate8_Click(object sender, RoutedEventArgs e)
        {
            textBoxY.Text = "180";
            textBoxX.Text = "0";
            textBoxZ.Text = "0";
        }
    }
}

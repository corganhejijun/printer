using Microsoft.Win32;
using System.Windows;

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

        private void buttonAdd_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            if (dialog.ShowDialog() == false)
                return;
            string path = dialog.FileName;
            labelAdd.Content = path;
        }
    }
}

using Microsoft.Win32;
using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for EntityEdit.xaml
    /// </summary>
    public partial class EntityEdit : Window
    {
        public EntityEdit()
        {
            InitializeComponent();
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

        public bool Combine
        {
            get
            {
                return (bool)checkBoxCombine.IsChecked;
            }
        }

        public string FileName
        {
            get
            {
                return labelAdd.Content.ToString();
            }
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
                if (labelAdd.Content.ToString().Length == 0)
                {
                    MessageBox.Show("请选择要添加的图形文件");
                    return;
                }
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
            dialog.DefaultExt = ".stp";
            dialog.Filter = "STEP file (*.stp;*.step)|*.stp;*.step";
            if (dialog.ShowDialog() == false)
                return;
            string path = dialog.FileName;
            labelAdd.Content = path;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
        }
    }
}

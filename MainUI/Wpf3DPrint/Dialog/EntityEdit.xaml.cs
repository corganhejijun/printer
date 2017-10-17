using Microsoft.Win32;
using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for EntityEdit.xaml
    /// </summary>
    public partial class EntityEdit : Window
    {
        enum Phase {
            Postion, Entity, Edit
        }
        Phase phase;
        public EntityEdit()
        {
            InitializeComponent();
            phase = Phase.Postion;
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

        private void checkBoxManual_Checked(object sender, RoutedEventArgs e)
        {
            checkBoxAuto.IsChecked = !checkBoxManual.IsChecked;
        }

        private void checkBoxAuto_Checked(object sender, RoutedEventArgs e)
        {
            try
            {
                checkBoxManual.IsChecked = !checkBoxAuto.IsChecked;
            }
            catch
            {
                return;
            }
        }

        void setEntityPhase()
        {
            textBoxX.IsEnabled = false;
            textBoxY.IsEnabled = false;
            textBoxZ.IsEnabled = false;
            buttonPrev.Visibility = Visibility.Visible;
            checkBoxCenter.IsEnabled = true;
            buttonAdd.IsEnabled = true;
            checkBoxAuto.IsEnabled = false;
            checkBoxManual.IsEnabled = false;
            checkBoxCombine.IsEnabled = false;
            checkBoxUncombine.IsEnabled = false;
            comboBox.IsEnabled = false;
            phase = Phase.Entity;
        }

        void setPostionPhase()
        {
            textBoxX.IsEnabled = true;
            textBoxY.IsEnabled = true;
            textBoxZ.IsEnabled = true;
            buttonPrev.Visibility = Visibility.Hidden;
            checkBoxCenter.IsEnabled = false;
            buttonAdd.IsEnabled = false;
            phase = Phase.Postion;
        }

        void setEditPhase()
        {
            checkBoxCenter.IsEnabled = false;
            buttonAdd.IsEnabled = false;
            checkBoxAuto.IsEnabled = true;
            checkBoxManual.IsEnabled = true;
            checkBoxCombine.IsEnabled = true;
            checkBoxUncombine.IsEnabled = true;
            comboBox.IsEnabled = true;
            phase = Phase.Edit;
        }

        private void buttonOk_Click(object sender, RoutedEventArgs e)
        {
            if (phase == Phase.Postion)
            {
                setEntityPhase();
                e.Handled = false;
                try
                {
                    float x = float.Parse(textBoxX.Text);
                    float y = float.Parse(textBoxY.Text);
                    float z = float.Parse(textBoxZ.Text);
                }
                catch
                {
                    MessageBox.Show("请输入合法内容");
                    phase = Phase.Postion;
                    setPostionPhase();
                    e.Handled = false;
                }
                return;
            }
            else if (phase == Phase.Entity)
            {
                setEditPhase();
                if (labelAdd.Content.ToString().Length == 0)
                {
                    MessageBox.Show("请选择要添加的图形文件");
                    phase = Phase.Entity;
                    setEntityPhase();
                    return;
                }
                e.Handled = false;
                return;
            }
            this.DialogResult = true;
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

        private void buttonPrev_Click(object sender, RoutedEventArgs e)
        {
            if (phase == Phase.Entity)
            {
                setPostionPhase();
            }
            else if (phase == Phase.Edit)
            {
                setEntityPhase();
            }
        }
    }
}

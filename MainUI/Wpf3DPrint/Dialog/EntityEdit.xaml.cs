﻿using Microsoft.Win32;
using System.Windows;
using Wpf3DPrint.Viewer;

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
        public EntityEdit(Shape shape, string unit)
        {
            double Xmin = double.MaxValue, Xmax = double.MinValue, Ymin = double.MaxValue, Ymax = double.MinValue, Zmin = double.MaxValue, Zmax = double.MinValue;
            if (!Cpp2Managed.Shape3D.getBoundary(shape.getShape(), ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax))
            {
                Xmin = 0; Xmax = 0; Ymin = 0; Ymax = 0; Zmin = 0; Zmax = 0;
            }
            InitializeComponent();
            phase = Phase.Postion;
            labelUnitX.Content = unit;
            labelUnitXCenter.Content = unit;
            labelUnitXRange.Content = unit;
            labelUnitY.Content = unit;
            labelUnitYCenter.Content = unit;
            labelUnitYRange.Content = unit;
            labelUnitZ.Content = unit;
            labelUnitZCenter.Content = unit;
            labelUnitZRange.Content = unit;
            TextBoxXCenter.Text = ((Xmax + Xmin) / 2).ToString("0.00");
            TextBoxXRange.Text = Xmin.ToString("0.00") + "~" + Xmax.ToString("0.00");
            TextBoxYCenter.Text = ((Ymax + Ymin) / 2).ToString("0.00");
            TextBoxYRange.Text = Ymin.ToString("0.00") + "~" + Ymax.ToString("0.00");
            TextBoxZCenter.Text = ((Zmax + Zmin) / 2).ToString("0.00");
            TextBoxZRange.Text = Zmin.ToString("0.00") + "~" + Zmax.ToString("0.00");
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

        private void checkBoxCenter_Checked(object sender, RoutedEventArgs e)
        {
            if (checkBoxCenter == null || CheckBoxCustomPos == null)
                return;
            CheckBoxCustomPos.IsChecked = !checkBoxCenter.IsChecked;
            if (checkBoxCenter.IsChecked == true)
            {
                textBoxX.Text = "0";
                textBoxX.IsEnabled = false;
                textBoxY.Text = "0";
                textBoxY.IsEnabled = false;
                textBoxZ.Text = "0";
                textBoxZ.IsEnabled = false;
            }
        }

        private void CheckBoxCustomPos_Checked(object sender, RoutedEventArgs e)
        {
            if (checkBoxCenter == null || CheckBoxCustomPos == null)
                return;
            checkBoxCenter.IsChecked = !CheckBoxCustomPos.IsChecked;
            if (checkBoxCenter.IsChecked == false)
            {
                textBoxX.IsEnabled = true;
                textBoxY.IsEnabled = true;
                textBoxZ.IsEnabled = true;
            }
        }
    }
}

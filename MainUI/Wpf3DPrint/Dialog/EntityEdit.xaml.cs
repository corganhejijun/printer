using System.Windows;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for EntityEdit.xaml
    /// </summary>
    public partial class EntityEdit : Window
    {
        Shape shape;
        public EntityEdit(string unit, Shape shape)
        {
            this.shape = shape;
            InitializeComponent();
            LabelUnitX.Content = LabelUnitY.Content = LabelUnitZ.Content = LabelUnit.Content = unit;
            if (shape.FileName.Length > 0)
                ComboBoxSelectEntity.Items.Add(shape.FileName);
            if (shape.MoreFileName.Length > 0)
                ComboBoxSelectEntity.Items.Add(shape.MoreFileName);
            if (ComboBoxSelectEntity.Items.Count > 0)
            {
                ComboBoxSelectEntity.SelectedIndex = 0;
            }
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {

        }

        private void RadioButtonManualArrange_Checked(object sender, RoutedEventArgs e)
        {
            TextBoxX.IsEnabled = TextBoxY.IsEnabled = TextBoxZ.IsEnabled = ComboBoxSelectEntity.IsEnabled = true;
        }

        private void RadioButtonManualArrange_Unchecked(object sender, RoutedEventArgs e)
        {
            TextBoxX.IsEnabled = TextBoxY.IsEnabled = TextBoxZ.IsEnabled = ComboBoxSelectEntity.IsEnabled = false;
        }
    }
}

using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for Rebuild.xaml
    /// </summary>
    public partial class Rebuild : Window
    {
        public Rebuild()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
    }
}

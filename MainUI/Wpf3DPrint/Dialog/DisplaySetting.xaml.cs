using System.Windows;
using System.Windows.Media;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for DisplaySetting.xaml
    /// </summary>
    public partial class DisplaySetting : Window
    {
        public DisplaySetting()
        {
            InitializeComponent();
            textBoxColor.Background = new SolidColorBrush(Color.FromArgb(255, 156, 156, 156));
            r = 156;
            g = 156;
            b = 156;
        }

        int r, g, b;

        public int ColorR
        {
            get
            {
                return r;
            }
        }

        public int ColorG
        {
            get
            {
                return g;
            }
        }

        public int ColorB
        {
            get
            {
                return b;
            }
        }

        private void buttonSelectColor_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.ColorDialog clDlg = new System.Windows.Forms.ColorDialog();
            if (clDlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                return;
            textBoxColor.Background = new SolidColorBrush(Color.FromRgb(clDlg.Color.R, clDlg.Color.G, clDlg.Color.B));
            textBoxColor.Text = "(" + clDlg.Color.R + "," + clDlg.Color.G + "," + clDlg.Color.B + ")";
            r = clDlg.Color.R;
            g = clDlg.Color.G;
            b = clDlg.Color.B;
        }
    }
}

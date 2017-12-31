using System.Windows;
using System.Windows.Media;
using System.Windows.Forms;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for DisplaySetting.xaml
    /// </summary>
    public partial class DisplaySetting : Window
    {
        Color entityColor;
        Color lineColor;
        Color selectEntityColor;
        Color selectLineColor;
        public DisplaySetting(Viewer.Setting setting)
        {
            InitializeComponent();
            entityColor = setting.entityColor;
            setTextBoxEntityColor();
            lineColor = setting.lineColor;
            setTextBoxLineColor();
            selectEntityColor = setting.selectEntityColor;
            setTextBoxSelectEntityColor();
            selectLineColor = setting.selectLineColor;
            setTextBoxSelectLineColor();
        }

        public Color EntityColor
        {
            get {
                return entityColor;
            }
        }

        private void button_OK_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private void buttonEntityColor_Click(object sender, RoutedEventArgs e)
        {
            ColorDialog clDlg = new ColorDialog();
            if (clDlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                return;
            entityColor = Color.FromArgb(255, clDlg.Color.R, clDlg.Color.G, clDlg.Color.B);
            setTextBoxEntityColor();
        }

        void setTextBoxEntityColor()
        {
            textBoxEntityColor.Background = new SolidColorBrush(entityColor);
            textBoxEntityColor.Text = "(" + entityColor.R + "," + entityColor.G + "," + entityColor.B + ")";
        }

        void setTextBoxLineColor()
        {
            textBoxLineColor.Background = new SolidColorBrush(lineColor);
            textBoxLineColor.Text = "(" + lineColor.R + "," + lineColor.G + "," + lineColor.B + ")";
        }

        void setTextBoxSelectEntityColor()
        {
            textBoxSelectEntityColor.Background = new SolidColorBrush(selectEntityColor);
            textBoxSelectEntityColor.Text = "(" + selectEntityColor.R + "," + selectEntityColor.G + "," + selectEntityColor.B + ")";
        }

        void setTextBoxSelectLineColor()
        {
            textBoxSelectLineColor.Background = new SolidColorBrush(selectLineColor);
            textBoxSelectLineColor.Text = "(" + selectLineColor.R + "," + selectLineColor.G + "," + selectLineColor.B + ")";
        }
    }
}

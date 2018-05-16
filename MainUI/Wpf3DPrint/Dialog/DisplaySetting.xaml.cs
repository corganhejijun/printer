using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

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
            get
            {
                return entityColor;
            }
        }

        public Color LineColor
        {
            get
            {
                return lineColor;
            }
        }

        public Color SelectEntityColor
        {
            get
            {
                return selectEntityColor;
            }
        }

        public Color SelectLineColor
        {
            get
            {
                return selectLineColor;
            }
        }

        private void button_OK_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }

        private void buttonEntityColor_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.ColorDialog clDlg = new System.Windows.Forms.ColorDialog();
            if (clDlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                return;
            Button btn = (Button)sender;
            if (btn.Name.Equals(buttonEntityColor.Name))
            {
                entityColor = Color.FromArgb(255, clDlg.Color.R, clDlg.Color.G, clDlg.Color.B);
                setTextBoxEntityColor();
            }
            else if (btn.Name.Equals(buttonLineColor.Name))
            {
                lineColor = Color.FromArgb(255, clDlg.Color.R, clDlg.Color.G, clDlg.Color.B);
                setTextBoxLineColor();
            }
            else if (btn.Name.Equals(buttonEntitySelectColor.Name))
            {
                selectEntityColor = Color.FromArgb(255, clDlg.Color.R, clDlg.Color.G, clDlg.Color.B);
                setTextBoxSelectEntityColor();
            }
            else if (btn.Name.Equals(buttonLineSelectColor.Name))
            {
                selectLineColor = Color.FromArgb(255, clDlg.Color.R, clDlg.Color.G, clDlg.Color.B);
                setTextBoxSelectLineColor();
            }
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

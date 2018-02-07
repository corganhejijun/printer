using System.Reflection;
using System.Windows;

namespace Wpf3DPrint.Dialog
{
    /// <summary>
    /// Interaction logic for About.xaml
    /// </summary>
    public partial class About : Window
    {
        public About()
        {
            InitializeComponent();
            var version = Assembly.GetExecutingAssembly().GetName().Version;
            labelVersion.Content = "版本生成时间：" + System.IO.File.GetLastWriteTime(this.GetType().Assembly.Location).ToString();
        }

        public string Version
        {
            get
            {
                return labelVersion.Content.ToString();
            }
        }
    }
}

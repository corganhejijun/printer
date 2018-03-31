using System;
using System.Windows;
using System.Windows.Threading;

namespace Wpf3DPrint
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        public App()
        {
            DispatcherUnhandledException += App_DispatcherUnhandledException;
        }

        void App_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            MessageBox.Show("程序发生异常." + Environment.NewLine + e.Exception.Message + Environment.NewLine + e.Exception.StackTrace);
            e.Handled = true;
        }
    }
}

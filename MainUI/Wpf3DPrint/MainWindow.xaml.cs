using Microsoft.Win32;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private Scene scene;
        private FileReader fileReader;

        public MainWindow()
        {
            InitializeComponent();
            scene = new Scene();
            fileReader = new FileReader(scene);
            ImageBrush brush = new ImageBrush(scene.Image);
            brush.RelativeTransform = new ScaleTransform(1.0, -1.0, 0.5, 0.5);
            GridScene.Background = brush;
        }

        private void buttonSelectMode_Click(object sender, RoutedEventArgs e)
        {
            buttonSelectMode.Background = Brushes.LightSkyBlue;
            buttonWatchMode.Background = Brushes.Transparent;
        }

        private void buttonWatchMode_Click(object sender, RoutedEventArgs e)
        {
            buttonSelectMode.Background = Brushes.Transparent;
            buttonWatchMode.Background = Brushes.LightSkyBlue;
        }

        private void onOpeningFile(string fileName)
        {
            GridOnReading.Visibility = Visibility.Visible;
            labelOnReading.Content = "Reading file " + fileName + " ...";
            buttonOpen.IsEnabled = false;
            buttonSave.IsEnabled = false;
        }

        private void afterOpenFile()
        {
            GridOnReading.Visibility = Visibility.Hidden;
            buttonOpen.IsEnabled = true;
            buttonSave.IsEnabled = true;
        }

        private void buttonOpen_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".stp";
            openFile.Filter = "STEP file (*.stp;*.step)|*.stp;*.step";
            if (openFile.ShowDialog() == false)
                return;
            if (!fileReader.openStep(openFile.FileName, afterOpenStep))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(openFile.FileName);
        }

        private void afterOpenStep(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(displayStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void displayStep(object workResult)
        {
            fileReader.displayStep(workResult);
            fileReader.afterOpenFile();
            afterOpenFile();
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "stp";
            saveFile.Filter = "STEP file (*.stp;*.step)|*.stp;*.step";
            if (false == saveFile.ShowDialog())
                return;
        }

        private void buttonSlice_Click(object sender, RoutedEventArgs e)
        {
            buttonSlice.IsEnabled = false;
            fileReader.sliceShape((Control)this, onAfterSlice);
        }

        private void onAfterSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(afterSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void afterSlice(object args)
        {
            buttonSlice.IsEnabled = true;
        }

        private void GridScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            scene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            fileReader.releaseShape();
            scene.Dispose();
        }

        bool beginRotate = false;
        private void GridScene_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (beginRotate)
            {
                return;
            }
            beginRotate = true;
            Point p = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
            scene.Proxy.StartRotation((int)p.X, (int)p.Y);
        }

        private void GridScene_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            beginRotate = false;
        }

        private void GridScene_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (beginRotate)
            {
                Point p = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
                scene.Proxy.Rotation((int)p.X, (int)p.Y);
                scene.Proxy.RedrawView();
            }
        }

        private void GridScene_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            beginRotate = false;
        }
    }
}

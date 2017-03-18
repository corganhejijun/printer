using Microsoft.Win32;
using System;
using System.Collections;
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
        private Scene sliceScene;
        private FileReader fileReader;

        public MainWindow()
        {
            InitializeComponent();
            scene = new Scene();
            sliceScene = new Scene();
            fileReader = new FileReader(scene);
            ImageBrush brush = new ImageBrush(scene.Image);
            ImageBrush sliceBrush = new ImageBrush(sliceScene.Image);
            brush.RelativeTransform = new ScaleTransform(1.0, -1.0, 0.5, 0.5);
            sliceBrush.RelativeTransform = new ScaleTransform(1.0, -1.0, 0.5, 0.5);
            GridScene.Background = brush;
            GridSlice.Background = sliceBrush;
            fileReader.setSliceScene(sliceScene);
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
            if (fileReader.HasFile)
            {
                MessageBox.Show("请先关闭已打开的文件");
                return;
            }
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".stp";
            openFile.Filter = "STEP file (*.stp;*.step)|*.stp;*.step";
            if (openFile.ShowDialog() == false)
                return;
            if (!fileReader.openStep(openFile.FileName, afterOpenStep, false))
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
            fileReader.saveSlice(saveFile.FileName);
        }

        private void buttonSlice_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
                return;
            Dialog.DialogSlice dlSlice = new Dialog.DialogSlice(fileReader.Shape);
            if (dlSlice.ShowDialog() == false)
                return;
            buttonSlice.IsEnabled = false;
            textBoxSlice.Visibility = Visibility.Visible;
            fileReader.sliceShape((Control)this, onAfterSlice, new SceneThread.onFunction(onSlice));
            sliderSlice.Maximum = fileReader.Shape.sliceCnt - 1;
        }

        private void onSlice(object args)
        {
            ArrayList argList = (ArrayList)args;
            Shape shape = (Shape)argList[0];
            int currentSlice = (int)argList[1];
            textBoxSlice.Text = "总层数：" + shape.sliceCnt + " 当前层数：" + currentSlice;
        }

        private void onAfterSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(afterSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void afterSlice(object args)
        {
            buttonSlice.IsEnabled = true;
            textBoxSlice.Visibility = Visibility.Hidden;
        }

        private void GridScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            scene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
        }

        private void GridSlice_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            sliceScene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            fileReader.releaseShape();
            scene.Dispose();
            sliceScene.Dispose();
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

        private void buttonClose_Click(object sender, RoutedEventArgs e)
        {
            fileReader.releaseShape();
        }

        private void sliderSlice_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            fileReader.displaySlice((int)sliderSlice.Value);
        }

        private void buttonOpenSlice_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.HasFile)
            {
                MessageBox.Show("请先关闭已打开的文件");
                return;
            }
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".stp";
            openFile.Filter = "STEP file (*.stp;*.step)|*.stp;*.step";
            if (openFile.ShowDialog() == false)
                return;
            if (!fileReader.openStep(openFile.FileName, afterOpenSlice, true))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(openFile.FileName);
        }

        private void afterOpenSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(displaySlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void displaySlice(object workResult)
        {
            int count = fileReader.afterOpenSlice(workResult);
            sliderSlice.Maximum = count - 1;
            fileReader.afterOpenFile();
            afterOpenFile();
        }

        private void menuQuit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void menuCloseFile_Click(object sender, RoutedEventArgs e)
        {
            fileReader.releaseShape();
        }
    }
}

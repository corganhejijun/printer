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
        private Scene2D sliceScene;
        private FileReader fileReader;

        public MainWindow()
        {
            InitializeComponent();
            scene = new Scene();
            sliceScene = new Scene2D(PanelSlice);
            fileReader = new FileReader(scene);
            ImageBrush brush = new ImageBrush(scene.Image);
            brush.RelativeTransform = new ScaleTransform(1.0, -1.0, 0.5, 0.5);
            GridScene.Background = brush;
        }

        private void onOpeningFile(string fileName)
        {
            labelStatus.Content = "Reading file " + fileName + " ...";
        }

        private void afterOpenFile()
        {
            labelStatus.Content = "X:[" + fileReader.Shape.Xmin.ToString("0.000") + "," + fileReader.Shape.Xmax.ToString("0.000")
                + "]; Y:[" + fileReader.Shape.Ymin.ToString("0.000") + "," + fileReader.Shape.Ymax.ToString("0.000")
                + "]; Z:[" + fileReader.Shape.Zmin.ToString("0.000") + "," + fileReader.Shape.Zmax.ToString("0.000") + "]";
            textBoxXRange.Text = fileReader.Shape.Xmin.ToString("0.0000") + "," + fileReader.Shape.Xmax.ToString("0.0000");
            textBoxYRange.Text = fileReader.Shape.Ymin.ToString("0.0000") + "," + fileReader.Shape.Ymax.ToString("0.0000");
            textBoxZRange.Text = fileReader.Shape.Zmin.ToString("0.0000") + "," + fileReader.Shape.Zmax.ToString("0.0000");
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
            set3DView();
        }

        private void afterOpenStep(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(displayStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void displayStep(object workResult)
        {
            fileReader.displayStep(workResult);
            fileReader.afterOpenFile();
            Dialog.DialogUnit unit = new Dialog.DialogUnit(fileReader.Shape);
            unit.ShowDialog();
            afterOpenFile();
        }

        private void buttonSave_Click(object sender, RoutedEventArgs e)
        {
            saveSlice();
        }

        private void buttonSlice_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            Dialog.DialogSlice dlSlice = new Dialog.DialogSlice(fileReader.Shape, textBoxSliceThick.Text);
            if (dlSlice.ShowDialog() == false)
                return;
            textBoxSliceThick.Text = fileReader.Shape.sliceThick.ToString();
            fileReader.sliceShape((Control)this, onAfterSlice, new SceneThread.onFunction(onSlice));
        }

        private string saveSlice()
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "slc";
            saveFile.Filter = "Slice file (*.slc)|*.slc";
            if (false == saveFile.ShowDialog())
                return "";
            fileReader.saveSlice(saveFile.FileName);
            return saveFile.FileName;
        }

        private void onSlice(object args)
        {
            ArrayList argList = (ArrayList)args;
            Shape shape = (Shape)argList[0];
            int currentSlice = (int)argList[1];
            labelStatus.Content = "总层数：" + ((int)((shape.Zmax - shape.Zmin) / shape.sliceThick)) + " 当前层数：" + currentSlice;
        }

        private void onAfterSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(afterSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void afterSlice(object args)
        {
            string fileName = saveSlice();
            if (fileName.Length == 0)
                return;
            fileReader.releaseShape();
            openSlice(fileName);
        }

        private void GridScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            scene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
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
            sliceScene.closeSlice();
            sliceScene.clearWindow();
            TreeView_Slice.Items.Clear();
            set3DView();
        }

        private void buttonOpenSlice_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.HasFile)
            {
                MessageBox.Show("请先关闭已打开的文件");
                return;
            }
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".slc";
            openFile.Filter = "Slice file (*.slc)|*.slc";
            if (openFile.ShowDialog() == false)
                return;
            openSlice(openFile.FileName);
        }

        private void openSlice(string fileName)
        {
            if (!fileReader.openStep(fileName, afterOpen, true))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(fileName);
        }

        private void afterOpen(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(afterOpenSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void afterOpenSlice(object workResult)
        {
            int count = fileReader.afterOpenSlice(workResult);
            fileReader.displaySlice();
            sliceScene.slice(fileReader.Shape.stepSlice, count);
            afterOpenFile();
            ArrayList list = new ArrayList();
            for (int i = 0; i < count; i++)
            {
                list.Add(i + 1);
            }
            comboBoxSliceList.ItemsSource = list;
            comboBoxSliceNumber.SelectedItem = 1;
            sliceScene.drawSlice((int)comboBoxSliceNumber.SelectedItem - 1);
            setSliceView();
            TreeViewItem root = new TreeViewItem();
            root.Header = fileReader.FileName.Substring(fileReader.FileName.LastIndexOf('\\') + 1);
            root.ItemsSource = list;
            TreeView_Slice.SelectedItemChanged += TreeView_Slice_SelectedItemChanged;
            root.IsExpanded = true;
            TreeView_Slice.Items.Add(root);
            /*root = (TreeViewItem)TreeView_Slice.Items[0];
            TreeViewItem first = (TreeViewItem)root.Items[0];
            first.IsSelected = true;
            */
        }

        private void TreeView_Slice_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            try {
                int value = (int)e.NewValue;
                selectSlice(value - 1);
            }
            catch
            {
                return;
            }
        }

        private void set3DView()
        {
            column3D.Width = new GridLength(100, GridUnitType.Star);
            column2D.Width = new GridLength(0, GridUnitType.Star);
            columnTree.Width = new GridLength(0, GridUnitType.Star);
        }

        private void setSliceView()
        {
            column3D.Width = new GridLength(40, GridUnitType.Star);
            column2D.Width = new GridLength(50, GridUnitType.Star);
            columnTree.Width = new GridLength(10, GridUnitType.Star);
        }

        private void buttonQuit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void PanelSlice_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            if (comboBoxSliceNumber.SelectedItem != null)
                sliceScene.drawSlice((int)comboBoxSliceNumber.SelectedItem - 1);
        }

        private void PanelSlice_Resize(object sender, EventArgs e)
        {
            if (sliceScene != null)
                sliceScene.onResize();
        }

        private void ribbonMenu_Loaded(object sender, RoutedEventArgs e)
        {
            try {
                Grid child = VisualTreeHelper.GetChild((DependencyObject)sender, 0) as Grid;
                if (child != null)
                {
                    child.RowDefinitions[0].Height = new GridLength(0);
                }
            }
            catch
            {
                return;
            }
        }

        private void comboBoxSliceNumber_SelectionChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            int index = (int)comboBoxSliceNumber.SelectedItem - 1;
            selectSlice(index);
        }

        private void selectSlice(int index)
        {
            sliceScene.drawSlice(index);
            fileReader.selectSlice(index);
            //fileReader.rebuildSlice(index);
        }

        private void buttonRebuild_Click(object sender, RoutedEventArgs e)
        {
            fileReader.rebuildSlice(0);
        }
    }
}

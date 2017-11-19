using Microsoft.Win32;
using System;
using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
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
        private Scene slicingScene;
        private Scene2D sliceScene;
        private FileReader fileReader;

        Dialog.EntityEdit dlgEntityEdit;

        public MainWindow()
        {
            InitializeComponent();
            scene = new Scene();
            slicingScene = new Scene();
            sliceScene = new Scene2D(PanelSlice);
            fileReader = new FileReader(scene, slicingScene);
            ImageBrush brush = new ImageBrush(scene.Image);
            ImageBrush slicingBrush = new ImageBrush(slicingScene.Image);
            brush.RelativeTransform = new ScaleTransform(1.0, -1.0, 0.5, 0.5);
            slicingBrush.RelativeTransform = new ScaleTransform(1.0, -1.0, 0.5, 0.5);
            GridScene.Background = brush;
            GridSlice3D.Background = slicingBrush;
            dlgEntityEdit = null;
        }

        private void onOpeningFile(string fileName)
        {
            labelStatus.Content = "Reading file " + fileName + " ...";
        }

        private void afterOpenFile()
        {
            /*
            labelStatus.Content = "X:[" + fileReader.Shape.Xmin.ToString("0.000") + "," + fileReader.Shape.Xmax.ToString("0.000")
                + "]; Y:[" + fileReader.Shape.Ymin.ToString("0.000") + "," + fileReader.Shape.Ymax.ToString("0.000")
                + "]; Z:[" + fileReader.Shape.Zmin.ToString("0.000") + "," + fileReader.Shape.Zmax.ToString("0.000") + "]";
            textBoxXRange.Text = fileReader.Shape.Xmin.ToString("0.0000") + "," + fileReader.Shape.Xmax.ToString("0.0000");
            textBoxYRange.Text = fileReader.Shape.Ymin.ToString("0.0000") + "," + fileReader.Shape.Ymax.ToString("0.0000");
            textBoxZRange.Text = fileReader.Shape.Zmin.ToString("0.0000") + "," + fileReader.Shape.Zmax.ToString("0.0000");
            */
            labelStatus.Content = "";
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
            openFile.Filter = "STEP file (*.stp;*.step)|*.stp;*.step|STL Mesh(*.stl;*.ast)|*.stl;*.ast";
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

        private void afterImportMoreStep(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(displayImportMoreStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        string unit;
        private void displayStep(object workResult)
        {
            fileReader.displayStep(workResult, false);
            fileReader.resetView(1);
            Dialog.DialogUnit unit = new Dialog.DialogUnit(fileReader.Shape);
            unit.Owner = this;
            unit.ShowDialog();
            this.unit = unit.Unit;
            afterOpenFile();
        }

        private void displayImportMoreStep(object workResult)
        {
            fileReader.displayStep(workResult, dlgEntityEdit.Combine);
            fileReader.resetView(1);
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
            Dialog.DialogSlice dlSlice = new Dialog.DialogSlice(fileReader.Shape, textBoxSliceThick.Text, unit);
            dlSlice.Owner = this;
            if (dlSlice.ShowDialog() == false)
                return;
            setSlicingView();
            textBoxSliceThick.Text = fileReader.Shape.sliceThick.ToString();
            fileReader.sliceShape((Control)this, dlSlice.locatePlane, dlSlice.gradientShape, dlSlice.quickSlice, dlSlice.outputRatio, onAfterSlice, new SceneThread.onFunction(onSlice));
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
            int total = (int)((shape.Zmax - shape.Zmin) / shape.sliceThick);
            if (shape.countLocate)
                total += shape.locateCount;
            labelStatus.Content = "总层数：" + total + " 当前层数：" + shape.sliceList.Count;
            if (shape.sliceList.Count == 1)
                fileReader.resetView(shape.outputRatio);
        }

        private void onAfterSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(afterSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void afterSlice(object args)
        {
            Shape shape = fileReader.Shape;
            fileReader.resetView(shape.outputRatio);
            string fileName = saveSlice();
            if (fileName.Length == 0)
                return;
            fileReader.releaseShape();
            openSlice(fileName);
        }

        private void GridScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            scene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
            slicingScene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            fileReader.releaseShape();
            scene.Dispose();
            slicingScene.Dispose();
            sliceScene.Dispose();
        }

        bool beginRotate = false;
        Point mouseOrigin;
        private void GridScene_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            mouseOrigin = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
            if (beginRotate)
            {
                return;
            }
            beginRotate = true;
            Point p = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
            scene.Proxy.StartRotation((int)p.X, (int)p.Y);
            slicingScene.Proxy.StartRotation((int)p.X, (int)p.Y);
        }

        private void GridScene_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            beginRotate = false;
        }

        private void GridScene_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (beginRotate)
            {
                float ratio = 1;
                Point p = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
                if ((bool)ButtonRoll.IsChecked)
                    scene.Proxy.Rotation((int)p.X, (int)p.Y);
                if ((bool)ButtonPan.IsChecked)
                    scene.Proxy.Pan(-(int)((mouseOrigin.X - p.X)/ratio), (int)((mouseOrigin.Y - p.Y)/ratio));
                scene.Proxy.RedrawView();
                if ((bool)ButtonRoll.IsChecked)
                    slicingScene.Proxy.Rotation((int)p.X, (int)p.Y);
                if ((bool)ButtonPan.IsChecked)
                    slicingScene.Proxy.Pan(-(int)((mouseOrigin.X - p.X)/ratio), (int)((mouseOrigin.Y - p.Y)/ratio));
                slicingScene.Proxy.RedrawView();
                mouseOrigin = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
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
            set3DFunction(true);
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
            foreach (int i in list)
            {
                TreeViewItem item = new TreeViewItem();
                item.Header = i;
                root.Items.Add(item);
            }
            TreeView_Slice.SelectedItemChanged += TreeView_Slice_SelectedItemChanged;
            root.IsExpanded = true;
            TreeView_Slice.Items.Add(root);
            set3DFunction(false);
        }

        void set3DFunction(bool enable)
        {
            ButtonRotate.IsEnabled = enable;
            ButtonMove.IsEnabled = enable;
            menuRotate.IsEnabled = enable;
            menuPan.IsEnabled = enable;
            menuBase0.IsEnabled = enable;
            menuBase0xy.IsEnabled = enable;
            ButtonSlice.IsEnabled = enable;
            menuEntityEdit.IsEnabled = enable;
            menuSlice.IsEnabled = enable;
        }

        private void TreeView_Slice_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            try {
                int value = (int)((TreeViewItem)e.NewValue).Header;
                selectSlice(value - 1);
                if ((int)comboBoxSliceNumber.SelectedItem != value)
                    comboBoxSliceNumber.SelectedItem = value;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
        }

        private void set3DView()
        {
            column3D.Width = new GridLength(100, GridUnitType.Star);
            column2D.Width = new GridLength(0, GridUnitType.Star);
            columnTree.Width = new GridLength(0, GridUnitType.Star);
            treeViewSplitter.Width = 0;
            sliceSplitter.Width = 0;
        }

        private void setSlicingView()
        {
            column3D.Width = new GridLength(50, GridUnitType.Star);
            column2D.Width = new GridLength(50, GridUnitType.Star);
            columnTree.Width = new GridLength(0, GridUnitType.Star);
            columnSlice2D.Width = new GridLength(0, GridUnitType.Star);
            columnSlice3D.Width = new GridLength(100, GridUnitType.Star);
            sliceSplitter.Width = 5;
        }

        private void setSliceView()
        {
            column3D.Width = new GridLength(40, GridUnitType.Star);
            column2D.Width = new GridLength(50, GridUnitType.Star);
            columnTree.Width = new GridLength(10, GridUnitType.Star);
            columnSlice2D.Width = new GridLength(100, GridUnitType.Star);
            columnSlice3D.Width = new GridLength(0, GridUnitType.Star);
            sliceSplitter.Width = 5;
            treeViewSplitter.Width = 5;
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
            int index = (int)comboBoxSliceNumber.SelectedItem;
            if (TreeView_Slice.Items.Count > 0)
            {
                TreeViewItem root = (TreeViewItem)TreeView_Slice.Items[0];
                foreach (TreeViewItem item in root.Items)
                {
                    if ((int)item.Header == index)
                    {
                        if (!item.IsSelected)
                            item.IsSelected = true;
                        break;
                    }
                }
            }
            selectSlice(index - 1);
        }

        private void selectSlice(int index)
        {
            sliceScene.drawSlice(index);
            fileReader.selectSlice(index);
        }

        private void buttonRebuild_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            Dialog.Rebuild rebuild = new Dialog.Rebuild();
            rebuild.Owner = this;
            if (rebuild.ShowDialog() == false)
                return;
            setSlicingView();
            fileReader.rebuildSlice();
        }

        private void GridScene_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            if (Math.Abs(e.Delta) > 8)
            {
                fileReader.sceneZoom(e.Delta);
            }
        }

        public delegate void TransformPreview(double x, double y, double z);

        private void menuRotate_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            // TODO 切片之前的实体才能旋转，切片中和切片后不能旋转
            TransformPreview preview = new TransformPreview(rotatePreview);
            Dialog.Rotate rotate = new Dialog.Rotate(preview);
            rotate.Owner = this;
            if (rotate.ShowDialog() == false)
            {
                fileReader.releaseTransform(fileReader.Shape);
                fileReader.displayAfterTransform(fileReader.Shape);
                return;
            }
            fileReader.applyTransform(fileReader.Shape);
        }

        void rotatePreview(double xAngle, double yAngle, double zAngle)
        {
            fileReader.rotateAllShape(xAngle, yAngle, zAngle);
        }

        private void menuEntityEdit_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (dlgEntityEdit != null)
                dlgEntityEdit = null;
            dlgEntityEdit = new Dialog.EntityEdit();
            dlgEntityEdit.Owner = this;
            if (dlgEntityEdit.ShowDialog() == false)
                return;
            if (!fileReader.importMoreStep(dlgEntityEdit.FileName, afterImportMoreStep))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(dlgEntityEdit.FileName);
            set3DView();
        }

        private void menuAbout_Click(object sender, RoutedEventArgs e)
        {
            Dialog.About about = new Dialog.About();
            about.Owner = this;
            about.ShowDialog();
        }

        private void menuEntityProp_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            Dialog.EntityProp entity = new Dialog.EntityProp(fileReader.Shape, unit);
            entity.Owner = this;
            if (entity.ShowDialog() == false)
                return;
        }

        private void menu2DProc_Click(object sender, RoutedEventArgs e)
        {
            Dialog.Param2D d2 = new Dialog.Param2D();
            d2.Owner = this;
            if (d2.ShowDialog() == false)
                return;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
        }

        private void menuPan_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            TransformPreview preview = new TransformPreview(panPreview);
            Dialog.Pan pan = new Dialog.Pan(unit, preview);
            pan.Owner = this;
            if (pan.ShowDialog() == false)
            {
                fileReader.releaseTransform(fileReader.Shape);
                fileReader.displayAfterTransform(fileReader.Shape);
                return;
            }
            fileReader.applyTransform(fileReader.Shape);
        }

        void panPreview(double x, double y, double z)
        {
            fileReader.moveAllShape(x, y, z);
        }

        private void buttonSaveAs_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "step";
            saveFile.Filter = "Step file (*.step)|*.step";
            if (false == saveFile.ShowDialog())
                return ;
            fileReader.saveStep(saveFile.FileName, fileReader.Shape);
        }

        private void ButtonRoll_Click(object sender, RoutedEventArgs e)
        {
            if ((bool)ButtonPan.IsChecked)
                ButtonPan.IsChecked = false;
            ButtonRoll.IsChecked = true;
        }

        private void ButtonPan_Click(object sender, RoutedEventArgs e)
        {
            if ((bool)ButtonRoll.IsChecked)
                ButtonRoll.IsChecked = false;
            ButtonPan.IsChecked = true;

        }

        private void ButtonPan_Checked(object sender, RoutedEventArgs e)
        {
            if (GridScene != null && (bool)ButtonPan.IsChecked)
                GridScene.Cursor = Cursors.Hand;
        }

        private void ButtonRoll_Checked(object sender, RoutedEventArgs e)
        {
            if (GridScene != null && (bool)ButtonRoll.IsChecked)
                GridScene.Cursor = Cursors.Arrow;
        }

        private void menuBase0_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            fileReader.base0AllShapes();
        }


        private void menuBase0xy_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            fileReader.base0XyCenter();
        }

        private void buttonSaveDxf_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.HasFile)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (fileReader.Shape.sliceList.Count == 0)
            {
                MessageBox.Show("没有切片");
                return;
            }
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "rp";
            saveFile.Filter = "RP file (*.rp)|*.rp";
            if (false == saveFile.ShowDialog())
                return;
            DxfWriter writer = new DxfWriter(saveFile.FileName, fileReader.Shape);
            writer.writeSlice();
            writer.Dispose();
            if (MessageBox.Show("保存完成, 是否使用RP软件打开?", "OK", MessageBoxButton.OKCancel) != MessageBoxResult.OK)
                return;
            System.Diagnostics.Process exep = new System.Diagnostics.Process();
            exep.StartInfo.FileName = "RP.exe";
            exep.StartInfo.Arguments = "-f " + saveFile.FileName;
            exep.Start();
        }
    }
}

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
            fileReader = new FileReader();
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
            this.Title = "3DLT  " + fileReader.Shape.fileName;
        }

        private void buttonOpen_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("请先关闭已打开的文件");
                return;
            }
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".stp";
            openFile.Filter = "STEP file (*.stp;*.step)|*.stp;*.step|STL Mesh(*.stl;*.ast)|*.stl;*.ast|Dxf file|*.dxf";
            if (openFile.ShowDialog() == false)
                return;
            if (!fileReader.openStep(openFile.FileName, afterOpenStep))
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

        string unit;
        private void displayStep(object workResult)
        {
            scene.displayShape(fileReader.Shape.getShape());
            resetView(1);
            /*
            Dialog.DialogUnit unit = new Dialog.DialogUnit(fileReader.Shape);
            unit.Owner = this;
            unit.ShowDialog();
            this.unit = unit.Unit;
            */
            afterOpenFile();
        }

        private void buttonSaveSlice_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.slice.sliceList.Count == 0)
            {
                MessageBox.Show("没有切片文件需要保存");
                return;
            }
            saveSlice();
        }

        private void buttonSlice_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (fileReader.Shape.HasMoreShape)
            {
                MessageBox.Show("请先合并多个图形");
                return;
            }
            Dialog.DialogSlice dlSlice = new Dialog.DialogSlice(fileReader.Shape, textBoxSliceThick.Text, unit);
            dlSlice.Owner = this;
            if (dlSlice.ShowDialog() == false)
                return;
            setSlicingView();
            fileReader.Shape.slice.sliceThick = dlSlice.sliceThick;
            textBoxSliceThick.Text = fileReader.Shape.slice.sliceThick.ToString();
            fileReader.sliceShape((Control)this, dlSlice.locatePlane, dlSlice.gradientShape, dlSlice.quickSlice, onAfterSlice, new SceneThread.onFunction(onSlice));
            ToolBarOper.IsEnabled = false;
            ToolBarReset.IsEnabled = false;
            MenuMain.IsEnabled = false;
        }

        private string saveSlice()
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "slc";
            saveFile.Filter = "Slice file (*.slc)|*.slc";
            if (false == saveFile.ShowDialog(this))
                return "";
            fileReader.saveSlice(saveFile.FileName);
            this.Title = fileReader.Shape.fileName;
            return saveFile.FileName;
        }

        private void onSlice(object args)
        {
            ArrayList argList = (ArrayList)args;
            IntPtr slice = (IntPtr)argList[0];
            double height = (double)argList[1];
            int totalSliceCnt = (int)argList[2];
            slicingScene.displaySlice(slice);
            scene.displaySliceCut(fileReader.Shape.getShape(), height);
            Shape shape = fileReader.Shape;
            labelStatus.Content = "总层数：" + totalSliceCnt + " 当前层数：" + shape.slice.sliceList.Count;
            /*
            if (shape.sliceList.Count == 1)
                fileReader.resetView(shape.outputRatio);
                */
        }

        private void onAfterSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(afterSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void afterSlice(object args)
        {
            ToolBarOper.IsEnabled = true;
            ToolBarReset.IsEnabled = true;
            MenuMain.IsEnabled = true;
            Shape shape = fileReader.Shape;
            //fileReader.resetView(shape.outputRatio);
            string fileName = saveSlice();
            if (fileName.Length == 0)
                return;
            fileReader.Shape.release();
            scene.Proxy.cleanScene();
            slicingScene.Proxy.cleanScene();
            openSlice(fileName);
        }

        private void GridScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            scene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
            slicingScene.Resize(Convert.ToInt32(e.NewSize.Width), Convert.ToInt32(e.NewSize.Height));
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            fileReader.Shape.release();
            scene.Proxy.cleanScene();
            slicingScene.Proxy.cleanScene();
            fileReader.Dispose();
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
            scene.Proxy.MoveTo((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
            scene.Proxy.Select();
        }

        private void GridScene_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (!(bool)ButtonSelect.IsChecked)
                return;
            IntPtr selected = scene.select(e.GetPosition(GridScene).X, e.GetPosition(GridScene).Y);
            if (selected == IntPtr.Zero)
                return;
            foreach (IntPtr shape in fileReader.Shape.selectList)
            {
                if (scene.Proxy.IsEqual(shape, selected))
                {
                    fileReader.Shape.selectList.Remove(shape);
                    scene.displayShape(shape);
                    return;
                }
            }
            fileReader.Shape.selectList.Add(selected);
            scene.displaySelectShape(selected);
        }

        private void menuUnselect_Click(object sender, RoutedEventArgs e)
        {
            foreach(IntPtr shape in fileReader.Shape.selectList)
            {
                scene.displayShape(shape);
            }
            fileReader.Shape.selectList.Clear();
        }

        private void GridScene_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            beginRotate = false;
        }

        private void buttonClose_Click(object sender, RoutedEventArgs e)
        {
            scene.Proxy.cleanScene();
            slicingScene.Proxy.cleanScene();
            sliceScene.closeSlice();
            sliceScene.clearWindow();
            TreeView_Slice.Items.Clear();
            fileReader.Shape.release();
            set3DView();
            set3DFunction(true);
        }

        private void buttonOpenSlice_Click(object sender, RoutedEventArgs e)
        {
            if (!fileReader.Shape.IsEmpty)
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
            if (!fileReader.openSlice(fileName, afterOpen))
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
            foreach (Slice.OneSlice slice in fileReader.Shape.slice.sliceList)
            {
                scene.displaySlice(slice.slice);
            }
            resetView(1);
            afterOpenFile();
            ArrayList list = new ArrayList();
            for (int i = 0; i < fileReader.Shape.slice.sliceList.Count; i++)
            {
                list.Add(i + 1);
            }
            comboBoxSliceList.ItemsSource = list;
            comboBoxSliceNumber.SelectedItem = 1;
            sliceScene.drawSlice((Slice.OneSlice)(fileReader.Shape.slice.sliceList[(int)comboBoxSliceNumber.SelectedItem - 1]));
            setSliceView();
            TreeViewItem root = new TreeViewItem();
            root.Header = fileReader.Shape.fileName.Substring(fileReader.Shape.fileName.LastIndexOf('\\') + 1);
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
            menuMove.IsEnabled = enable;
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
                sliceScene.drawSlice((Slice.OneSlice)(fileReader.Shape.slice.sliceList[(int)comboBoxSliceNumber.SelectedItem - 1]));
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
            sliceScene.drawSlice((Slice.OneSlice)(fileReader.Shape.slice.sliceList[index]));
            scene.selectSlice(((Slice.OneSlice)fileReader.Shape.slice.sliceList[index]).slice);
        }

        System.Windows.Forms.Timer rebuildTimer;
        int rebuildIndex;
        public void rebuildSlice()
        {
            if (rebuildTimer == null)
                rebuildTimer = new System.Windows.Forms.Timer();
            rebuildTimer.Tick += RebuildTimer_Elapsed;
            rebuildTimer.Interval = 100;
            slicingScene.Proxy.cleanScene();
            rebuildIndex = fileReader.Shape.slice.sliceList.Count;
            rebuildTimer.Start();
        }

        private void RebuildTimer_Elapsed(object sender, EventArgs e)
        {
            if (rebuildIndex == 0)
            {
                rebuildTimer.Stop();
                rebuildTimer.Dispose();
                rebuildTimer = null;
                return;
            }
            Slice.OneSlice slice = (Slice.OneSlice)fileReader.Shape.slice.sliceList[rebuildIndex - 1];
            slicingScene.Proxy.strechSlice(slice.slice, fileReader.Shape.slice.sliceThick);
            rebuildIndex--;
        }

        private void buttonRebuild_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.slice.sliceList.Count == 0)
            {
                MessageBox.Show("未打开切片文件");
                return;
            }
            Dialog.Rebuild rebuild = new Dialog.Rebuild();
            rebuild.Owner = this;
            if (rebuild.ShowDialog() == false)
                return;
            setSlicingView();
            rebuildSlice();
        }

        private void GridScene_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            if (Math.Abs(e.Delta) > 8)
            {
                scene.zoom(e.Delta);
                slicingScene.zoom(e.Delta);
            }
        }

        public delegate void TransformPreview(double x, double y, double z);

        private void menuRotate_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (fileReader.Shape.HasMoreShape)
            {
                if (fileReader.Shape.selectList.Count != 1)
                {
                    MessageBox.Show("请选择一个实体操作");
                    return;
                }
            }
            // TODO 切片之前的实体才能旋转，切片中和切片后不能旋转
            TransformPreview preview = new TransformPreview(rotatePreview);
            Dialog.Rotate rotate = new Dialog.Rotate(preview);
            rotate.Owner = this;
            if (rotate.ShowDialog() == false)
                fileReader.Shape.releaseTransform();
            else
                fileReader.Shape.applyTransform();
            scene.displayAfterTransform(fileReader.Shape.getShape());
            scene.displayShape(fileReader.Shape.getMoreShape());
        }

        void rotatePreview(double xAngle, double yAngle, double zAngle)
        {
            fileReader.Shape.rotate(xAngle, yAngle, zAngle);
            scene.displayAfterTransform(fileReader.Shape.transform);
            scene.displayShape(fileReader.Shape.getNotTransformShape());
        }

        private void menuEntityEdit_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (fileReader.Shape.HasMoreShape)
            {
                MessageBox.Show("已经打开大于一个图形，请先合并图形");
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

        private void afterImportMoreStep(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(displayImportMoreStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void displayImportMoreStep(object workResult)
        {
            if (dlgEntityEdit.Combine)
            {
                fileReader.Shape.combine();
                scene.Proxy.cleanScene();
                scene.displayShape(fileReader.Shape.getShape());
            }
            else
                scene.displayShape(fileReader.Shape.getMoreShape());
            afterOpenFile();
        }

        private void menuAbout_Click(object sender, RoutedEventArgs e)
        {
            Dialog.About about = new Dialog.About();
            about.Owner = this;
            about.ShowDialog();
        }

        private void menuEntityProp_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
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

        private void menuMove_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (fileReader.Shape.HasMoreShape)
            {
                if (fileReader.Shape.selectList.Count != 1)
                {
                    MessageBox.Show("请选择一个实体操作");
                    return;
                }
            }
            TransformPreview preview = new TransformPreview(panPreview);
            Dialog.Pan pan = new Dialog.Pan(unit, preview);
            pan.Owner = this;
            if (pan.ShowDialog() == false)
                fileReader.Shape.releaseTransform();
            else
                fileReader.Shape.applyTransform();
            scene.displayAfterTransform(fileReader.Shape.getShape());
            scene.displayShape(fileReader.Shape.getMoreShape());
        }

        void panPreview(double x, double y, double z)
        {
            fileReader.Shape.move(x, y, z);
            scene.displayAfterTransform(fileReader.Shape.transform);
            scene.displayShape(fileReader.Shape.getNotTransformShape());
        }

        private void buttonSaveAs_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "step";
            saveFile.Filter = "Step file (*.step)|*.step";
            if (false == saveFile.ShowDialog(this))
                return ;
            fileReader.saveStep(saveFile.FileName);
            this.Title = saveFile.FileName;
        }

        private void ButtonRoll_Click(object sender, RoutedEventArgs e)
        {
            if ((bool)ButtonPan.IsChecked)
                ButtonPan.IsChecked = false;
            if ((bool)ButtonSelect.IsChecked)
                ButtonSelect.IsChecked = false;
            ButtonRoll.IsChecked = true;
        }

        private void ButtonPan_Click(object sender, RoutedEventArgs e)
        {
            if ((bool)ButtonRoll.IsChecked)
                ButtonRoll.IsChecked = false;
            if ((bool)ButtonSelect.IsChecked)
                ButtonSelect.IsChecked = false;
            ButtonPan.IsChecked = true;

        }

        private void ButtonSelect_Checked(object sender, RoutedEventArgs e)
        {
            if ((bool)ButtonPan.IsChecked)
                ButtonPan.IsChecked = false;
            if ((bool)ButtonRoll.IsChecked)
                ButtonRoll.IsChecked = false;
            ButtonSelect.IsChecked = true;
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

        private void ButtonSelect_Click(object sender, RoutedEventArgs e)
        {
            if (GridScene != null && (bool)ButtonSelect.IsChecked)
                GridScene.Cursor = Cursors.Cross;
        }

        private void menuBase0_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            fileReader.Shape.base0AllShapes();
            scene.displayAfterTransform(fileReader.Shape.getShape());
        }


        private void menuBase0xy_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            fileReader.Shape.base0XyCenter();
            scene.displayAfterTransform(fileReader.Shape.getShape());
        }

        private void buttonSaveDxf_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.slice.sliceList.Count == 0)
            {
                MessageBox.Show("未打开切片文件");
                return;
            }
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "rp";
            saveFile.Filter = "RP file (*.rp)|*.rp";
            if (false == saveFile.ShowDialog(this))
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

        private void menuDisplaySetting_Click(object sender, RoutedEventArgs e)
        {
            Dialog.DisplaySetting dspSetDlg = new Dialog.DisplaySetting(scene.Setting);
            if (false == dspSetDlg.ShowDialog())
                return;
        }

        public void resetView(double ratio)
        {
            scene.Proxy.ZoomAllView();
            double scale = scene.Proxy.getZoomScale();
            slicingScene.Proxy.setZoomScale(scale / ratio);
            double x = 0, y = 0, z = 0;
            unsafe
            {
                scene.Proxy.getViewPoint(&x, &y, &z);
            }
            slicingScene.Proxy.setViewPoint(x * ratio, y * ratio, z * ratio);
        }

        private void menuEntityMerge_Click(object sender, RoutedEventArgs e)
        {
            if (fileReader.Shape.IsEmpty)
            {
                MessageBox.Show("未打开3D文件");
                return;
            }
            if (!fileReader.Shape.HasMoreShape)
            {
                MessageBox.Show("没有可以合并的图形");
                return;
            }
            fileReader.Shape.combine();
            scene.Proxy.cleanScene();
            scene.displayShape(fileReader.Shape.getShape());
        }

        private void ButtonSetCenter_Click(object sender, RoutedEventArgs e)
        {
            scene.Proxy.ZoomAllView();
            slicingScene.Proxy.ZoomAllView();
        }
    }
}

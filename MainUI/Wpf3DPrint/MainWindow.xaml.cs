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
            Dialog.About about = new Dialog.About();
            this.Title += "  " + about.Version;
        }

        private void onOpeningFile(string fileName)
        {
            labelStatus.Content = "Reading file " + fileName + " ...";
        }

        private void afterOpenFile()
        {
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
            if (fileReader.Shape.hasSlice)
            {
                MessageBox.Show("请先关闭已打开的切片文件");
                return;
            }
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".stp";
            openFile.Filter = "所有实体文件(STEP; STL)|*.stp;*.step;*.stl;*.ast|STEP file (*.stp;*.step)|*.stp;*.step|STL Mesh(*.stl;*.ast)|*.stl;*.ast";
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
            Dialog.DialogUnit unit = new Dialog.DialogUnit(fileReader.Shape);
            unit.Owner = this;
            unit.ShowDialog();
            this.unit = unit.Unit;
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
            double x0 = 0, y0 = 0, z0 = 0;
            if (!fileReader.Shape.checkBase0(ref x0, ref y0, ref z0))
            {
                if (MessageBox.Show("图形底面中心坐标为(" + x0.ToString("0.000") + "," + y0.ToString("0.000") + "," + z0.ToString("0.000") + "),未居中，要继续分层吗？", "提示", MessageBoxButton.OKCancel) != MessageBoxResult.OK)
                    return;
            }
            Dialog.DialogSlice dlSlice = new Dialog.DialogSlice(fileReader.Shape, unit);
            dlSlice.Owner = this;
            if (dlSlice.ShowDialog() == false)
                return;
            setSlicingView();
            fileReader.Shape.slice.sliceThick = dlSlice.sliceThick;
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
            {
                setSlicingView();
                return "";
            }
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
            fileReader.Shape.slice.sortSliceList();
            string fileName = saveSlice();
            if (fileName.Length == 0)
            {
                afterSliceView();
                slicingScene.Proxy.cleanScene();
                return;
            }
            fileReader.Shape.release();
            scene.Proxy.cleanScene();
            slicingScene.Proxy.cleanScene();
            openSlice(fileName);
        }

        private void GridScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            scene.Resize(Convert.ToInt32(GridScene.ActualWidth), Convert.ToInt32(GridScene.ActualHeight));
            slicingScene.Resize(Convert.ToInt32(GridSlice3D.ActualWidth), Convert.ToInt32(GridSlice3D.ActualHeight));
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
                {
                    unsafe
                    {
                        double x = 0, y = 0, z = 0;
                        scene.Proxy.Rotation((int)p.X, (int)p.Y, &x, &y, &z);
                        slicingScene.Proxy.setProjection(x, y, z);
                    }
                }
                if ((bool)ButtonPan.IsChecked)
                {
                    scene.Proxy.Pan(-(int)((mouseOrigin.X - p.X) / ratio), (int)((mouseOrigin.Y - p.Y) / ratio));
                    unsafe
                    {
                        double x = 0, y = 0, z = 0;
                        scene.Proxy.getViewPoint(&x, &y, &z);
                        slicingScene.Proxy.setViewPoint(x, y, z);
                    }
                }
                scene.Proxy.RedrawView();
                slicingScene.Proxy.RedrawView();
                mouseOrigin = new Point((int)e.GetPosition(GridScene).X, (int)e.GetPosition(GridScene).Y);
            }
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
            foreach(Slice.OneSlice shape in fileReader.Shape.slice.sliceList)
            {
                scene.displayShape(shape.slice);
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
            sliceScene.clearWindow();
            TreeViewSlice.Items.Clear();
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
            if (fileReader.Shape.hasSlice)
            {
                MessageBox.Show("请先关闭已打开的切片文件");
                return;
            }
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.DefaultExt = ".slc";
            openFile.Filter = "所有切片文件|*.slc;*dxf|Slice file (*.slc)|*.slc|Dxf file (*.dxf)|*.dxf";
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
            setSliceView();
            fileReader.Shape.slice.base0SliceList();
            fileReader.Shape.slice.base0XyCenter();
            fileReader.Shape.slice.sortSliceList();
            scene.Proxy.cleanScene();
            afterOpenFile();
            afterSliceView();
        }

        void afterSliceView()
        {
            foreach (Slice.OneSlice slice in fileReader.Shape.slice.sliceList)
            {
                scene.displaySlice(slice.slice);
            }
            ArrayList list = new ArrayList();
            for (int i = 0; i < fileReader.Shape.slice.sliceList.Count; i++)
            {
                list.Add(i + 1);
            }
            TreeViewItem root = new TreeViewItem();
            root.Header = fileReader.Shape.fileName.Substring(fileReader.Shape.fileName.LastIndexOf('\\') + 1);
            foreach (int i in list)
            {
                TreeViewItem item = new TreeViewItem();
                item.Header = i;
                root.Items.Add(item);
            }
            TreeViewSlice.SelectedItemChanged += TreeViewSlice_SelectedItemChanged;
            root.IsExpanded = true;
            TreeViewSlice.Items.Add(root);
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

        private void TreeViewSlice_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue == null)
                return;
            object header = ((TreeViewItem)e.NewValue).Header;
            if (header is int)
            {
                int value = (int)header;
                setSliceView();
                selectSlice(value - 1);
            }
        }

        private void set3DView()
        {
            column3D.Width = new GridLength(100, GridUnitType.Star);
            columnTree.Width = new GridLength(0, GridUnitType.Star);
            column2D.Width = new GridLength(0, GridUnitType.Star);
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

        private void setRebuildView()
        {
            column3D.Width = new GridLength(40, GridUnitType.Star);
            column2D.Width = new GridLength(50, GridUnitType.Star);
            columnTree.Width = new GridLength(10, GridUnitType.Star);
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
            if (TreeViewSlice.SelectedValue == null)
                return;
            object header = ((TreeViewItem)TreeViewSlice.SelectedValue).Header;
            if (header is int)
            {
                int selectIndex = (int)header;
                if (fileReader.Shape.slice.sliceList.Count > selectIndex)
                    sliceScene.drawSlice(e, (Slice.OneSlice)(fileReader.Shape.slice.sliceList[selectIndex - 1]));
            }
        }

        private void PanelSlice_Resize(object sender, EventArgs e)
        {
            if (sliceScene != null)
                sliceScene.onResize(fileReader.Shape.slice.getBound());
        }

        private void selectSlice(int index)
        {
            if (fileReader.Shape.slice.sliceList.Count == 0 || fileReader.Shape.slice.sliceList.Count <= index)
                return;
            Slice.OneSlice slice = (Slice.OneSlice)(fileReader.Shape.slice.sliceList[index]);
            if (fileReader.Shape.selectList.IndexOf(slice) != -1)
                return;
            scene.selectSlice(slice.slice);
            fileReader.Shape.selectList.Add(slice.slice);
            slicingScene.Proxy.cleanScene();
            PanelSlice.Invalidate();
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
            setRebuildView();
            RebuildSlice rebuildSlice = new RebuildSlice(fileReader.Shape.slice, slicingScene);
            slicingScene.Proxy.cleanScene();
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
            dlgEntityEdit = new Dialog.EntityEdit(fileReader.Shape, unit);
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

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            fileReader.Shape.release();
            scene.Dispose();
            slicingScene.Dispose();
            fileReader.Dispose();
            sliceScene.Dispose();
            System.Environment.Exit(0);
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
            if (fileReader.Shape.HasMoreShape)
            {
                MessageBox.Show("请先合并多个图形");
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
            if (fileReader.Shape.HasMoreShape)
            {
                MessageBox.Show("请先合并多个图形");
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

        private void menuDisplayRebuildView_Click(object sender, RoutedEventArgs e)
        {
            setRebuildView();
        }

        private void menuDisplaySliceView_Click(object sender, RoutedEventArgs e)
        {
            setSliceView();
        }

        private void menuDisplay3DView_Click(object sender, RoutedEventArgs e)
        {
            set3DView();
        }
    }
}

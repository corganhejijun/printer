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
        private Scene slicingScene;
        private Scene2D sliceScene;
        private FileReader fileReader;

        Dialog.EntityEdit dlgEntityEdit;
        string unit;

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
            bool reDisplay = false;
            if (dlSlice.setZ0)
            {
                fileReader.Shape.base0AllShapes();
                reDisplay = true;
            }
            if (dlSlice.setXY0)
            {
                fileReader.Shape.base0XyCenter();
                reDisplay = true;
            }
            if (reDisplay)
                scene.displayAfterTransform(fileReader.Shape.getShape());
            setSlicingView();
            fileReader.Shape.slice.sliceThick = dlSlice.sliceThick;
            fileReader.sliceShape((Control)this, dlSlice.locatePlane, dlSlice.gradientShape, dlSlice.quickSlice, afterSlice, new SceneThread.onFunction(onSlice));
            ToolBarOper.IsEnabled = false;
            ToolBarReset.IsEnabled = false;
            MenuMain.IsEnabled = false;
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

        private void afterSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(onAfterSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void onAfterSlice(object args)
        {
            ToolBarOper.IsEnabled = true;
            ToolBarReset.IsEnabled = true;
            MenuMain.IsEnabled = true;
            fileReader.Shape.slice.sortSliceList();
            string fileName = saveSlice();
            if (fileName.Length == 0)
            {
                afterSlice();
                slicingScene.Proxy.cleanScene();
                return;
            }
            fileReader.Shape.release();
            scene.Proxy.cleanScene();
            slicingScene.Proxy.cleanScene();
            openSlice(fileName);
        }

        void afterSlice()
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

        private void menuAbout_Click(object sender, RoutedEventArgs e)
        {
            Dialog.About about = new Dialog.About();
            about.Owner = this;
            about.ShowDialog();
        }

        private void menuUnselect_Click(object sender, RoutedEventArgs e)
        {
            foreach (Slice.OneSlice shape in fileReader.Shape.slice.sliceList)
            {
                scene.displayShape(shape.slice);
            }
            fileReader.Shape.selectList.Clear();
        }
    }
}

using System;
using System.Windows;
using System.Windows.Input;

namespace Wpf3DPrint
{
    public partial class MainWindow
    {

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

        private void GridScene_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            beginRotate = false;
        }

        private void GridScene_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            if (Math.Abs(e.Delta) > 8)
            {
                scene.zoom(e.Delta);
                slicingScene.zoom(e.Delta);
            }
        }

        private void PanelSlice_Resize(object sender, EventArgs e)
        {
            if (sliceScene != null)
                sliceScene.onResize(fileReader.Shape.slice.getBound());
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

        private void ButtonSelect_Checked(object sender, RoutedEventArgs e)
        {
            if ((bool)ButtonPan.IsChecked)
                ButtonPan.IsChecked = false;
            if ((bool)ButtonRoll.IsChecked)
                ButtonRoll.IsChecked = false;
            ButtonSelect.IsChecked = true;
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

        private void menuDisplaySetting_Click(object sender, RoutedEventArgs e)
        {
            Dialog.DisplaySetting dspSetDlg = new Dialog.DisplaySetting(scene.Setting);
            if (false == dspSetDlg.ShowDialog())
                return;
        }
    }
}

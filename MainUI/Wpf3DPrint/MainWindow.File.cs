using Microsoft.Win32;
using System.Windows;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint
{
    public partial class MainWindow
    {
        private void onOpeningFile(string fileName)
        {
            labelStatus.Content = "Reading file " + fileName + " ...";
        }

        private void afterOpenFile()
        {
            labelStatus.Content = "";
            this.Title = "3DLT  " + fileReader.Shape.FileName;
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
            this.Dispatcher.Invoke(new SceneThread.afterFunction(onAfterOpenStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void onAfterOpenStep(object workResult)
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
            this.Title = fileReader.Shape.FileName;
            return saveFile.FileName;
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
            if (!fileReader.openSlice(fileName, afterOpenSlice))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(fileName);
        }

        private void afterOpenSlice(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(onAfterOpenSlice), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void onAfterOpenSlice(object workResult)
        {
            setSliceView();
            fileReader.Shape.slice.base0SliceList();
            fileReader.Shape.slice.base0XyCenter();
            fileReader.Shape.slice.sortSliceList();
            scene.Proxy.cleanScene();
            afterOpenFile();
            afterSlice();
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
            saveFile.Filter = "Step file (*.step)|*.step|STL file (*.stl)|*.stl";
            if (false == saveFile.ShowDialog(this))
                return;
            if (saveFile.FileName.EndsWith(".step"))
            {
                fileReader.saveStep(saveFile.FileName);
            }
            else if (saveFile.FileName.EndsWith(".stl"))
            {
                fileReader.saveStl(saveFile.FileName);
            }
        }

        public void saveAsStep()
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.FileName = "model";
            saveFile.DefaultExt = "step";
            saveFile.Filter = "Step file (*.step)|*.step";
            if (false == saveFile.ShowDialog(this))
                return;
            fileReader.saveStep(saveFile.FileName);
            afterOpenFile();
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
            saveFile.DefaultExt = "dxf";
            saveFile.Filter = "DXF file (*.dxf)|*.dxf";
            if (false == saveFile.ShowDialog(this))
                return;
            DxfWriter writer = new DxfWriter(saveFile.FileName, fileReader.Shape);
            writer.openFile();
            writer.writeSlice();
            writer.fileClose();
            writer.Dispose();
        }

        private void buttonClose_Click(object sender, RoutedEventArgs e)
        {
            scene.Proxy.cleanScene();
            slicingScene.Proxy.cleanScene();
            scene.Proxy.AxoView();
            slicingScene.Proxy.AxoView();
            TreeViewSlice.Items.Clear();
            fileReader.Shape.release();
            set3DView();
            set3DFunction(true);
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

        private void buttonQuit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}

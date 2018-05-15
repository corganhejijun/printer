using System.Windows;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint
{
    public partial class MainWindow
    {

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
            TransformPreview preview = new TransformPreview(rotatePreview);
            Dialog.Rotate rotate = new Dialog.Rotate(preview);
            rotate.Owner = this;
            if (rotate.ShowDialog() == false)
                fileReader.Shape.releaseTransform();
            else
            {
                fileReader.Shape.applyTransform();
                if (MessageBox.Show("是否保存当前实体？", "提醒", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    saveAsStep();
            }
            scene.displayAfterTransform(fileReader.Shape.getShape());
            scene.displayShape(fileReader.Shape.getMoreShape());
        }

        void rotatePreview(double xAngle, double yAngle, double zAngle)
        {
            fileReader.Shape.rotate(xAngle, yAngle, zAngle);
            scene.displayAfterTransform(fileReader.Shape.transform);
            scene.displayShape(fileReader.Shape.getNotTransformShape());
        }

        private void menuEntityAdd_Click(object sender, RoutedEventArgs e)
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
            if (dlgEntityAdd != null)
                dlgEntityAdd = null;
            TransformPreview preview = new TransformPreview(addEntityPreview);
            dlgEntityAdd = new Dialog.EntityAdd(fileReader.Shape, unit, preview);
            dlgEntityAdd.Owner = this;
            if (dlgEntityAdd.ShowDialog() == false)
            {
                fileReader.Shape.releaseMoreShape();
                scene.Proxy.cleanScene();
                scene.displayShape(fileReader.Shape.getShape());
                return;
            }
            importStep();
        }

        void importStep()
        {
            if (!fileReader.importMoreStep(dlgEntityAdd.FileName, afterImportMoreStep))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(dlgEntityAdd.FileName);
            set3DView();
        }

        void addEntityPreview(double x, double y, double z)
        {
            dlgEntityAdd.isPreview = true;
            importStep();
        }

        private void afterImportMoreStep(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(onAfterImportMoreStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void onAfterImportMoreStep(object workResult)
        {
            if (!dlgEntityAdd.CenterPos)
            {
                fileReader.Shape.setMoreShapePos(dlgEntityAdd.X, dlgEntityAdd.Y, dlgEntityAdd.Z);
            }
            if (dlgEntityAdd.Combine && !dlgEntityAdd.isPreview)
            {
                fileReader.Shape.combine();
                scene.Proxy.cleanScene();
                scene.displayShape(fileReader.Shape.getShape());
            }
            else
                scene.displayShape(fileReader.Shape.getMoreShape());
            afterOpenFile();
            if (!dlgEntityAdd.isPreview)
            {
                if (MessageBox.Show("是否保存当前所有实体？", "提醒", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                {
                    saveAsStep();
                }
            }
            else
            {
                dlgEntityAdd.isPreview = false;
                fileReader.Shape.releaseMoreShape();
            }
        }

        private void menuEntityProp_Click(object sender, RoutedEventArgs e)
        {
            Dialog.EntityProp entity = new Dialog.EntityProp(fileReader.Shape, unit);
            entity.Owner = this;
            if (entity.ShowDialog() == false)
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
            TransformPreview preview = new TransformPreview(movePreview);
            Dialog.Pan pan = new Dialog.Pan(unit, preview);
            pan.Owner = this;
            if (pan.ShowDialog() == false)
                fileReader.Shape.releaseTransform();
            else
            {
                fileReader.Shape.applyTransform();
                if (MessageBox.Show("是否保存当前实体？", "提醒", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    saveAsStep();
            }
            scene.displayAfterTransform(fileReader.Shape.getShape());
            scene.displayShape(fileReader.Shape.getMoreShape());
        }

        void movePreview(double x, double y, double z)
        {
            fileReader.Shape.move(x, y, z);
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
            Dialog.EntityEdit entityEdit = new Dialog.EntityEdit(unit, fileReader.Shape);
            entityEdit.Owner = this;
            if (entityEdit.ShowDialog() == false)
            {
                return;
            }
        }

        private void menuUndo_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}

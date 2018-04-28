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
                fileReader.Shape.applyTransform();
            scene.displayAfterTransform(fileReader.Shape.getShape());
            scene.displayShape(fileReader.Shape.getMoreShape());
            if (MessageBox.Show("是否保存当前实体？", "提醒", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                saveAsStep();
            }
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
            TransformPreview preview = new TransformPreview(addEntityPreview);
            dlgEntityEdit = new Dialog.EntityEdit(fileReader.Shape, unit, preview);
            dlgEntityEdit.Owner = this;
            if (dlgEntityEdit.ShowDialog() == false)
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
            if (!fileReader.importMoreStep(dlgEntityEdit.FileName, afterImportMoreStep))
            {
                MessageBox.Show("Open file Failed!");
                return;
            }
            onOpeningFile(dlgEntityEdit.FileName);
            set3DView();
        }

        void addEntityPreview(double x, double y, double z)
        {
            dlgEntityEdit.isPreview = true;
            importStep();
        }

        private void afterImportMoreStep(object args)
        {
            this.Dispatcher.Invoke(new SceneThread.afterFunction(onAfterImportMoreStep), System.Windows.Threading.DispatcherPriority.Normal, new object[] { args });
        }

        private void onAfterImportMoreStep(object workResult)
        {
            if (!dlgEntityEdit.CenterPos)
            {
                fileReader.Shape.setMoreShapePos(dlgEntityEdit.X, dlgEntityEdit.Y, dlgEntityEdit.Z);
            }
            if (dlgEntityEdit.Combine && !dlgEntityEdit.isPreview)
            {
                fileReader.Shape.combine();
                scene.Proxy.cleanScene();
                scene.displayShape(fileReader.Shape.getShape());
            }
            else
                scene.displayShape(fileReader.Shape.getMoreShape());
            afterOpenFile();
            if (!dlgEntityEdit.isPreview)
            {
                if (MessageBox.Show("是否保存当前所有实体？", "提醒", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                {
                    saveAsStep();
                }
            }
            else
            {
                dlgEntityEdit.isPreview = false;
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
                fileReader.Shape.applyTransform();
            scene.displayAfterTransform(fileReader.Shape.getShape());
            scene.displayShape(fileReader.Shape.getMoreShape());
            if (MessageBox.Show("是否保存当前实体？", "提醒", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                saveAsStep();
            }
        }

        void movePreview(double x, double y, double z)
        {
            fileReader.Shape.move(x, y, z);
            scene.displayAfterTransform(fileReader.Shape.transform);
            scene.displayShape(fileReader.Shape.getNotTransformShape());
        }
    }
}

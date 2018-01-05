using System;
using System.Collections;

namespace Wpf3DPrint.Viewer
{
    public class Shape
    {
        public string fileName;
        IntPtr shape;
        public IntPtr transform;
        public Slice slice;
        public ArrayList selectList;
        IntPtr moreShape;

        public Shape()
        {
            fileName = "";
            shape = IntPtr.Zero;
            transform = IntPtr.Zero;
            slice = new Slice();
            selectList = new ArrayList();
        }

        public void setShape(IntPtr shape)
        {
            if (this.shape != IntPtr.Zero)
            {
                this.moreShape = shape;
                return;
            }
            this.shape = shape;
        }

        public IntPtr getShape()
        {
            return shape;
        }

        public IntPtr getMoreShape()
        {
            return moreShape;
        }

        public bool HasMoreShape
        {
            get
            {
                return moreShape != IntPtr.Zero;
            }
        }

        public bool IsEmpty
        {
            get
            {
                return shape == IntPtr.Zero;
            }
        }

        public void release()
        {
            releaseTransform();
            if (slice.sliceList.Count > 0)
            {
                foreach (Slice.OneSlice slice in slice.sliceList)
                {
                    Cpp2Managed.Shape3D.del(slice.slice);
                }
                slice.sliceList.Clear();
            }
            if (moreShape != IntPtr.Zero)
            {
                Cpp2Managed.Shape3D.del(moreShape);
                moreShape = IntPtr.Zero;
            }
            if (shape != IntPtr.Zero)
            {
                Cpp2Managed.Shape3D.del(shape);
                shape = IntPtr.Zero;
            }
        }

        public void move(double x, double y, double z)
        {
            IntPtr toMove = shape;
            if (moreShape != IntPtr.Zero)
            {
                if (selectList.Count != 1)
                    return;
                toMove = (IntPtr)selectList[0];
            }
            IntPtr move = Cpp2Managed.Shape3D.move(toMove, x, y, z);
            releaseTransform();
            transform = move;
        }

        public void rotate(double x, double y, double z)
        {
            IntPtr toRotate = shape;
            if (moreShape != IntPtr.Zero)
            {
                if (selectList.Count != 1)
                    return;
                toRotate = (IntPtr)selectList[0];
            }
            IntPtr rotate = Cpp2Managed.Shape3D.rotate(toRotate, x, y, z);
            releaseTransform();
            transform = rotate;
        }

        public void releaseTransform()
        {
            if (transform != IntPtr.Zero)
            {
                Cpp2Managed.Shape3D.del(transform);
                transform = IntPtr.Zero;
            }
        }

        public void applyTransform()
        {
            if (transform == IntPtr.Zero)
                return;
            if (moreShape != IntPtr.Zero)
            {
                IntPtr selectPt = (IntPtr)selectList[0];
                selectList.Clear();
                if (moreShape == selectPt)
                {
                    Cpp2Managed.Shape3D.del(moreShape);
                    moreShape = transform;
                    transform = IntPtr.Zero;
                    return;
                }
            }
            Cpp2Managed.Shape3D.del(shape);
            shape = transform;
            transform = IntPtr.Zero;
        }

        public IntPtr getNotTransformShape()
        {
            IntPtr selectPt = (IntPtr)selectList[0];
            if (moreShape != selectPt)
                return moreShape;
            if (shape != selectPt)
                return shape;
            return IntPtr.Zero;
        }

        public void base0AllShapes()
        {
            double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, Zmin = 0, Zmax = 0;
            Cpp2Managed.Shape3D.getBoundary(shape, ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
            if (Zmin < 0.0001 && Zmin > -0.0001)
                return;
            IntPtr move = Cpp2Managed.Shape3D.move(shape, 0, 0, -Zmin);
            Cpp2Managed.Shape3D.del(shape);
            shape = move;
        }

        public void base0XyCenter()
        {
            double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, Zmin = 0, Zmax = 0;
            Cpp2Managed.Shape3D.getBoundary(shape, ref Zmin, ref Zmax, ref Ymin, ref Ymax, ref Xmin, ref Xmax);
            double centerX = Xmin + (Xmax - Xmin) / 2;
            double centerY = Ymin + (Ymax - Ymin) / 2;
            IntPtr move = Cpp2Managed.Shape3D.move(shape, -centerX, -centerY, 0);
            Cpp2Managed.Shape3D.del(shape);
            shape = move;
        }
    }
}

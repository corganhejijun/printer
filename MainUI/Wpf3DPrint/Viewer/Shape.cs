﻿using System;
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
            this.shape = shape;
        }

        public IntPtr getShape()
        {
            return shape;
        }

        public bool IsEmpty
        {
            get
            {
                return shape == IntPtr.Zero;
            }
        }
    }
}

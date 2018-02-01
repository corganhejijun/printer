using System;
using System.Collections;
using System.Runtime.InteropServices;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint
{
    class RebuildSlice
    {
        System.Windows.Forms.Timer rebuildTimer;
        int rebuildIndex;
        Slice slice;
        Scene scene;
        Cpp2Managed.Shape3D.GetNextEdge deleOnGetEdge;
        Cpp2Managed.Shape3D.GetFaceHole deleOnGetHole;
        ArrayList outlineList;
        ArrayList toDel;
        public RebuildSlice(Slice slice, Scene scene)
        {
            if (rebuildTimer == null)
                rebuildTimer = new System.Windows.Forms.Timer();
            rebuildTimer.Tick += RebuildTimer_Elapsed;
            rebuildTimer.Interval = 100;
            rebuildIndex = slice.sliceList.Count;
            this.slice = slice;
            this.scene = scene;
            deleOnGetEdge = new Cpp2Managed.Shape3D.GetNextEdge(onGetEdge);
            deleOnGetHole = new Cpp2Managed.Shape3D.GetFaceHole(onGetFaceHole);
            outlineList = new ArrayList();
            toDel = new ArrayList();
            rebuildTimer.Start();
        }

        IntPtr onGetEdge(ref Cpp2Managed.EdgeType type, int index)
        {
            IntPtr result = IntPtr.Zero;
            object obj = currentOutline.edges[index];
            if (obj is Cpp2Managed.BSpline)
            {
                type = Cpp2Managed.EdgeType.bSplice;
                Cpp2Managed.BSpline bs = (Cpp2Managed.BSpline)obj;
                result = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.BSpline4Dll)));
                Cpp2Managed.BSpline4Dll bsDll = new Cpp2Managed.BSpline4Dll();
                bsDll.start = bs.start;
                bsDll.end = bs.end;
                bsDll.polesCnt = bs.poles.Length;
                bsDll.polesPt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Point)) * bsDll.polesCnt);
                for (int i = 0; i < bs.poles.Length; i++)
                {
                    IntPtr pt = new IntPtr(bsDll.polesPt.ToInt64() + Marshal.SizeOf(typeof(Cpp2Managed.Point)) * i);
                    Marshal.StructureToPtr(bs.poles[i], pt, false);
                }
                Marshal.StructureToPtr(bsDll, result, false);
                toDel.Add(bsDll.polesPt);
            }
            else if (obj is Cpp2Managed.Circle)
            {
                type = Cpp2Managed.EdgeType.circle;
                Cpp2Managed.Circle c = (Cpp2Managed.Circle)obj;
                result = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Circle)));
                Marshal.StructureToPtr(c, result, false);
            }
            else if (obj is Cpp2Managed.Line)
            {
                type = Cpp2Managed.EdgeType.line;
                Cpp2Managed.Line l = (Cpp2Managed.Line)obj;
                result = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Cpp2Managed.Line)));
                Marshal.StructureToPtr(l, result, false);
            }
            else
            {
                type = Cpp2Managed.EdgeType.unknown;
                return IntPtr.Zero;
            }
            toDel.Add(result);
            return result;
        }

        void arrangeOutline(Slice.OneSlice.Outline outline, int direction)
        {
            // 一级轮廓是外轮廓，二级轮廓是内轮廓，三级轮廓又是外轮廓，以此类推
            // 将所有外轮廓提取出来,组成外轮廓数组，
            if (direction > 0)
                outlineList.Add(outline);
            if (outline.child != null)
            {
                foreach (Slice.OneSlice.Outline child in outline.child)
                    arrangeOutline(child, -direction);
            }
        }

        IntPtr onGetFaceHole(double height, int index)
        {
            Slice.OneSlice.Outline outlineChild = (Slice.OneSlice.Outline)currentOutline.child[index];
            currentOutline = outlineChild;
            return Cpp2Managed.Shape3D.slice2Wires(height, deleOnGetEdge, outlineChild.edges.Count);
        }

        Slice.OneSlice.Outline currentOutline;
        void rebuild(Slice.OneSlice oneSlice)
        {
            ArrayList outlines = oneSlice.getOutlineList();
            foreach (Slice.OneSlice.Outline outline in outlines)
            {
                arrangeOutline(outline, 1);
            }
            foreach (Slice.OneSlice.Outline outline in outlineList)
            {
                currentOutline = outline;
                // 将外轮廓组成一个wire
                IntPtr wires = Cpp2Managed.Shape3D.slice2Wires(oneSlice.height, deleOnGetEdge, outline.edges.Count);
                // 由外轮廓wire生成面, 在面内根据内轮廓挖洞
                IntPtr face = Cpp2Managed.Shape3D.makeFaceFromWire(wires, oneSlice.height, outline.child.Count, deleOnGetHole);
                // 拉伸并显示
                scene.Proxy.strechSlice(slice.sliceThick, face);
            }
            foreach (IntPtr pt in toDel)
                Marshal.FreeHGlobal(pt);
            toDel.Clear();
        }

        void RebuildTimer_Elapsed(object sender, EventArgs e)
        {
            if (rebuildIndex == 0)
            {
                rebuildTimer.Stop();
                rebuildTimer.Dispose();
                rebuildTimer = null;
                return;
            }
            rebuildIndex--;
            Slice.OneSlice oneSlice = (Slice.OneSlice)slice.sliceList[rebuildIndex];
            rebuild(oneSlice);
        }
    }
}

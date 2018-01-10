using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wpf3DPrint.Viewer;

namespace Wpf3DPrint
{
    class RebuildSlice
    {
        System.Windows.Forms.Timer rebuildTimer;
        int rebuildIndex;
        Slice slice;
        Scene scene;
        public RebuildSlice(Slice slice, Scene scene)
        {
            if (rebuildTimer == null)
                rebuildTimer = new System.Windows.Forms.Timer();
            rebuildTimer.Tick += RebuildTimer_Elapsed;
            rebuildTimer.Interval = 100;
            rebuildIndex = slice.sliceList.Count;
            this.slice = slice;
            this.scene = scene;
            rebuildTimer.Start();
        }

        void rebuild(Slice.OneSlice oneSlice)
        {
            scene.Proxy.strechSlice(oneSlice.slice, slice.sliceThick);
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

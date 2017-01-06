using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Sys;

namespace Driver
{
    public class ART
    {
        public ART()
        {

        }
        IntPtr hDevice;
        public bool create()
        {
            hDevice = PCI1010.PCI1010_CreateDevice(0);
            if (hDevice == (IntPtr)(-1))
            {
                return false;
            }
            return true;
        }
    }
}

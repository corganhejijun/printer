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
            hDevice = (IntPtr)(-1);
            m_x = new Axis();
            m_y = new Axis();
        }
        IntPtr hDevice;
        public bool create()
        {
            hDevice = PCI1010.PCI1010_CreateDevice(0);
            if (!DeviceAvailable)
            {
                return false;
            }
            return true;
        }
        public bool DeviceAvailable
        {
            get
            {
                return !(hDevice == (IntPtr)(-1));
            }
        }
        class Axis
        {
            public PCI1010.PCI1010_PARA_DataList DL;
            public PCI1010.PCI1010_PARA_LCData LC;
            public bool running;
            public Axis()
            {
                LC.LV_DV = PCI1010.PCI1010_LV;           // 定长驱动:PCI1010_DV | 连续驱动:PCI1010_LV
                LC.Mode = PCI1010.PCI1010_CWCCW;        // 模式0：CW/CCW方式，1：CP/DIR方式 
                LC.Line_Curve = PCI1010.PCI1010_CURVE;  // 直线曲线(0:直线加/减速; 1:S曲线加/减速)
                LC.nPulseNum = 120000;			// 定量输出脉冲数(0~268435455)

                DL.Multiple = 1;                    // 倍数 (1~500) 
                DL.AccelerationK = 2000;        // 加速度变化率(仅S曲线驱动时有效)
                DL.StartSpeed = 100;			// 初始速度(1~8000)

                running = false;
            }
        }
        public enum Direction
        {
            backward = 0,
            forward = 1
        }
        Axis m_x;
        Axis m_y;
        bool StartAxis(ref Axis ax, int speed, int accelerate, Direction direction)
        {
            ax.LC.AxisNum = PCI1010.PCI1010_XAXIS;		// 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
            ax.LC.Direction = (int)direction;// 转动方向 PCI1010_PDirection: 正转  PCI1010_MDirection:反转		
            ax.DL.Acceleration = accelerate;			// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
            ax.DL.DriveSpeed = speed;			// 驱动速度	(1~8000)	

            if (0 == PCI1010.PCI1010_SetLP(hDevice, ax.LC.AxisNum, 0))
                return false;
            // 设置实位计数器 
            if (0 == PCI1010.PCI1010_SetEP(hDevice, ax.LC.AxisNum, 0))
                return false;
            if (0 == PCI1010.PCI1010_SetAccofst(hDevice, ax.LC.AxisNum, 0))
                return false;
            //	初始化连续,定长脉冲驱动
            if (0 == PCI1010.PCI1010_InitLVDV(hDevice, ref ax.DL, ref ax.LC))
                return false;
            // 启动定长脉冲驱动
            if (0 == PCI1010.PCI1010_StartLVDV(hDevice, ax.LC.AxisNum))
                return false;
            ax.running = true;
            return true;
        }

        bool stopAxis(ref Axis ax)
        {
            if (0 == PCI1010.PCI1010_DecStop(hDevice, ax.LC.AxisNum))	// 减速停止
                return false;
            return true;
        }

        public bool stopX()
        {
            return stopAxis(ref m_x);
        }

        public bool stopY()
        {
            return stopAxis(ref m_y);
        }

        public bool startX(int speed, int accelerate, Direction direction)
        {
            return StartAxis(ref m_x, speed, accelerate, direction);
        }

        public bool startY(int speed, int accelerate, Direction direction)
        {
            return StartAxis(ref m_y, speed, accelerate, direction);
        }

        public bool XIsRuning
        {
            get
            {
                return m_x.running;
            }
        }

        public bool YIsRunning
        {
            get
            {
                return m_y.running;
            }
        }
    }
}

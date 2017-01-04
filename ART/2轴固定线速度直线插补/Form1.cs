using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            
        }
        Int32 A, LPX, LPY, speed;
        IntPtr hDevice;
        PCI1010.PCI1010_PARA_DataList pDL;
        PCI1010.PCI1010_PARA_LineData pLD;

        private void button_Start_Click(object sender, EventArgs e)
        {
            hDevice = PCI1010.PCI1010_CreateDevice(0);
            if (hDevice == (IntPtr)(-1))
            {
                MessageBox.Show("创建设备失败！");
                return;
            }
            pLD.Line_Curve = 1;			// 直线曲线(PCI1010_Line:直线加/减速; PCI1010_Curve:S曲线加/减速)
            pLD.ConstantSpeed = 1;   // 固定线速度
            pDL.Multiple = 1;						// 倍数(1~500)
            pDL.Acceleration = 1250;				// 加速度(1~1000,000)
            pDL.AccelerationK = 3000;               // 加速度
            pDL.StartSpeed = 100;					// 初始速度(1~8000)
            pDL.DriveSpeed = 5000;					// 驱动速度	(1~8000)
            pLD.nXAxisPulseNum = 90000;				// X轴终点坐标脉冲数(-8388608~+8388607) 
            pLD.nYAxisPulseNum = 45000;				// Y轴终点坐标脉冲数(-8388608~+8388607)

            PCI1010.PCI1010_SetLP(hDevice, 1, 0);
            PCI1010.PCI1010_SetLP(hDevice, 2, 0);
            PCI1010.PCI1010_InitLineInterpolation(		    // 初始化直线插补运动 
                                     hDevice,			// 设备句柄
                                     ref pDL,
                                     ref pLD);

            PCI1010.PCI1010_StartLineInterpolation(hDevice); // 开始插补运动

            timer1.Enabled = true;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            speed = (PCI1010.PCI1010_ReadCV(hDevice, 0x01)) * pDL.Multiple;
            A = (PCI1010.PCI1010_ReadCA(hDevice, 0x1)) * pDL.Multiple * 125;
            LPX = PCI1010.PCI1010_ReadLP(hDevice, 0x1);
            LPY = PCI1010.PCI1010_ReadLP(hDevice, 0x2);

                textBox_AD0.Text = speed.ToString("0.##");
                textBox_AD1.Text = A.ToString("0.##");
                textBox_AD2.Text = LPX.ToString("0.##");
                textBox_AD3.Text = LPY.ToString("0.##");	
             
        }

        private void button_stop_Click(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            PCI1010.PCI1010_DecStop(hDevice, 3);	// 减速停止
        }
    }
}
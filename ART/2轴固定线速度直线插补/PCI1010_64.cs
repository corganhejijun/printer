using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class PCI1010
    {
       // 公用参数 

        public struct PCI1010_PARA_DataList
    {
        public Int32 Multiple;
	    public Int32 StartSpeed;			// 初始速度(1~8000)
	    public Int32 DriveSpeed;			// 驱动速度(1~8000)
	    public Int32 Acceleration;			// 加速度(125~1000000)
	    public Int32 Deceleration;			// 减速度(125~1000000)
        public Int32 AccelerationK;			// 加速度变化率(954~62500000)
    }

    // 直线和S曲线参数 
        public struct PCI1010_PARA_LCData
    {
	    public Int32 AxisNum;				// 轴号 (X轴 | Y轴 | X、Y轴)
	    public Int32 LV_DV;					// 驱动方式  (连续 | 定长 )	
	    public Int32 Mode;				    // 脉冲方式 (CW/CCW方式 | CP/DIR方式)	
	    public Int32 Line_Curve;			// 运动方式	(直线 | 曲线)
	    public Int32 Direction;				// 运动方向 (正方向 | 反方向)
	    public Int32 nPulseNum;		    	// 定量输出脉冲数(0~268435455)
    } 

    // 直线插补和固定线速度直线插补参数
        public struct PCI1010_PARA_LineData	
    {	
	    public Int32 Line_Curve;			// 运动方式	(直线 | 曲线)
	    public Int32 ConstantSpeed;			// 固定线速度 (不固定线速度 | 固定线速度)
        public Int32 nXAxisPulseNum;		// X轴插补脉冲数 (-8388608~8388607)
        public Int32 nYAxisPulseNum;		// Y轴插补脉冲数 (-8388608~8388607)		
    } 

    // 正反方向圆弧插补参数
        public struct PCI1010_PARA_CircleData	
    {
	    public Int32 ConstantSpeed;			// 固定线速度 (不固定线速度 | 固定线速度)
        public Int32 XCenter;				// X轴圆心坐标(脉冲数-8388608~8388607)
        public Int32 YCenter;				// Y轴圆心坐标(脉冲数-8388608~8388607)
        public Int32 XPulse;				// X轴终点坐标(脉冲数-8388608~8388607)	
        public Int32 YPulse;				// Y轴终点坐标(脉冲数-8388608~8388607)
    } 

    /***************************************************************/
    // 轴号
    public const Int32		PCI1010_XAXIS			=0X1;				// X轴
    public const Int32		PCI1010_YAXIS			=0x2;				// Y轴
    public const Int32      PCI1010_XYAXIS          =0x3;				// X、Y轴

    /***************************************************************/
    // 驱动方式
    public const Int32		PCI1010_DV				=0x0;				// 定长驱动
    public const Int32		PCI1010_LV				=0x1;				// 连续驱动

    /***************************************************************/
    // 脉冲输出方式
    public const Int32 	    PCI1010_CWCCW			=0X0;				// CW/CCW方式 
    public const Int32 	    PCI1010_CPDIR 			=0X1;				// CP/DIR方式

    /***************************************************************/
    // 脉冲输入方式
    public const Int32 	    PCI1010_A_B			    =0X0;				// A/B相方式
    public const Int32 	    PCI1010_U_D 			=0X1;				//上/下脉冲输入方式

    /***************************************************************/
    // 运动方式
    public const Int32		PCI1010_LINE			=0X0;				// 直线运动
    public const Int32		PCI1010_CURVE			=0X1;				// S曲线运动

    /***************************************************************/
    // 运动方向
    public const Int32		PCI1010_MDIRECTION		=0x0;				// 反方向
    public const Int32		PCI1010_PDIRECTION		=0x1;				// 正方向

    /***************************************************************/
    //固定线速度
    public const Int32		PCI1010_NOCONSTAND		=0X0;				// 不固定线速度
    public const Int32		PCI1010_CONSTAND		=0X1;				// 固定线速度

    /***************************************************************/
    // 软件限位的逻辑实位计数器选择和设置外部越限信号的停止方式和设置外部停止信号的停止号选择
    /***************************************************************/
    // 计数器类别
    public const Int32		PCI1010_LOGIC			=0x0;				// 逻辑位计数器
    public const Int32		PCI1010_FACT			=0x1;				// 实位计数器

    /***************************************************************/
    // 外部停止信号
    public const Int32      PCI1010_STOP0           =0X0;				// 停止信号0
    public const Int32      PCI1010_STOP1           =0X1;				// 停止信号1

    /***************************************************************/
    // 停止方式
    public const Int32		PCI1010_SUDDENSTOP		=0x0;				// 立即停止
    public const Int32		PCI1010_DECSTOP			=0X1;				// 减速停止

    /********************************************************************/
    public const Int32		PCI1010_ERROR			=0XFF;			    // 错误

    /****************************************************************/
    // 状态寄存器RR0
    public struct PCI1010_PARA_RR0      
    {
	    public UInt32 XDRV;			// X轴的驱动状态  1：正在输出脉冲 0：停止驱动
	    public UInt32 YDRV;			// Y轴的驱动状态  1：正在输出脉冲 0：停止驱动
	    public UInt32 XERROR;		// X轴的出错状态  X轴的RR2寄存器的任何一位为1，此位就为1
	    public UInt32 YERROR;		// Y轴的出错状态  Y轴的RR2寄存器的任何一位为1，此位就为1
	    public UInt32 IDRV;			// 插补驱动状态   1：正处于插补模式  0：未处于插补模式
	    public UInt32 CNEXT;		// 表示可以写入连续插补的下一个数据  1：可以写入 0：不可以写入
	                                // 当设置连续插补中断使能后，CNEXT为1表示产生了中断，在中断程序写入下一个插补命令后，该位清零并且中断信号回到高电平
	    public UInt32 ZONE0;		// ZONE2、ZONE1、ZONE0表示在圆弧插补驱动中所在的象限
	    public UInt32 ZONE1;		// 000 ：第0象限   001：第1象限  010：第2象限  011：第3象限
	    public UInt32 ZONE2;		// 100 ：第4象限   101：第5象限	 110：第6象限  111：第7象限
	    public UInt32 BPSC0;		// BPSC1、BPSC0表示在位插补驱动中堆栈计数器(SC)的数值
	    public UInt32 BPSC1;		// 00： 0   01：1   10： 2   11：3
						            // 设置位插补中断使能后，当SC的值由2变为1时，产生中断，
	                                // 当向位插补堆栈写入新的数据或调用PCI1010_ClearInterruptStatus，中断解除。
    } 

    // 状态寄存器RR1，每一个轴都有RR1寄存器，读哪个要指定轴号
    public struct PCI1010_PARA_RR1    
    {
	    public UInt32 CMPP;			// 表示逻辑/实位计数器和COMP+寄存器的大小关系 1：逻辑/实位计数器≥COMP+ 0：逻辑/实位计数器＜COMP+
	    public UInt32 CMPM;			// 表示逻辑/实位计数器和COMP-寄存器的大小关系 1：逻辑/实位计数器＜COMP- 0：逻辑/实位计数器≥COMP-
	    public UInt32 ASND;			// 在加/减速驱动中加速时，为1
	    public UInt32 CNST;			// 在加/减速驱动中定速时，为1
	    public UInt32 DSND;			// 在加/减速驱动中减速时，为1
	    public UInt32 AASND;		// 在S曲线加/减速驱动中，加速度/减速度增加时，为1 
	    public UInt32 ACNST;		// 在S曲线加/减速驱动中，加速度/减速度不变时，为1 
	    public UInt32 ADSND;		// 在S曲线加/减速驱动中，加速度/减速度减少时，为1 
        public UInt32 STOP0;		// 外部停止信号IN0有效使驱动停止时，为1
        public UInt32 STOP1;		// 外部停止信号IN1有效使驱动停止时，为1
	    public UInt32 LMTP;			// 外部正方向限制信号(nLMTP)有效使驱动停止时，为1
	    public UInt32 LMTM;			// 外部反方向限制信号(nLMTM)有效使驱动停止时，为1
	    public UInt32 ALARM;		// 外部伺服马达报警信号(nALARM)有效使驱动停止时，为1
	    public UInt32 EMG;			// 外部紧急停止信号(EMGN)使驱动停止时，为1
    } 

    // 状态寄存器RR2，每一个轴都有RR2寄存器，读哪个要指定轴号

    public struct PCI1010_PARA_RR2     
    {
	    public UInt32 SLMTP;		// 设置正方向软件限位后，在正方向驱动中，逻辑/实位计数器大于COMP+寄存器值时，为1
	    public UInt32 SLMTM;		// 设置反方向软件限位后，在反方向驱动中，逻辑/实位计数器小于COMP-寄存器值时，为1
	    public UInt32 HLMTP;		// 外部正方向限制信号(nLMTP)处于有效电平时，为1
	    public UInt32 HLMTM;		// 外部反方向限制信号(nLMTM)处于有效电平时，为1
	    public UInt32 ALARM;		// 外部伺服马达报警信号(nALARM)设置为有效并处于有效状态时，为1
	    public UInt32 EMG;			// 外部紧急停止信号处于低电平时，为1
    } 

    // 状态寄存器RR3
    public struct PCI1010_PARA_RR3      
    {
        public UInt32 XSTOP0;		// 外部停止信号XIN0的电平状态 1：高电平 0：低电平
        public UInt32 XSTOP1;		// 外部停止信号XIN1的电平状态 1：高电平 0：低电平
        public UInt32 EMG;			// 外部紧急停止信号EMG的电平状态 1：高电平 0：低电平
	    public UInt32 XEXPP;		// 外部正方向点动输入信号XEXPP的电平状态 1：高电平 0：低电平
	    public UInt32 XEXPM;		// 外部反方向点动输入信号XEXPM的电平状态 1：高电平 0：低电平
	    public UInt32 XINPOS;		// 外部伺服电机到位信号XINPOS的电平状态  1：高电平 0：低电平
	    public UInt32 XALARM;		// 外部伺服马达报警信号XALARM的电平状态  1：高电平 0：低电平
        public UInt32 XIN0;			// 外部输入信号XIN0的电平状态  1：高电平 0：低电平
        public UInt32 XIN1;			// 外部输入信号XIN1的电平状态  1：高电平 0：低电平
        public UInt32 XLMTP;		// 外部正方向限制信号XLMTP的电平状态  1：高电平 0：低电平
        public UInt32 XLMTM;		// 外部反方向限制信号XLMTM的电平状态  1：高电平 0：低电平
    } 

    // 状态寄存器RR4
    public struct PCI1010_PARA_RR4     
    {
        public UInt32 YSTOP0;		// 外部停止信号YSTOP0的电平状态 1：高电平 0：低电平
        public UInt32 YSTOP1;		// 外部停止信号YSTOP1的电平状态 1：高电平 0：低电平
        public UInt32 YEXPP;		// 外部正方向点动输入信号YEXPP的电平状态 1：高电平 0：低电平
        public UInt32 YEXPM;		// 外部反方向点动输入信号YEXPM的电平状态 1：高电平 0：低电平
        public UInt32 YINPOS;		// 外部伺服电机到位信号YINPOS的电平状态  1：高电平 0：低电平
        public UInt32 YALARM;		// 外部伺服马达报警信号YALARM的电平状态  1：高电平 0：低电平
        public UInt32 YIN0;			// 外部输入信号YIN0的电平状态  1：高电平 0：低电平
        public UInt32 YIN1;			// 外部输入信号YIN1的电平状态  1：高电平 0：低电平
        public UInt32 YLMTP;		// 外部正方向限制信号YLMTP的电平状态  1：高电平 0：低电平
        public UInt32 YLMTM;		// 外部反方向限制信号YLMTM的电平状态  1：高电平 0：低电平
    } 

    //######################## 设备对象管理函数 #################################
    // 适用于本设备的最基本操作	
    [DllImport("PCI1010_64.DLL")]        
    public static extern IntPtr PCI1010_CreateDevice(			// 创建句柄
							    int DeviceID);				    // 设备ID号
    [DllImport("PCI1010_64.DLL")] 
    public static extern int PCI1010_GetDeviceCount(			// 获得设备总数
							    IntPtr hDevice);			    // 设备句柄


    [DllImport("PCI1010_64.DLL")] 
    public static extern Int32 PCI1010_ReleaseDevice(			// 释放设备
							    IntPtr hDevice);			    // 设备句柄

    [DllImport("PCI1010_64.DLL")] 
    public static extern Int32 PCI1010_Reset(				    // 复位整个USB设备
							    IntPtr hDevice);		        // 设备句柄
    //*******************************************************************
    // 设置电机的基本参数模式、倍率、加速度、减速度、加/减速度变化率、初始速度、输出脉冲数
    // 插补输出脉冲数、圆心坐标、逻辑计数器、实际位置计数器、加速计数器偏移
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_PulseOutMode(        // 设置脉冲输出模式
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Mode);				    // 模式

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_PulseInputMode(      // 设置脉冲输入模式
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Mode);				    // 模式

         [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetR(				// 设置倍率(1-500)
                                 IntPtr hDevice,			    // 设备句柄
                                 Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)     
                                 Int32 Data);				    // 倍率值(1-500)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetA(				// 设置加速度
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)     
                                Int32 Data);				    // 加速度 (125-1000000)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetDec(				// 设置减速度
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Data);				    // 减速度值


        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetAK(		        // 加速度变化率  
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)   
							    Int32 Data);				    // 数据

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetSV(				// 设置初始速度
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)   
							    Int32 Data);				    // 速度值

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetV(				// 设置驱动速度
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)     
							    Int32 Data);				    // 驱动速度值

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetP(				// 设置定长脉冲数
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴) 
							    Int32 Data);			        // 定长脉冲数(0-268435455)

        [DllImport("PCI1010_64.DLL")] 
            public static extern Int32 PCI1010_SetIP(		    // 设置插补终点脉冲数(-8388608-+8388607)
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Data);				    // 插补终点脉冲数(-8388608-+8388607)

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_SetC(               // 设置圆心坐标(脉冲数)  
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Data);				    // 圆心坐标脉冲数范围(-2147483648-+2147483647)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetLP(				// 设置逻辑位置计数器
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Data);				    // 逻辑位置计数器值(-2147483648-+2147483647)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetEP(			    // 设置实位计数器 
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Data);				    // 实位计数器值(-2147483648-+2147483647)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetAccofst(			// 设置加速计数器偏移
							    IntPtr hDevice,			        // 设备句柄
                                Int32 AxisNum,			        // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Data);				    // 偏移范围(0-65535)

    //*******************************************************************
    // 直线S曲线初始化、启动函数
        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_InitLVDV(			// 初始化连续,定长脉冲驱动
							    IntPtr hDevice,				    // 设备句柄
							    ref PCI1010_PARA_DataList pDL,  // 公共参数结构体指针
							    ref PCI1010_PARA_LCData pLD);	// 直线S曲线参数结构体指针

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_StartLVDV(			// 启动连续,定长脉冲驱动
							    IntPtr hDevice,				    // 设备句柄
                                Int32 AxisNum);				    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32	PCI1010_Start2D(IntPtr hDevice);// 2轴同时启动	
    //*******************************************************************
    // 直线插补初始化、启动函数
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InitLineInterpolation(	// 初始化连续,定长脉冲驱动
                                IntPtr hDevice,				    // 设备句柄
                                ref PCI1010_PARA_DataList pDL,  // 公共参数结构体指针
                                ref PCI1010_PARA_LineData pLD);	// 直线插补结构体指针	

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartLineInterpolation(// 启动直线插补运动
                                IntPtr hDevice);                // 2设备句柄
         
    //*******************************************************************
    // 正反方向圆弧插补初始化、启动函数
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InitCWInterpolation(	// 初始化正反方向圆弧插补运动 
							    IntPtr hDevice,				    // 设备句柄
							    ref PCI1010_PARA_DataList pDL,  // 公共参数结构体指针
							    ref PCI1010_PARA_CircleData pCD);// 圆弧插补结构体指针

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartCWInterpolation( // 启动正方向圆弧插补运动 
	    						IntPtr hDevice);		        // 设备句柄

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartCCWInterpolation( // 启动反方向圆弧插补运动 
                                IntPtr hDevice);		        // 设备句柄      
    //*************************************************************************
    // 位插补相关函数
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InitBitInterpolation(	// 初始化任意2轴位插补参数
							    IntPtr hDevice,				    // 设备句柄
							    ref PCI1010_PARA_DataList pDL); // 公共参数结构体指针

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_AutoBitInterpolation( // 启动任意2轴位插补子线程
							    IntPtr hDevice,				    // 设备句柄
							    UInt16 []pBuffer,				// 位插补数据指针	
							    UInt32 nCount);				    // 数据组数

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReleaseBitInterpolation(	// 释放位插补
							    IntPtr hDevice);			    // 设备句柄

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetBP(                // 设置位插补数据
							    IntPtr hDevice,				    // 设备句柄 
                                Int32 BP1PData,				    // X轴正方向驱动数据
                                Int32 BP1MData,				    // X轴反方向驱动数据
                                Int32 BP2PData,				    // Y轴正方向驱动数据
                                Int32 BP2MData);			    // Y轴反方向驱动数据

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartBitInterpolation(// 启动位插补
							    IntPtr hDevice);			// 设备句柄

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_BPWait(			// 等待位插补的下一个数据设定
							    IntPtr hDevice);			// 设备句柄

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ClearBPData(	    // 清除BP寄存器数据
							    IntPtr hDevice);			// 设备句柄
    //*******************************************************************
    // 连续插补相关函数

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_NextWait(		// 等待连续插补下一个节点命令设定
							    IntPtr hDevice);			// 设备句柄

    //*******************************************************************
    // 外部信号启动电机定长驱动、连续驱动

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetOutEnableDV(	// 设置外部使能定量驱动(下降沿有效)
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetOutEnableLV(	// 设置外部使能连续驱动(保持低电平有效)
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

    //*******************************************************************
    // 设置软件限位有效和无效
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetPDirSoftwareLimit( // 设置正方向软件限位
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum,			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
                                Int32 LogicFact,			// 逻辑/实位计数器选择 PCI1010_LOGIC：逻辑位置计数器 PCI1010_FACT：实位计数器
							    Int32 Data);				// 软件限位数据

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetMDirSoftwareLimit( // 设置反方向软件限位
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum,			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 LogicFact,		    // 逻辑/实位计数器选择 PCI1010_LOGIC：逻辑位置计数器 PCI1010_FACT：实位计数器	
							    Int32 Data);

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ClearSoftwareLimit(	 // 清除软件限位
							    IntPtr hDevice,			 // 设备句柄
                                Int32 AxisNum);			 // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

    //******************************************************************* 
    // 设置外部输入信号的有效和无效	
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetLMTEnable(	// 设置外部越限信号的有效及停止方式	
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum,			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 StopMode);            // PCI1010_DECSTOP：减速停止，PCI1010_SUDDENSTOP：立即停止

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_SetStopEnable(  // 设置外部停止信号有效
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum,			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 StopNum);			    // 停止号

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_SetStopDisable(		 // 设置外部停止信号无效
							    IntPtr hDevice,			 // 设备句柄
                                Int32 AxisNum,			 // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 StopNum);			 // 停止号

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetALARMEnable(// 设置伺服报警信号有效 
							    IntPtr hDevice,			 // 设备句柄
                                Int32 AxisNum);			 // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetALARMDisable(// 设置伺服报警信号无效  
							    IntPtr hDevice,			 // 设备句柄
                                Int32 AxisNum);			 // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetINPOSEnable(// 设置伺服马达定位完毕输入信号有效 
							    IntPtr hDevice,			 // 设备句柄	
                                Int32 AxisNum);			 // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴) 

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetINPOSDisable(// 设置伺服马达定位完毕输入信号无效
							    IntPtr hDevice,			 // 设备句柄
                                Int32 AxisNum);			 // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

    //*******************************************************************
    // 减速函数设置

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_DecValid(	    // 减速有效
							    IntPtr hDevice);		    // 设备句柄

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_DecInvalid(		// 减速无效
							    IntPtr hDevice);		    // 设备句柄

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_DecStop(		// 减速停止
							    IntPtr hDevice,			    // 设备句柄
							    Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴,PCI1010_YAXIS:Y轴, PCI1010_ZAXIS:Z轴,PCI1010_UAXIS:U轴)  

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InstStop(		// 立即停止
							    IntPtr hDevice,			    // 设备句柄
							    Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴,PCI1010_YAXIS:Y轴, PCI1010_ZAXIS:Z轴,PCI1010_UAXIS:U轴) 

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_AutoDec(		// 自动减速
							    IntPtr hDevice,			    // 设备句柄
							    Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴,PCI1010_YAXIS:Y轴, PCI1010_ZAXIS:Z轴,PCI1010_UAXIS:U轴) 

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_HanDec(		    // 手动减速点设定
							    IntPtr hDevice,			    // 设备句柄
							    Int32 AxisNum,			    // 轴号(PCI1010_XAXIS:X轴,PCI1010_YAXIS:Y轴, PCI1010_ZAXIS:Z轴,PCI1010_UAXIS:U轴) 
							    Int32 Data);				// 手动减速点数据，范围(0 - 4294967295)

    //*************************************************************************
    // 读电机状态：逻辑计数器、实际位置计数器、当前速度、加/减速度
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadLP(			// 读逻辑计数器
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_ReadEP(			// 读实位计数器
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴) 
            
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadCV(			// 读当前速度
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴) 

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadCA(			// 读当前加速度
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum);			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)

    //*******************************************************************
    // 设置通用输出和读通用输入
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetOutput(		// 设置通用输出电平
							    IntPtr hDevice,			    // 设备句柄
                                Int32 XOut1,			    // X轴OUT1 1：XOUT1为高电平  0：XOUT0为低电平
                                Int32 XOut0,                // X轴OUT0 1：XOUT1为高电平  0：XOUT0为低电平
                                Int32 YOut1,                // Y轴OUT1 1：YOUT1为高电平  0：YOUT0为低电平
                                Int32 YOut0);	            // Y轴OUT0 1：YOUT1为高电平  0：YOUT0为低电平

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadTYPE(
                                 IntPtr hDevice,	 	    // 设备句柄
                                 Int32 AxisNum);			// 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
    //*******************************************************************
    // 读状态寄存器的位状态
        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_ReadRR(			// 读RR寄存器
							    IntPtr hDevice,			    // 设备句柄
                                Int32 AxisNum,			    // 轴号(PCI1010_XAXIS:X轴; PCI1010_YAXIS:Y轴)
							    Int32 Num);				    // 寄存器号

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR0Status(		 // 获得主状态寄存器RR0的位状态
							    IntPtr hDevice,			 // 设备句柄
							    ref PCI1010_PARA_RR0 pPara);// RR0寄存器状态

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR1Status(		 // 获得状态寄存器RR1的位状态
							    IntPtr hDevice,			 // 设备句柄
							    Int32 AxisNum,			 // 轴号(PCI1010_XAXIS:X轴,PCI1010_YAXIS:Y轴, PCI1010_ZAXIS:Z轴,PCI1010_UAXIS:U轴) 
							    ref PCI1010_PARA_RR1 pPara);// RR1寄存器状态			

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR2Status(		 // 获得状态寄存器RR2的位状态
							    IntPtr hDevice,			 // 设备句柄
							    Int32 AxisNum,			 // 轴号(PCI1010_XAXIS:X轴,PCI1010_YAXIS:Y轴, PCI1010_ZAXIS:Z轴,PCI1010_UAXIS:U轴) 
							    ref PCI1010_PARA_RR2 pPara);// RR2寄存器状态			

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR3Status(		 // 获得状态寄存器RR3的位状态
							    IntPtr hDevice,			 // 设备句柄
							    ref PCI1010_PARA_RR3 pPara);// RR3寄存器状态			

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR4Status(		 // 获得状态寄存器RR4的位状态
							    IntPtr hDevice,			 // 设备句柄
							    ref PCI1010_PARA_RR4 pPara);// RR4寄存器状态

    }
}

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class PCI1010
    {
       // ���ò��� 

        public struct PCI1010_PARA_DataList
    {
        public Int32 Multiple;
	    public Int32 StartSpeed;			// ��ʼ�ٶ�(1~8000)
	    public Int32 DriveSpeed;			// �����ٶ�(1~8000)
	    public Int32 Acceleration;			// ���ٶ�(125~1000000)
	    public Int32 Deceleration;			// ���ٶ�(125~1000000)
        public Int32 AccelerationK;			// ���ٶȱ仯��(954~62500000)
    }

    // ֱ�ߺ�S���߲��� 
        public struct PCI1010_PARA_LCData
    {
	    public Int32 AxisNum;				// ��� (X�� | Y�� | X��Y��)
	    public Int32 LV_DV;					// ������ʽ  (���� | ���� )	
	    public Int32 Mode;				    // ���巽ʽ (CW/CCW��ʽ | CP/DIR��ʽ)	
	    public Int32 Line_Curve;			// �˶���ʽ	(ֱ�� | ����)
	    public Int32 Direction;				// �˶����� (������ | ������)
	    public Int32 nPulseNum;		    	// �������������(0~268435455)
    } 

    // ֱ�߲岹�͹̶����ٶ�ֱ�߲岹����
        public struct PCI1010_PARA_LineData	
    {	
	    public Int32 Line_Curve;			// �˶���ʽ	(ֱ�� | ����)
	    public Int32 ConstantSpeed;			// �̶����ٶ� (���̶����ٶ� | �̶����ٶ�)
        public Int32 nXAxisPulseNum;		// X��岹������ (-8388608~8388607)
        public Int32 nYAxisPulseNum;		// Y��岹������ (-8388608~8388607)		
    } 

    // ��������Բ���岹����
        public struct PCI1010_PARA_CircleData	
    {
	    public Int32 ConstantSpeed;			// �̶����ٶ� (���̶����ٶ� | �̶����ٶ�)
        public Int32 XCenter;				// X��Բ������(������-8388608~8388607)
        public Int32 YCenter;				// Y��Բ������(������-8388608~8388607)
        public Int32 XPulse;				// X���յ�����(������-8388608~8388607)	
        public Int32 YPulse;				// Y���յ�����(������-8388608~8388607)
    } 

    /***************************************************************/
    // ���
    public const Int32		PCI1010_XAXIS			=0X1;				// X��
    public const Int32		PCI1010_YAXIS			=0x2;				// Y��
    public const Int32      PCI1010_XYAXIS          =0x3;				// X��Y��

    /***************************************************************/
    // ������ʽ
    public const Int32		PCI1010_DV				=0x0;				// ��������
    public const Int32		PCI1010_LV				=0x1;				// ��������

    /***************************************************************/
    // ���������ʽ
    public const Int32 	    PCI1010_CWCCW			=0X0;				// CW/CCW��ʽ 
    public const Int32 	    PCI1010_CPDIR 			=0X1;				// CP/DIR��ʽ

    /***************************************************************/
    // �������뷽ʽ
    public const Int32 	    PCI1010_A_B			    =0X0;				// A/B�෽ʽ
    public const Int32 	    PCI1010_U_D 			=0X1;				//��/���������뷽ʽ

    /***************************************************************/
    // �˶���ʽ
    public const Int32		PCI1010_LINE			=0X0;				// ֱ���˶�
    public const Int32		PCI1010_CURVE			=0X1;				// S�����˶�

    /***************************************************************/
    // �˶�����
    public const Int32		PCI1010_MDIRECTION		=0x0;				// ������
    public const Int32		PCI1010_PDIRECTION		=0x1;				// ������

    /***************************************************************/
    //�̶����ٶ�
    public const Int32		PCI1010_NOCONSTAND		=0X0;				// ���̶����ٶ�
    public const Int32		PCI1010_CONSTAND		=0X1;				// �̶����ٶ�

    /***************************************************************/
    // �����λ���߼�ʵλ������ѡ��������ⲿԽ���źŵ�ֹͣ��ʽ�������ⲿֹͣ�źŵ�ֹͣ��ѡ��
    /***************************************************************/
    // ���������
    public const Int32		PCI1010_LOGIC			=0x0;				// �߼�λ������
    public const Int32		PCI1010_FACT			=0x1;				// ʵλ������

    /***************************************************************/
    // �ⲿֹͣ�ź�
    public const Int32      PCI1010_STOP0           =0X0;				// ֹͣ�ź�0
    public const Int32      PCI1010_STOP1           =0X1;				// ֹͣ�ź�1

    /***************************************************************/
    // ֹͣ��ʽ
    public const Int32		PCI1010_SUDDENSTOP		=0x0;				// ����ֹͣ
    public const Int32		PCI1010_DECSTOP			=0X1;				// ����ֹͣ

    /********************************************************************/
    public const Int32		PCI1010_ERROR			=0XFF;			    // ����

    /****************************************************************/
    // ״̬�Ĵ���RR0
    public struct PCI1010_PARA_RR0      
    {
	    public UInt32 XDRV;			// X�������״̬  1������������� 0��ֹͣ����
	    public UInt32 YDRV;			// Y�������״̬  1������������� 0��ֹͣ����
	    public UInt32 XERROR;		// X��ĳ���״̬  X���RR2�Ĵ������κ�һλΪ1����λ��Ϊ1
	    public UInt32 YERROR;		// Y��ĳ���״̬  Y���RR2�Ĵ������κ�һλΪ1����λ��Ϊ1
	    public UInt32 IDRV;			// �岹����״̬   1�������ڲ岹ģʽ  0��δ���ڲ岹ģʽ
	    public UInt32 CNEXT;		// ��ʾ����д�������岹����һ������  1������д�� 0��������д��
	                                // �����������岹�ж�ʹ�ܺ�CNEXTΪ1��ʾ�������жϣ����жϳ���д����һ���岹����󣬸�λ���㲢���ж��źŻص��ߵ�ƽ
	    public UInt32 ZONE0;		// ZONE2��ZONE1��ZONE0��ʾ��Բ���岹���������ڵ�����
	    public UInt32 ZONE1;		// 000 ����0����   001����1����  010����2����  011����3����
	    public UInt32 ZONE2;		// 100 ����4����   101����5����	 110����6����  111����7����
	    public UInt32 BPSC0;		// BPSC1��BPSC0��ʾ��λ�岹�����ж�ջ������(SC)����ֵ
	    public UInt32 BPSC1;		// 00�� 0   01��1   10�� 2   11��3
						            // ����λ�岹�ж�ʹ�ܺ󣬵�SC��ֵ��2��Ϊ1ʱ�������жϣ�
	                                // ����λ�岹��ջд���µ����ݻ����PCI1010_ClearInterruptStatus���жϽ����
    } 

    // ״̬�Ĵ���RR1��ÿһ���ᶼ��RR1�Ĵ��������ĸ�Ҫָ�����
    public struct PCI1010_PARA_RR1    
    {
	    public UInt32 CMPP;			// ��ʾ�߼�/ʵλ��������COMP+�Ĵ����Ĵ�С��ϵ 1���߼�/ʵλ��������COMP+ 0���߼�/ʵλ��������COMP+
	    public UInt32 CMPM;			// ��ʾ�߼�/ʵλ��������COMP-�Ĵ����Ĵ�С��ϵ 1���߼�/ʵλ��������COMP- 0���߼�/ʵλ��������COMP-
	    public UInt32 ASND;			// �ڼ�/���������м���ʱ��Ϊ1
	    public UInt32 CNST;			// �ڼ�/���������ж���ʱ��Ϊ1
	    public UInt32 DSND;			// �ڼ�/���������м���ʱ��Ϊ1
	    public UInt32 AASND;		// ��S���߼�/���������У����ٶ�/���ٶ�����ʱ��Ϊ1 
	    public UInt32 ACNST;		// ��S���߼�/���������У����ٶ�/���ٶȲ���ʱ��Ϊ1 
	    public UInt32 ADSND;		// ��S���߼�/���������У����ٶ�/���ٶȼ���ʱ��Ϊ1 
        public UInt32 STOP0;		// �ⲿֹͣ�ź�IN0��Чʹ����ֹͣʱ��Ϊ1
        public UInt32 STOP1;		// �ⲿֹͣ�ź�IN1��Чʹ����ֹͣʱ��Ϊ1
	    public UInt32 LMTP;			// �ⲿ�����������ź�(nLMTP)��Чʹ����ֹͣʱ��Ϊ1
	    public UInt32 LMTM;			// �ⲿ�����������ź�(nLMTM)��Чʹ����ֹͣʱ��Ϊ1
	    public UInt32 ALARM;		// �ⲿ�ŷ���ﱨ���ź�(nALARM)��Чʹ����ֹͣʱ��Ϊ1
	    public UInt32 EMG;			// �ⲿ����ֹͣ�ź�(EMGN)ʹ����ֹͣʱ��Ϊ1
    } 

    // ״̬�Ĵ���RR2��ÿһ���ᶼ��RR2�Ĵ��������ĸ�Ҫָ�����

    public struct PCI1010_PARA_RR2     
    {
	    public UInt32 SLMTP;		// ���������������λ���������������У��߼�/ʵλ����������COMP+�Ĵ���ֵʱ��Ϊ1
	    public UInt32 SLMTM;		// ���÷����������λ���ڷ����������У��߼�/ʵλ������С��COMP-�Ĵ���ֵʱ��Ϊ1
	    public UInt32 HLMTP;		// �ⲿ�����������ź�(nLMTP)������Ч��ƽʱ��Ϊ1
	    public UInt32 HLMTM;		// �ⲿ�����������ź�(nLMTM)������Ч��ƽʱ��Ϊ1
	    public UInt32 ALARM;		// �ⲿ�ŷ���ﱨ���ź�(nALARM)����Ϊ��Ч��������Ч״̬ʱ��Ϊ1
	    public UInt32 EMG;			// �ⲿ����ֹͣ�źŴ��ڵ͵�ƽʱ��Ϊ1
    } 

    // ״̬�Ĵ���RR3
    public struct PCI1010_PARA_RR3      
    {
        public UInt32 XSTOP0;		// �ⲿֹͣ�ź�XIN0�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
        public UInt32 XSTOP1;		// �ⲿֹͣ�ź�XIN1�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
        public UInt32 EMG;			// �ⲿ����ֹͣ�ź�EMG�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
	    public UInt32 XEXPP;		// �ⲿ������㶯�����ź�XEXPP�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
	    public UInt32 XEXPM;		// �ⲿ������㶯�����ź�XEXPM�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
	    public UInt32 XINPOS;		// �ⲿ�ŷ������λ�ź�XINPOS�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
	    public UInt32 XALARM;		// �ⲿ�ŷ���ﱨ���ź�XALARM�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 XIN0;			// �ⲿ�����ź�XIN0�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 XIN1;			// �ⲿ�����ź�XIN1�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 XLMTP;		// �ⲿ�����������ź�XLMTP�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 XLMTM;		// �ⲿ�����������ź�XLMTM�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
    } 

    // ״̬�Ĵ���RR4
    public struct PCI1010_PARA_RR4     
    {
        public UInt32 YSTOP0;		// �ⲿֹͣ�ź�YSTOP0�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YSTOP1;		// �ⲿֹͣ�ź�YSTOP1�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YEXPP;		// �ⲿ������㶯�����ź�YEXPP�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YEXPM;		// �ⲿ������㶯�����ź�YEXPM�ĵ�ƽ״̬ 1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YINPOS;		// �ⲿ�ŷ������λ�ź�YINPOS�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YALARM;		// �ⲿ�ŷ���ﱨ���ź�YALARM�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YIN0;			// �ⲿ�����ź�YIN0�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YIN1;			// �ⲿ�����ź�YIN1�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YLMTP;		// �ⲿ�����������ź�YLMTP�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
        public UInt32 YLMTM;		// �ⲿ�����������ź�YLMTM�ĵ�ƽ״̬  1���ߵ�ƽ 0���͵�ƽ
    } 

    //######################## �豸��������� #################################
    // �����ڱ��豸�����������	
    [DllImport("PCI1010_64.DLL")]        
    public static extern IntPtr PCI1010_CreateDevice(			// �������
							    int DeviceID);				    // �豸ID��
    [DllImport("PCI1010_64.DLL")] 
    public static extern int PCI1010_GetDeviceCount(			// ����豸����
							    IntPtr hDevice);			    // �豸���


    [DllImport("PCI1010_64.DLL")] 
    public static extern Int32 PCI1010_ReleaseDevice(			// �ͷ��豸
							    IntPtr hDevice);			    // �豸���

    [DllImport("PCI1010_64.DLL")] 
    public static extern Int32 PCI1010_Reset(				    // ��λ����USB�豸
							    IntPtr hDevice);		        // �豸���
    //*******************************************************************
    // ���õ���Ļ�������ģʽ�����ʡ����ٶȡ����ٶȡ���/���ٶȱ仯�ʡ���ʼ�ٶȡ����������
    // �岹�����������Բ�����ꡢ�߼���������ʵ��λ�ü����������ټ�����ƫ��
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_PulseOutMode(        // �����������ģʽ
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Mode);				    // ģʽ

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_PulseInputMode(      // ������������ģʽ
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Mode);				    // ģʽ

         [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetR(				// ���ñ���(1-500)
                                 IntPtr hDevice,			    // �豸���
                                 Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)     
                                 Int32 Data);				    // ����ֵ(1-500)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetA(				// ���ü��ٶ�
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)     
                                Int32 Data);				    // ���ٶ� (125-1000000)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetDec(				// ���ü��ٶ�
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Data);				    // ���ٶ�ֵ


        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetAK(		        // ���ٶȱ仯��  
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)   
							    Int32 Data);				    // ����

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetSV(				// ���ó�ʼ�ٶ�
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)   
							    Int32 Data);				    // �ٶ�ֵ

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetV(				// ���������ٶ�
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)     
							    Int32 Data);				    // �����ٶ�ֵ

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetP(				// ���ö���������
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��) 
							    Int32 Data);			        // ����������(0-268435455)

        [DllImport("PCI1010_64.DLL")] 
            public static extern Int32 PCI1010_SetIP(		    // ���ò岹�յ�������(-8388608-+8388607)
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Data);				    // �岹�յ�������(-8388608-+8388607)

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_SetC(               // ����Բ������(������)  
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Data);				    // Բ��������������Χ(-2147483648-+2147483647)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetLP(				// �����߼�λ�ü�����
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Data);				    // �߼�λ�ü�����ֵ(-2147483648-+2147483647)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetEP(			    // ����ʵλ������ 
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Data);				    // ʵλ������ֵ(-2147483648-+2147483647)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetAccofst(			// ���ü��ټ�����ƫ��
							    IntPtr hDevice,			        // �豸���
                                Int32 AxisNum,			        // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Data);				    // ƫ�Ʒ�Χ(0-65535)

    //*******************************************************************
    // ֱ��S���߳�ʼ������������
        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_InitLVDV(			// ��ʼ������,������������
							    IntPtr hDevice,				    // �豸���
							    ref PCI1010_PARA_DataList pDL,  // ���������ṹ��ָ��
							    ref PCI1010_PARA_LCData pLD);	// ֱ��S���߲����ṹ��ָ��

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_StartLVDV(			// ��������,������������
							    IntPtr hDevice,				    // �豸���
                                Int32 AxisNum);				    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32	PCI1010_Start2D(IntPtr hDevice);// 2��ͬʱ����	
    //*******************************************************************
    // ֱ�߲岹��ʼ������������
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InitLineInterpolation(	// ��ʼ������,������������
                                IntPtr hDevice,				    // �豸���
                                ref PCI1010_PARA_DataList pDL,  // ���������ṹ��ָ��
                                ref PCI1010_PARA_LineData pLD);	// ֱ�߲岹�ṹ��ָ��	

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartLineInterpolation(// ����ֱ�߲岹�˶�
                                IntPtr hDevice);                // 2�豸���
         
    //*******************************************************************
    // ��������Բ���岹��ʼ������������
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InitCWInterpolation(	// ��ʼ����������Բ���岹�˶� 
							    IntPtr hDevice,				    // �豸���
							    ref PCI1010_PARA_DataList pDL,  // ���������ṹ��ָ��
							    ref PCI1010_PARA_CircleData pCD);// Բ���岹�ṹ��ָ��

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartCWInterpolation( // ����������Բ���岹�˶� 
	    						IntPtr hDevice);		        // �豸���

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartCCWInterpolation( // ����������Բ���岹�˶� 
                                IntPtr hDevice);		        // �豸���      
    //*************************************************************************
    // λ�岹��غ���
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InitBitInterpolation(	// ��ʼ������2��λ�岹����
							    IntPtr hDevice,				    // �豸���
							    ref PCI1010_PARA_DataList pDL); // ���������ṹ��ָ��

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_AutoBitInterpolation( // ��������2��λ�岹���߳�
							    IntPtr hDevice,				    // �豸���
							    UInt16 []pBuffer,				// λ�岹����ָ��	
							    UInt32 nCount);				    // ��������

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReleaseBitInterpolation(	// �ͷ�λ�岹
							    IntPtr hDevice);			    // �豸���

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetBP(                // ����λ�岹����
							    IntPtr hDevice,				    // �豸��� 
                                Int32 BP1PData,				    // X����������������
                                Int32 BP1MData,				    // X�ᷴ������������
                                Int32 BP2PData,				    // Y����������������
                                Int32 BP2MData);			    // Y�ᷴ������������

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_StartBitInterpolation(// ����λ�岹
							    IntPtr hDevice);			// �豸���

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_BPWait(			// �ȴ�λ�岹����һ�������趨
							    IntPtr hDevice);			// �豸���

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ClearBPData(	    // ���BP�Ĵ�������
							    IntPtr hDevice);			// �豸���
    //*******************************************************************
    // �����岹��غ���

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_NextWait(		// �ȴ������岹��һ���ڵ������趨
							    IntPtr hDevice);			// �豸���

    //*******************************************************************
    // �ⲿ�ź��������������������������

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetOutEnableDV(	// �����ⲿʹ�ܶ�������(�½�����Ч)
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetOutEnableLV(	// �����ⲿʹ����������(���ֵ͵�ƽ��Ч)
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

    //*******************************************************************
    // ���������λ��Ч����Ч
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetPDirSoftwareLimit( // ���������������λ
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum,			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
                                Int32 LogicFact,			// �߼�/ʵλ������ѡ�� PCI1010_LOGIC���߼�λ�ü����� PCI1010_FACT��ʵλ������
							    Int32 Data);				// �����λ����

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetMDirSoftwareLimit( // ���÷����������λ
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum,			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 LogicFact,		    // �߼�/ʵλ������ѡ�� PCI1010_LOGIC���߼�λ�ü����� PCI1010_FACT��ʵλ������	
							    Int32 Data);

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ClearSoftwareLimit(	 // ��������λ
							    IntPtr hDevice,			 // �豸���
                                Int32 AxisNum);			 // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

    //******************************************************************* 
    // �����ⲿ�����źŵ���Ч����Ч	
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetLMTEnable(	// �����ⲿԽ���źŵ���Ч��ֹͣ��ʽ	
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum,			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 StopMode);            // PCI1010_DECSTOP������ֹͣ��PCI1010_SUDDENSTOP������ֹͣ

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_SetStopEnable(  // �����ⲿֹͣ�ź���Ч
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum,			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 StopNum);			    // ֹͣ��

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_SetStopDisable(		 // �����ⲿֹͣ�ź���Ч
							    IntPtr hDevice,			 // �豸���
                                Int32 AxisNum,			 // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 StopNum);			 // ֹͣ��

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetALARMEnable(// �����ŷ������ź���Ч 
							    IntPtr hDevice,			 // �豸���
                                Int32 AxisNum);			 // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetALARMDisable(// �����ŷ������ź���Ч  
							    IntPtr hDevice,			 // �豸���
                                Int32 AxisNum);			 // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetINPOSEnable(// �����ŷ���ﶨλ��������ź���Ч 
							    IntPtr hDevice,			 // �豸���	
                                Int32 AxisNum);			 // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��) 

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetINPOSDisable(// �����ŷ���ﶨλ��������ź���Ч
							    IntPtr hDevice,			 // �豸���
                                Int32 AxisNum);			 // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

    //*******************************************************************
    // ���ٺ�������

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_DecValid(	    // ������Ч
							    IntPtr hDevice);		    // �豸���

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_DecInvalid(		// ������Ч
							    IntPtr hDevice);		    // �豸���

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_DecStop(		// ����ֹͣ
							    IntPtr hDevice,			    // �豸���
							    Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��,PCI1010_YAXIS:Y��, PCI1010_ZAXIS:Z��,PCI1010_UAXIS:U��)  

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_InstStop(		// ����ֹͣ
							    IntPtr hDevice,			    // �豸���
							    Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��,PCI1010_YAXIS:Y��, PCI1010_ZAXIS:Z��,PCI1010_UAXIS:U��) 

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_AutoDec(		// �Զ�����
							    IntPtr hDevice,			    // �豸���
							    Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��,PCI1010_YAXIS:Y��, PCI1010_ZAXIS:Z��,PCI1010_UAXIS:U��) 

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_HanDec(		    // �ֶ����ٵ��趨
							    IntPtr hDevice,			    // �豸���
							    Int32 AxisNum,			    // ���(PCI1010_XAXIS:X��,PCI1010_YAXIS:Y��, PCI1010_ZAXIS:Z��,PCI1010_UAXIS:U��) 
							    Int32 Data);				// �ֶ����ٵ����ݣ���Χ(0 - 4294967295)

    //*************************************************************************
    // �����״̬���߼���������ʵ��λ�ü���������ǰ�ٶȡ���/���ٶ�
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadLP(			// ���߼�������
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_ReadEP(			// ��ʵλ������
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��) 
            
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadCV(			// ����ǰ�ٶ�
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��) 

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadCA(			// ����ǰ���ٶ�
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum);			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)

    //*******************************************************************
    // ����ͨ������Ͷ�ͨ������
        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_SetOutput(		// ����ͨ�������ƽ
							    IntPtr hDevice,			    // �豸���
                                Int32 XOut1,			    // X��OUT1 1��XOUT1Ϊ�ߵ�ƽ  0��XOUT0Ϊ�͵�ƽ
                                Int32 XOut0,                // X��OUT0 1��XOUT1Ϊ�ߵ�ƽ  0��XOUT0Ϊ�͵�ƽ
                                Int32 YOut1,                // Y��OUT1 1��YOUT1Ϊ�ߵ�ƽ  0��YOUT0Ϊ�͵�ƽ
                                Int32 YOut0);	            // Y��OUT0 1��YOUT1Ϊ�ߵ�ƽ  0��YOUT0Ϊ�͵�ƽ

        [DllImport("PCI1010_64.DLL")] 
        public static extern Int32 PCI1010_ReadTYPE(
                                 IntPtr hDevice,	 	    // �豸���
                                 Int32 AxisNum);			// ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
    //*******************************************************************
    // ��״̬�Ĵ�����λ״̬
        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_ReadRR(			// ��RR�Ĵ���
							    IntPtr hDevice,			    // �豸���
                                Int32 AxisNum,			    // ���(PCI1010_XAXIS:X��; PCI1010_YAXIS:Y��)
							    Int32 Num);				    // �Ĵ�����

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR0Status(		 // �����״̬�Ĵ���RR0��λ״̬
							    IntPtr hDevice,			 // �豸���
							    ref PCI1010_PARA_RR0 pPara);// RR0�Ĵ���״̬

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR1Status(		 // ���״̬�Ĵ���RR1��λ״̬
							    IntPtr hDevice,			 // �豸���
							    Int32 AxisNum,			 // ���(PCI1010_XAXIS:X��,PCI1010_YAXIS:Y��, PCI1010_ZAXIS:Z��,PCI1010_UAXIS:U��) 
							    ref PCI1010_PARA_RR1 pPara);// RR1�Ĵ���״̬			

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR2Status(		 // ���״̬�Ĵ���RR2��λ״̬
							    IntPtr hDevice,			 // �豸���
							    Int32 AxisNum,			 // ���(PCI1010_XAXIS:X��,PCI1010_YAXIS:Y��, PCI1010_ZAXIS:Z��,PCI1010_UAXIS:U��) 
							    ref PCI1010_PARA_RR2 pPara);// RR2�Ĵ���״̬			

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR3Status(		 // ���״̬�Ĵ���RR3��λ״̬
							    IntPtr hDevice,			 // �豸���
							    ref PCI1010_PARA_RR3 pPara);// RR3�Ĵ���״̬			

        [DllImport("PCI1010_64.DLL")] 
         public static extern Int32 PCI1010_GetRR4Status(		 // ���״̬�Ĵ���RR4��λ״̬
							    IntPtr hDevice,			 // �豸���
							    ref PCI1010_PARA_RR4 pPara);// RR4�Ĵ���״̬

    }
}

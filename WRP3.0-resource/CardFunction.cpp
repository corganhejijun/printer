#include "stdafx.h"
#include "types2.h"
int __GetDriver(int index, __DRIVER* pd);
int __GetDriverContext(int&kMax,int&iCurrent);
static BOOL GetMainDriver(char filename[])
{
	int max,ic;__DRIVER drv;
	if(__GetDriverContext(max,ic))
	{
		if(__GetDriver(ic,&drv))
		{
			strcpy(filename,drv.file);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL InitRpcDll()
{
	HINSTANCE HRPC;
	if(rp_dllok)return TRUE;
	char drivername[MAX_PATH]="\0";
	if(!GetMainDriver(drivername))
	{
			AfxMessageBox("还没有设置任何驱动程序!");
			return FALSE;
	}
	if((HRPC=LoadLibrary(drivername))==NULL){
		AfxMessageBox("主驱动程序加载失败!");
		return FALSE;
	}
	CardPosTest=(void(*)(HWND,BOOL)) GetProcAddress(HRPC, "CardPosTest");
	CardTest= (int (*)(HWND )) GetProcAddress(HRPC, "CardTest");
	FillCardPropertyPage=(void(*)(LPPROPSHEETPAGE,LPARAM))GetProcAddress(HRPC, "FillCardPropertyPage");
	RP_InitCards=(int(*)())GetProcAddress(HRPC,"RP_InitCards");
	RP_ArcXY=(int(*)(double[3],double[3],double[3],short))GetProcAddress(HRPC,"RP_ArcXY");
	RP_CardState=(int(*)(long *))GetProcAddress(HRPC,"RP_CardState");
	RP_LightSwitch=(void (*)(BOOL))GetProcAddress(HRPC,"RP_LightSwitch");
	RP_LineXY=(int(*)(double[3],double[3],BOOL))GetProcAddress(HRPC,"RP_LineXY");
	RP_MoveTo=(int(*)(long [3]))GetProcAddress(HRPC,"RP_MoveTo");
	RP_SetZero=(void (*)())GetProcAddress(HRPC,"RP_SetZero");
	RP_StopMove=(void(*)())GetProcAddress(HRPC,"RP_StopMove");
	RP_ZeroAbsolutePos=(void (*)(long*))GetProcAddress(HRPC,"RP_ZeroAbsolutePos");
	RP_Debug=(void (*)())GetProcAddress(HRPC,"RP_Debug");
	RP_BeginWork=(int(*)())GetProcAddress(HRPC,"RP_BeginWork");
	RP_EndWork=(int(*)())GetProcAddress(HRPC,"RP_EndWork");
	RP_BeginLayer=(int(*)())GetProcAddress(HRPC,"RP_BeinLayer");
	RP_EndLayer=(int(*)())GetProcAddress(HRPC,"RP_EndLayer");
	RP_NextLayer=(int(*)(long))GetProcAddress(HRPC,"RP_NextLayer");
	rp_dllok=TRUE;
	return TRUE;
}

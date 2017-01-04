#include "stdafx.h"
#include "rp.h"
#include "types2.h"
HKEY MyGetAppRegistryKey(HKEY HROOT);
//int getwumtoolautoload();
static int pmgeneral=0;
static int pmgraphic=0;
BOOL GetAutoCADPath(LPSTR path)
{
	HKEY hKey;DWORD dwType,lResult,dwCount;
	CString name;
	HWND hWnd;
	//name.LoadString(IDS_ACADPATH_KEY);

	// 2007
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.0\\ACAD-5001:804", 0, KEY_WRITE|KEY_READ|KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS){
		name = "SOFTWARE\\Autodesk\\AutoCAD\\R17.0\\ACAD-5001:804";
		name.LoadString(IDS_ACADPATH_VALUENAME);
		dwType=REG_SZ;
		lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
			NULL, &dwCount);
		if(lResult==ERROR_SUCCESS){
			lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
				(LPBYTE)path, &dwCount);
		}
		RegCloseKey(hKey);
		if (lResult == ERROR_SUCCESS)return TRUE;
		}
	// 2006
	else if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R16.2\\ACAD-4001:804", 0, KEY_WRITE|KEY_READ|KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS){
		name = "SOFTWARE\\Autodesk\\AutoCAD\\R16.2\\ACAD-4001:804";
		name.LoadString(IDS_ACADPATH_VALUENAME);
		dwType=REG_SZ;
		lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
			NULL, &dwCount);
		if(lResult==ERROR_SUCCESS){
			lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
				(LPBYTE)path, &dwCount);
		}
		RegCloseKey(hKey);
		if (lResult == ERROR_SUCCESS)return TRUE;
		}
	// 2005
	else if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R16.1\\ACAD-301:804", 0, KEY_WRITE|KEY_READ|KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS){
		name = "SOFTWARE\\Autodesk\\AutoCAD\\R16.1\\ACAD-301:804";
		name.LoadString(IDS_ACADPATH_VALUENAME);
		dwType=REG_SZ;
		lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
			NULL, &dwCount);
		if(lResult==ERROR_SUCCESS){
			lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
				(LPBYTE)path, &dwCount);
		}
		RegCloseKey(hKey);
		if (lResult == ERROR_SUCCESS)return TRUE;
		}
	// 2004
	else if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R16.0\\ACAD-201:804", 0, KEY_WRITE|KEY_READ|KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS){
		name = "SOFTWARE\\Autodesk\\AutoCAD\\R16.0\\ACAD-201:804";
		name.LoadString(IDS_ACADPATH_VALUENAME);
		dwType=REG_SZ;
		lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
			NULL, &dwCount);
		if(lResult==ERROR_SUCCESS){
			lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
				(LPBYTE)path, &dwCount);
		}
		RegCloseKey(hKey);
		if (lResult == ERROR_SUCCESS)return TRUE;
	}
	// 2000
	else if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Autodesk\\AutoCAD\\R14.0\\ACAD-1:804", 0, KEY_WRITE|KEY_READ|KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS){
		name = "AcadLocation";
		dwType=REG_SZ;
		lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
			NULL, &dwCount);
		if(lResult==ERROR_SUCCESS){
			lResult=RegQueryValueExA(hKey, name , NULL, &dwType,
				(LPBYTE)path, &dwCount);
		}
		RegCloseKey(hKey);
		if (lResult == ERROR_SUCCESS)return TRUE;
	}
	else{
		hWnd = FindWindow(NULL, "");
		MessageBox(hWnd, "您尚未在本机安装 AutoCAD ? ", "Error",  MB_ICONERROR | MB_OK);

	}
	return FALSE;
}
static BOOL getwumtoolautoload(){
	HKEY hKey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software\\Autodesk\\AutoCAD\\R15.0\\ACAD-1:804\\Profiles\\<<Unnamed Profile>>\\Dialogs\\Appload\\Startup"), 0, KEY_READ,
		&hKey) == ERROR_SUCCESS)
	{
		CString strValue;
		DWORD dwType;DWORD dwCount;LONG lResult;
		lResult=RegQueryValueEx(hKey,"NumStartup" , NULL, &dwType,
			NULL, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			lResult=RegQueryValueEx(hKey,"NumStartup" , NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			char*send;long NumStartup;
			NumStartup=strtol(strValue,&send,10);
			if(NumStartup>0){
				CString ValueName;
				for(int i=1;i<=NumStartup;i++){
					ValueName.Format("%dStartup",i);
					lResult=RegQueryValueEx(hKey,ValueName , NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						lResult=RegQueryValueEx(hKey,ValueName , NULL, &dwType,
							(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
						strValue.ReleaseBuffer();
					}
					if (lResult == ERROR_SUCCESS)
					{	
						//////////////////////
						if(strValue.Right(11).CollateNoCase("wumtool.arx")==0)
						{
							RegCloseKey(hKey);
							return TRUE;
						}
						////////////////////////
					}
				}
			}
		}
		RegCloseKey(hKey);
	}
	return FALSE;
}
static CString getmtoolpath(){
	CFileFind ff;CString p;
	if(ff.FindFile("wumtool.arx")){
		p=ff.GetFileName();
		return p;
	}else {
		char path[MAX_PATH];
		if(GetAutoCADPath(path)){
			p=path;
			p+="\\wumtool.arx";
			if(ff.FindFile(p)){
				return p;
			}
		}
	}
	static char szOpenFile[MAX_PATH];
	char szFileTitle[256];
	static OPENFILENAMEEX ofn={ sizeof(OPENFILENAME),NULL,NULL,
	"wumtool.arx\0*.arx",
	(LPSTR)NULL,0L,1,szOpenFile,sizeof(szOpenFile),szFileTitle,sizeof(szFileTitle),
	NULL,(LPSTR)NULL,OFN_FILEMUSTEXIST,0,0,"arx",
	NULL,NULL,NULL,0,0,0
	};
	if (GetOpenFileName((LPOPENFILENAME)&ofn)){
		return p=szOpenFile;
	}
	return p;
}
static BOOL setwumtoolautoload(BOOL autoload){
	HKEY hKey = NULL,hKey2;DWORD dw=6;
	/////////////////////确认可以加载
	{
		HKEY hLoadKey;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Autodesk\\AutoCAD\\R15.0\\ACAD-1:804\\Applications\\AcadAppload")
			,0, KEY_READ|KEY_WRITE,&hLoadKey);
		RegSetValueEx(hLoadKey,"LOADCTRLS",0,REG_DWORD,(LPBYTE)&dw, sizeof(DWORD));
		RegCloseKey(hLoadKey);	
	}
////////////////////////////
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software\\Autodesk\\AutoCAD\\R15.0\\ACAD-1:804\\Profiles\\<<Unnamed Profile>>\\Dialogs"), 0, KEY_READ|KEY_WRITE,
		&hKey) != ERROR_SUCCESS)return FALSE;
	if (RegCreateKeyEx(hKey, _T("AppLoad"), 0, REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_READ|KEY_WRITE,NULL,&hKey2,&dw) != ERROR_SUCCESS)return FALSE;
	RegCloseKey(hKey);
	if (RegCreateKeyEx(hKey2, _T("Startup"), 0, REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_READ|KEY_WRITE,NULL,&hKey,&dw) == ERROR_SUCCESS)
//	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software\\Autodesk\\AutoCAD\\R15.0\\ACAD-1:804\\Profiles\\<<Unnamed Profile>>\\Dialogs\\Appload\\Startup"), 0, KEY_READ|KEY_WRITE,
//		&hKey) == ERROR_SUCCESS)
	{
		CString strValue;long NumStartup=0;
		DWORD dwType;DWORD dwCount;LONG lResult;
		lResult=RegQueryValueEx(hKey,"NumStartup" , NULL, &dwType,
			NULL, &dwCount);
		RegCloseKey(hKey2);
		if (lResult == ERROR_SUCCESS)
		{
			lResult=RegQueryValueEx(hKey,"NumStartup" , NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			char*send;
			NumStartup=strtol(strValue,&send,10);
			if(NumStartup>0){
				CString ValueName;
				for(int i=1;i<=NumStartup;i++){
					ValueName.Format("%dStartup",i);
					lResult=RegQueryValueEx(hKey,ValueName , NULL, &dwType,
						NULL, &dwCount);
					if (lResult == ERROR_SUCCESS)
					{
						lResult=RegQueryValueEx(hKey,ValueName , NULL, &dwType,
							(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
						strValue.ReleaseBuffer();
					}
					if (lResult == ERROR_SUCCESS)
					{	
						//////////////////////
						if(strValue.Right(11).CollateNoCase("wumtool.arx")==0)
						{
							if(!autoload){
							////////////从自动中删除
								if(i<NumStartup)
								{
									CString nextvaluename;
									///把值前移
									nextvaluename.Format("%dStartup",NumStartup);
									lResult=RegQueryValueEx(hKey,nextvaluename, NULL, &dwType,
										NULL, &dwCount);
									if (lResult == ERROR_SUCCESS)
									{
										lResult=RegQueryValueEx(hKey,nextvaluename, NULL, &dwType,
											(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
										strValue.ReleaseBuffer();
									}
									if(lResult==ERROR_SUCCESS)
									{
										lResult=RegSetValueEx(hKey,ValueName,NULL,dwType,
											(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)),dwCount);
										ValueName=nextvaluename;
									}

								}
								///删除最后一个
								lResult=RegDeleteValue(hKey,ValueName);
								NumStartup--;
								strValue.Format("%d",NumStartup);
								lResult=RegSetValueEx(hKey,"NumStartup" , NULL, REG_SZ,
									(LPBYTE)strValue.GetBuffer(strValue.GetLength()), strValue.GetLength());
							}
							RegCloseKey(hKey);
							if(lResult==ERROR_SUCCESS)return TRUE;
							else return FALSE;
						}
						////////////////////////
					}
				}

			}


		}
		if(autoload){
			////////////添加自动到最后一个	
			CString strValue,str;
			NumStartup++;
			str=getmtoolpath();
			strValue.Format("%d",NumStartup);
			lResult=RegSetValueEx(hKey,"NumStartup" , NULL, REG_SZ,
				(LPBYTE)strValue.GetBuffer(strValue.GetLength()), strValue.GetLength());
			strValue.Format("%dStartup",NumStartup);
			lResult=RegSetValueEx(hKey,strValue,NULL,REG_SZ,
				(LPBYTE)str.GetBuffer(str.GetLength()),str.GetLength());
		}
		RegCloseKey(hKey);
		///////////////////////////////
		if(lResult==ERROR_SUCCESS)return TRUE;
		else return FALSE;
	}
	return FALSE;
}
void FillInPropertyPage( PROPSHEETPAGE* psp, int idDlg, LPSTR pszProc, DLGPROC pfnDlgProc)
{
    psp->dwSize = sizeof(PROPSHEETPAGE);
    psp->dwFlags = PSP_DEFAULT;
    psp->hInstance = NULL;
    psp->pszTemplate = MAKEINTRESOURCE(idDlg);
    psp->pszIcon = NULL;
    psp->pfnDlgProc = pfnDlgProc;
    psp->pszTitle = pszProc;
    psp->lParam = 0;

}
int CALLBACK	PropertyGeneralPageProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		pmgeneral=0;
		::SendDlgItemMessage(hDlg,IDC_AUTO_SAVE_PARAM,BM_SETCHECK,AppParam.bAutoSaveParam,0);
	//	((CButton*)GetDlgItem(IDC_AUTO_LOAD_WMTOOL))->SetCheck(AppParam.bAutoRunWMtool);
		::SendDlgItemMessage(hDlg,IDC_AUTO_LOAD_WMTOOL,BM_SETCHECK,/*1*/getwumtoolautoload(),0);
		::SendDlgItemMessage(hDlg,IDC_READ_CONTINUE_WHEN_FILE_HAVE_3DENTITY,BM_SETCHECK,AppParam.bReadContinueOn3dEntity,0 );
		::SendDlgItemMessage(hDlg,IDC_CHECK_SELFX_ON_BUILD_LOOP,BM_SETCHECK,AppParam.bCheckSelfXOnLoopBuild ,0);
		::SendDlgItemMessage(hDlg,IDC_CHECK_SELFX_AFTER_BUILD_LOOP,BM_SETCHECK,AppParam.bCheckSelfXAfterLoopBuild ,0);
		::SendDlgItemMessage(hDlg,IDC_CHECK_SELFX_AFTER_OFFSET,BM_SETCHECK,AppParam.bCheckSelfXAfterOffset,0 );
		::SendDlgItemMessage(hDlg,IDC_CHECK_COX_AFTER_OFFSET,BM_SETCHECK,AppParam.bCheckCoXAfterOffset,0);		
		SetDlgItemInt(hDlg,IDC_DIST_O1_O,AppParam.lDist_O1_O,TRUE);
		SendDlgItemMessage(hDlg,IDC_AJUST_CENTER_ET,BM_SETCHECK,AppParam.bCenterAjustEveryTime,0);
		return TRUE; // indicate we did not set focus to the control

	case WM_NOTIFY:
		switch (((NMHDR FAR *) lParam)->code)
		{
		case PSN_APPLY:
			{
				AppParam.bAutoSaveParam=SendDlgItemMessage(hDlg,IDC_AUTO_SAVE_PARAM,BM_GETCHECK,0,0);
				AppParam.bAutoRunWMtool=SendDlgItemMessage(hDlg,IDC_AUTO_LOAD_WMTOOL,BM_GETCHECK,0,0);
				setwumtoolautoload(AppParam.bAutoRunWMtool);
				AppParam.bReadContinueOn3dEntity=
					SendDlgItemMessage(hDlg,IDC_READ_CONTINUE_WHEN_FILE_HAVE_3DENTITY,BM_GETCHECK,0,0 );
				int bv;
				bv=(unsigned char)SendDlgItemMessage(hDlg,IDC_CHECK_COX_AFTER_OFFSET,BM_GETCHECK,0,0 );	
				if(bv!=AppParam.bCheckCoXAfterOffset){
					AppParam.bCheckCoXAfterOffset=bv;
					pmgeneral=2;
				}
				bv=(unsigned char)SendDlgItemMessage(hDlg,IDC_CHECK_SELFX_AFTER_OFFSET,BM_GETCHECK,0,0 );
				if(bv!=AppParam.bCheckSelfXAfterOffset){
					AppParam.bCheckSelfXAfterOffset=bv;
					pmgeneral=2;
				}
				bv=(unsigned char)SendDlgItemMessage(hDlg,IDC_CHECK_SELFX_AFTER_BUILD_LOOP,BM_GETCHECK,0,0 );
				if(bv!=AppParam.bCheckSelfXAfterLoopBuild){
					AppParam.bCheckSelfXAfterLoopBuild=bv;
					pmgeneral=3;
				}
				bv=(unsigned char)SendDlgItemMessage(hDlg,IDC_CHECK_SELFX_ON_BUILD_LOOP,BM_GETCHECK,0,0 );
				if(bv!=AppParam.bCheckSelfXOnLoopBuild){
					AppParam.bCheckSelfXOnLoopBuild=bv;
					pmgeneral=3;
				}
				{
				AppParam.lDist_O1_O=GetDlgItemInt(hDlg,IDC_DIST_O1_O,&bv,TRUE);
				AppParam.bCenterAjustEveryTime=SendDlgItemMessage(hDlg,IDC_AJUST_CENTER_ET,BM_GETCHECK,0,0);
				}

			}
			return TRUE;
		}
		return FALSE;
	default: return FALSE;
	};

}


int CALLBACK	__DriverEditDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			__DRIVER * pd=(__DRIVER*) lParam;
			SetDlgItemText(hDlg,IDC_DRIVER_NAME,pd->name);
			SetDlgItemText(hDlg,IDC_FILE_NAME,pd->file);
			SetDlgItemText(hDlg,IDC_DRIVER_INFO,pd->info);
			SetWindowLong(hDlg,GWL_USERDATA,lParam);
			
		}
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				__DRIVER * pd=(__DRIVER*)GetWindowLong(hDlg,GWL_USERDATA);
				GetDlgItemText(hDlg,IDC_DRIVER_NAME,pd->name,16);
				GetDlgItemText(hDlg,IDC_FILE_NAME,pd->file,MAX_PATH);
				GetDlgItemText(hDlg,IDC_DRIVER_INFO,pd->info,256);
				EndDialog(hDlg,1);
			}
			break;
		case IDCANCEL:
			EndDialog(hDlg,0);
			break;
		case IDC_BROWSE:
			{
				char szOpenFile[MAX_PATH]="\0";
				char szFileTitle[_MAX_FNAME]="\0";
				char szFilter[]="DLL文件(*.dll)\0*.dll\0所有文件(*.*)\0*.*\0\0";
				OPENFILENAMEEX ofn;
				ZeroMemory(&ofn,sizeof(OPENFILENAMEEX));
				ofn.lStructSize=sizeof(OPENFILENAME);
				ofn.hwndOwner=hDlg;
				ofn.lpstrFile=szOpenFile;
				ofn.nMaxFile=sizeof(szOpenFile); 
				ofn.lpstrFileTitle=szFileTitle;; 
				ofn.nMaxFileTitle=sizeof(szFileTitle); 
				ofn.Flags=OFN_OVERWRITEPROMPT; 
				ofn.nFilterIndex=1;
				ofn.lpstrFilter=szFilter;
				ofn.lpstrDefExt="dll";
				if(!GetOpenFileName((LPOPENFILENAME)&ofn))break;
				SetDlgItemText(hDlg,IDC_FILE_NAME,szOpenFile);
			}
			break;
		case IDC_PARAM:
			{
				char path[MAX_PATH];
				GetDlgItemText(hDlg,IDC_FILE_NAME,path,MAX_PATH);


			}
			break;
		}
		return TRUE;

	}
	return FALSE;
	
}
int __GetDriverContext(int&kMax,int&iCurrent)
{
	HKEY hDrvKey;int iResult=0;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("software\\中科大现代制造技术实验室\\Rapid Prototype\\Driver"), 
		0, KEY_WRITE|KEY_READ,&hDrvKey) == ERROR_SUCCESS)
	{
		DWORD lResult,dwType,dwValue,dwCount=sizeof(DWORD);
		lResult=RegQueryValueEx(hDrvKey,"nMax" , NULL, &dwType,(LPBYTE)&dwValue, &dwCount);
		if(lResult==ERROR_SUCCESS)
		{
			kMax=dwValue;	
			lResult=RegQueryValueEx(hDrvKey,"Current" , NULL, &dwType,(LPBYTE)&dwValue, &dwCount);
			if(lResult==ERROR_SUCCESS)
			{
				iCurrent=dwValue;
				iResult= 1;
			}
		}
	}
	RegCloseKey(hDrvKey);
	return iResult;
}
static int __SetDriverContext(int kMax,int iCurrent)
{
	HKEY hAppKey=MyGetAppRegistryKey(HKEY_CURRENT_USER);
	HKEY hDrvKey;int iResult=0;
	if (RegCreateKeyEx(hAppKey,	_T("Driver"), 
		0, NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE|KEY_READ,NULL,&hDrvKey,NULL) == ERROR_SUCCESS)
	{
		RegSetValueEx(hDrvKey,"nMax" , NULL, REG_DWORD,(LPBYTE)&kMax, sizeof(DWORD));
		RegSetValueEx(hDrvKey,"Current" , NULL, REG_DWORD,(LPBYTE)&iCurrent, sizeof(DWORD));
	}
	RegCloseKey(hDrvKey);
	RegCloseKey(hAppKey);
	return iResult;
}
int __GetDriver(int index, __DRIVER* pd)
{
	HKEY hDrvKey,hKey;int iResult=0;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("software\\中科大现代制造技术实验室\\Rapid Prototype\\Driver"), 
		0, KEY_WRITE|KEY_READ,&hDrvKey) == ERROR_SUCCESS)
	{

		DWORD lResult,dwType,dwCount=sizeof(DWORD);
		char keyname[256];
		sprintf(keyname,"Driver%d",index);
		if (RegOpenKeyEx(hDrvKey,keyname,0, KEY_WRITE|KEY_READ,&hKey) == ERROR_SUCCESS)
		{
			dwType=REG_SZ;
			dwCount=16;
			lResult=RegQueryValueEx(hKey,"DriverName" , NULL, &dwType,(LPBYTE )pd->name, &dwCount);
			if(lResult==ERROR_SUCCESS)
			{
				dwCount=256;
				lResult=RegQueryValueEx(hKey,"DriverInfo",NULL,&dwType,(LPBYTE )pd->info,&dwCount);
				{
					if(lResult==ERROR_SUCCESS)
					{
						dwCount=MAX_PATH;
						lResult=RegQueryValueEx(hKey,"FileName",NULL,&dwType,(LPBYTE )pd->file,&dwCount);
						{
							if(lResult==ERROR_SUCCESS)
							{
								iResult=1;
							}
						}
					}
				}
			}
			RegCloseKey(hKey);
		}
		RegCloseKey(hDrvKey);

	}

	return iResult;
}
static int __SetDriver(int index, __DRIVER*pd)
{
	HKEY hAppKey=MyGetAppRegistryKey(HKEY_CURRENT_USER);
	HKEY hDrvKey,hKey;
	int iResult=0;
	if (RegCreateKeyEx(hAppKey,	_T("Driver"), 
		0, NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE|KEY_READ,NULL,&hDrvKey,NULL) == ERROR_SUCCESS)
	{
		char keyname[256];
		sprintf(keyname,"Driver%d",index);
		if (RegCreateKeyEx(hDrvKey,keyname,0,NULL,REG_OPTION_NON_VOLATILE,
			KEY_WRITE|KEY_READ,NULL,&hKey,NULL) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey,"DriverName",NULL,REG_SZ,(LPBYTE)pd->name,16);
			RegSetValueEx(hKey,"DriverInfo",NULL,REG_SZ,(LPBYTE)pd->info,256);
			RegSetValueEx(hKey,"FileName",NULL,REG_SZ,(LPBYTE)pd->file,MAX_PATH);
			RegCloseKey(hKey);
		}
	}
	RegCloseKey(hDrvKey);
	RegCloseKey(hAppKey);
	return iResult;
}
int CALLBACK	__PropertyDriverPageProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			int idriver,ilist,kMax;
			__DRIVER drv,*pd;
			int iCurrent=0;
			SetDlgItemText(hDlg,IDC_CURRENT,"无硬件");
			EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_EDIT_CURRENT),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_PARAM),FALSE);
			SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_ADDSTRING, 0,(LPARAM)"无硬件");
			SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETITEMDATA, 0,0);
			if(__GetDriverContext(kMax,iCurrent))
			{
				if(kMax>0)
				{
					for(idriver=1;idriver<kMax;idriver++)
					{
						if(__GetDriver(idriver, &drv))
						{
							pd=new __DRIVER(drv);
							pd->iscurrent=0;
							ilist=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_ADDSTRING,0,(LPARAM)drv.name);
							SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETITEMDATA,ilist,(WPARAM)pd);
							if(idriver==iCurrent)
							{
								pd->iscurrent=1;
								SetDlgItemText(hDlg,IDC_CURRENT,pd->name);
								SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETCURSEL,ilist,0);
								EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_EDIT_CURRENT),TRUE);
								EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_PARAM),TRUE);
							}
						}		
					}

				}
			
			}
			SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(IDC_DRIVERS,LBN_SELCHANGE),NULL);
			return TRUE;
		}
	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_DRIVER_DEL:
			{
				int i=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCURSEL,0,0);
				if(i==0)return TRUE;
				__DRIVER*pd=(__DRIVER*)SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETITEMDATA,0,0);
				if(pd) 
				{
					if(pd->iscurrent)
					{
						if(MessageBox(hDlg,"要删除的是当前驱动，是不是继续？","删除确认",MB_YESNO)==IDYES)
						{
							SetDlgItemText(hDlg,IDC_CURRENT,"无硬件");
							EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_EDIT_CURRENT),FALSE);
							EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_PARAM),FALSE);
							delete[](BYTE*)pd;
							SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_DELETESTRING,i,0);
						}
					}else
					{
						delete[](BYTE*)pd;
						SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_DELETESTRING,i,0);
					}
				}else
				{
						SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_DELETESTRING,i,0);
				}
				SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETCURSEL,0,0);
				SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(IDC_DRIVERS,LBN_SELCHANGE),NULL);
			}
			break;
		case IDC_DRIVER_ADD:
			{
				__DRIVER *pd=new __DRIVER;
				pd->info[0]=0;pd->iscurrent=0;pd->file[0]=0;
				int index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCOUNT,0,0);
				do
				{
					sprintf(pd->name,"方案%d",index);
					index++;
				}while(SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_FINDSTRING,-1,LPARAM(pd->name))!=LB_ERR );
				index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_ADDSTRING,0,LPARAM(pd->name));
				SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETITEMDATA,index,LPARAM(pd));
				SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETCURSEL,index,0);
				SendMessage(hDlg,WM_COMMAND,IDC_DRIVER_EDIT,0);
				SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(IDC_DRIVERS,LBN_SELCHANGE),NULL);
			}
			break;
		case IDC_DRIVER_EDIT_CURRENT:
			{
				char name[16];
				GetDlgItemText(hDlg,IDC_CURRENT,name,16);
				int index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_FINDSTRING,-1,(LPARAM)name);
				SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETCURSEL,index,0);
				SendMessage(hDlg,WM_COMMAND,IDC_DRIVER_EDIT,0);
				break;
			}
		case IDC_DRIVER_EDIT:
			{
				int index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCURSEL,0,0);
				int count=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCOUNT,0,0);
				if(index<=0||index>=count)break;
				__DRIVER*pd=(__DRIVER*)SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETITEMDATA,index,0);
				__DRIVER newdrv(*pd);
				if(DialogBoxParam(NULL,MAKEINTRESOURCE(IDD_DRIVER_EDIT),hDlg,__DriverEditDlgProc,(LPARAM)&newdrv))
				{
					
					if(SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_FINDSTRING,-1,(WPARAM)newdrv.name)==LB_ERR)
					{
						strcpy(pd->name,newdrv.name);
						SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_DELETESTRING,index,0);
						SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_INSERTSTRING,index,(LPARAM)pd->name);
						SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_SETITEMDATA,index,(LPARAM)pd);
						if(pd->iscurrent)
						{		
							SetDlgItemText(hDlg,IDC_CURRENT,pd->name);
						}
					}
					strcpy(pd->info,newdrv.info);
					strcpy(pd->file,newdrv.file);
				}
				SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(IDC_DRIVERS,LBN_SELCHANGE),NULL);
			}
			break;
		case IDC_DRIVER_SET:
			{
				int index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCURSEL,0,0);
				int count=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCOUNT,0,0);
				if(index<0||index>=count)break;
				__DRIVER*pd=(__DRIVER*)SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETITEMDATA,index,0);
				if(index==0)
				{
					ASSERT(pd==NULL);
					SetDlgItemText(hDlg,IDC_CURRENT,"无硬件");
					EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_EDIT_CURRENT),FALSE);
					EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_PARAM),FALSE);


				}
				else
				{
					ASSERT(pd);
					SetDlgItemText(hDlg,IDC_CURRENT,pd->name);
					EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_EDIT_CURRENT),TRUE);
					EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_PARAM),TRUE);
				}
			}
			break;
		case IDC_DRIVER_PARAM:
			{
				HINSTANCE HRPC;
				char name[16];
				GetDlgItemText(hDlg,IDC_CURRENT,name,16);
				int index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_FINDSTRING,-1,(LPARAM)name);
				__DRIVER* pd=(__DRIVER*)SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETITEMDATA,index,0);
				if(pd!=NULL&&(HRPC=LoadLibrary(pd->file))!=NULL)
				{
					void (* FillCardPropertyPage)(LPPROPSHEETPAGE,LPARAM);
					int Modified;
					FillCardPropertyPage=(void(*)(LPPROPSHEETPAGE,LPARAM))GetProcAddress(HRPC, "FillCardPropertyPage");
					PROPSHEETPAGE psp[1];
					PROPSHEETHEADER psh;
					FillCardPropertyPage(psp,(LPARAM)&Modified);
					psh.dwSize = sizeof(PROPSHEETHEADER);
					psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
					psh.hwndParent =hDlg;
					psh.pszCaption = (LPSTR) TEXT("参数设置");
					psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
					psh.nStartPage = 0;
					psh.ppsp = (LPCPROPSHEETPAGE) &psp;
					PropertySheet(&psh);				
					FreeLibrary(HRPC);
				}
				else
				{
					AfxMessageBox("找不到驱动文件!");

				}
			}
				break;
		case MAKEWPARAM(IDC_DRIVERS,LBN_SELCHANGE):
			{

				int i=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCURSEL,0,0);
				EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_DEL),i>0);
				EnableWindow(GetDlgItem(hDlg,IDC_DRIVER_EDIT),i>0);
				if(i==-1 || i==0)
				{
					SetDlgItemText(hDlg,IDC_INFO,"不使用任何硬件");
				}
				else
				{
					__DRIVER*pd=(__DRIVER*)SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETITEMDATA,i,0);
					SetDlgItemText(hDlg,IDC_INFO,pd->info);
				}
				break;
			}

	
		}
		return TRUE;
	case WM_NOTIFY:
		switch (((NMHDR FAR *) lParam)->code)
		{
		case PSN_APPLY:
			{
				int index=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCURSEL,0,0);
				int count=SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETCOUNT,0,0);
				__DRIVER*pd;
				__SetDriverContext(count,index);
				for(int i=1;i<count;i++)
				{
					pd=(__DRIVER*)SendDlgItemMessage(hDlg,IDC_DRIVERS,LB_GETITEMDATA,i,0);
					__SetDriver(i,pd);
				}

			}
			return TRUE;
		}
		return FALSE;
	default: return FALSE;
	};

}
#define fabs(X) (X>0?X:-X)
#define ZERO(X) (fabs(X)<0.0001)
#define PEQU(X,Y) ZERO((X-Y))
#define PI ((float)3.1415926535897932384626433832795)

int CALLBACK	PropertyGraphicPageProc(HWND hDlg,UINT uMsg,/* message*/
  WPARAM wParam, /* first message parameter*/LPARAM lParam  /* second message parameter*/)
{
	int i;int bv;float d;
	char  str[16],*send;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		pmgraphic=0;
		if(AppParam.bCncOrder==0x01)i = 0;
		else if(AppParam.bCncOrder==0x02)i=1;
		else if(AppParam.bCncOrder==0x06)i=2;
		else i=3;
		SendDlgItemMessage(hDlg,IDC_SCAN_CONTOUR+i,BM_SETCHECK,1,0);
		SendDlgItemMessage(hDlg,IDC_LINE_CROSS,BM_SETCHECK,AppParam.bCncLineCross,0);
		if(AppParam.fScale==AppParam.fScalez)
		{
			SendDlgItemMessage(hDlg,IDC_SAMESCALE,BM_SETCHECK,TRUE,0);
			EnableWindow(GetDlgItem(hDlg,IDC_ZOOM_SCALEZ),FALSE);
		}
		sprintf(str,"%.4f",AppParam.fScale);SetDlgItemText(hDlg,IDC_ZOOM_SCALE,str);
		sprintf(str,"%.4f",AppParam.fScalez);SetDlgItemText(hDlg,IDC_ZOOM_SCALEZ,str);
		sprintf(str,"%.4f",AppParam.fLayerDist);SetDlgItemText(hDlg,IDC_LAYER_DIST,str);
		sprintf(str,"%.4f",AppParam.fOffsetDist);SetDlgItemText(hDlg,IDC_OFFSET_DIST,str);
		sprintf(str,"%.4f",AppParam.fEstep);SetDlgItemText(hDlg,IDC_EMULATE_SPLIT_LEN,str);
		sprintf(str,"%.4f",AppParam.fCncLineDist);SetDlgItemText(hDlg,IDC_CNC_LINE_DIST,str);
		sprintf(str,"%.4f",AppParam.fCncLenMin );SetDlgItemText(hDlg,IDC_CNC_LEN_MIN,str);
		sprintf(str,"%.4f",AppParam.fCncAngleDelta/PI*180);SetDlgItemText(hDlg,IDC_LINEANGLE_DELTA,str);
		sprintf(str,"%.4f",AppParam.fCncFirstAngle/PI*180);SetDlgItemText(hDlg,IDC_LINEANGLE_FIRST,str);
		::SetDlgItemInt(hDlg,IDC_FREEBACK_INTERVAL,AppParam.ifreebackinterval,TRUE);
		return TRUE; // indicate we did not set focus to the control
	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_SAMESCALE:
			if(::SendDlgItemMessage(hDlg,IDC_SAMESCALE,BM_GETCHECK,0,0))
				EnableWindow(GetDlgItem(hDlg,IDC_ZOOM_SCALEZ),FALSE);
			else
				EnableWindow(GetDlgItem(hDlg,IDC_ZOOM_SCALEZ),TRUE);
		case MAKEWPARAM(IDC_ZOOM_SCALE,EN_CHANGE):
			if(::SendDlgItemMessage(hDlg,IDC_SAMESCALE,BM_GETCHECK,0,0))
			{
				char str[32];
				GetDlgItemText(hDlg,IDC_ZOOM_SCALE,str,32);
				SetDlgItemText(hDlg,IDC_ZOOM_SCALEZ,str);
			}
			return TRUE;

		default:
			return FALSE;
		}
	case WM_NOTIFY:
		switch (((NMHDR FAR *) lParam)->code)
		{
		case PSN_APPLY:
		{
			for(i=0;i<4;i++)
				if(SendDlgItemMessage(hDlg,IDC_SCAN_CONTOUR+i,BM_GETCHECK,0,0))break;
			if(i==0)bv=0x01;else if(i==1)bv=0x02;else if(i==3)bv=0x09;else bv=0x06;
			if(bv!=AppParam.bCncOrder){pmgraphic=1;AppParam.bCncOrder=bv;}

			bv=(unsigned char)SendDlgItemMessage(hDlg,IDC_LINE_CROSS,BM_GETCHECK,0,0);
			if(bv!=AppParam.bCncLineCross){
				pmgraphic=1;
				AppParam.bCncLineCross=bv;
			}
			GetDlgItemText(hDlg,IDC_CNC_LINE_DIST,str,16);d=(float)strtod(str,&send);
			if(d<1||d>2000)goto dataerror;
			if(pmgraphic==1)AppParam.fCncLineDist=d;
			else if(!PEQU(AppParam.fCncLineDist,d)){pmgraphic=1;AppParam.fCncLineDist=d;}

			GetDlgItemText(hDlg,IDC_CNC_LEN_MIN,str,16);d=(float)strtod(str,&send);
			if(d<0.001||d>2000)goto dataerror;
			if(pmgraphic==1)AppParam.fCncLineDist=d;
			else if(!PEQU(AppParam.fCncLenMin,d)){pmgraphic=1;AppParam.fCncLenMin=d;}

			GetDlgItemText(hDlg,IDC_LINEANGLE_DELTA,str,16);d=(float)strtod(str,&send);
			if(d<=-90||d>90)goto dataerror;d=d*PI/180;
			if(pmgraphic==1)AppParam.fCncAngleDelta=d;
			else if(!PEQU(AppParam.fCncAngleDelta,d)){pmgraphic=1;AppParam.fCncAngleDelta=d;}

			GetDlgItemText(hDlg,IDC_LINEANGLE_FIRST,str,16);d=(float)strtod(str,&send);
			if(d<0||d>=180)goto dataerror;d=d*PI/180;
			if(pmgraphic==1)AppParam.fCncFirstAngle=d;
			else if(!PEQU(AppParam.fCncFirstAngle,d)){pmgraphic=1;AppParam.fCncFirstAngle=d;}

			GetDlgItemText(hDlg,IDC_LAYER_DIST,str,16);d=(float)strtod(str,&send);
			if(d<0.001||d>10000)goto dataerror;AppParam.fLayerDist=d;
			GetDlgItemText(hDlg,IDC_EMULATE_SPLIT_LEN,str,16);d=(float)strtod(str,&send);
			if(d<1||d>1000)goto dataerror;AppParam.fEstep=d;	

			GetDlgItemText(hDlg,IDC_OFFSET_DIST,str,16);d=(float)strtod(str,&send);
			if(d<1||d>1000)goto dataerror;
			if(!PEQU(AppParam.fOffsetDist,d)){pmgraphic=2;AppParam.fOffsetDist=d;}

			GetDlgItemText(hDlg,IDC_ZOOM_SCALE,str,16);d=(float)strtod(str,&send);
			if(d<0.00001||d>10000)goto dataerror;
			if(!PEQU(AppParam.fScale,d)){pmgraphic=3;AppParam.fScale=d;}

			GetDlgItemText(hDlg,IDC_ZOOM_SCALEZ,str,16);d=(float)strtod(str,&send);
			if(d<0.00001||d>10000)goto dataerror;
			if(!PEQU(AppParam.fScalez,d)){pmgraphic=3;AppParam.fScalez=d;}

			
			i=::GetDlgItemInt(hDlg,IDC_FREEBACK_INTERVAL,&bv,TRUE);
			if(bv&&i>1&&i<100)AppParam.ifreebackinterval=i;
		
			return TRUE;
		dataerror:
			MessageBox(hDlg,"输入数据错误!","rp",MB_OK);
			return FALSE;
		}
		}
		return FALSE;
	default: return FALSE;
	};

}

void OnParamsTest(int* pModified){
	PROPSHEETPAGE psp[4];
    PROPSHEETHEADER psh;
	HINSTANCE DLLInst = NULL;
	FillInPropertyPage( psp, IDD_PARAM_GENERAL, NULL, PropertyGeneralPageProc);
	FillInPropertyPage( psp+1, IDD_PARAM_GRAPHIC, NULL, PropertyGraphicPageProc);
//	FillInPropertyPage(psp+2,IDD_PARAM_DRIVER,NULL,PropertyDriverPageProc);
	FillInPropertyPage(psp+2,IDD_DRIVER_MANAGE,NULL,__PropertyDriverPageProc);
//	if(InitRpcDll())
//		FillCardPropertyPage(psp+2,(LPARAM)(pModified+3));
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
    psh.hwndParent =AfxGetMainWnd()->m_hWnd;
    psh.pszCaption = (LPSTR) TEXT("参数设置");
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 1;
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
	PropertySheet(&psh);
	pModified[0]=pmgeneral;
	pModified[1]=pmgraphic;

}
int  CALLBACK ZeroDlgProc(HWND hDlg,/*handle to dialog box*/UINT uMsg,/* message*/
  WPARAM wParam, /* first message parameter*/LPARAM lParam  /* second message parameter*/)
{
	static int haveaction;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			long pos[3];
			haveaction=FALSE;
			RP_ZeroAbsolutePos(pos);
			pos[0]=-AppParam.lDist_O1_O-pos[0];
			pos[1]=-pos[1];pos[2]=-pos[2];
			for(int i=0;i<3;i++)SetDlgItemInt(hDlg,IDC_POS1+i,pos[i],TRUE);
			SetTimer(hDlg,1,110,NULL);
			return TRUE;
		}
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
		case IDCANCEL:
			if(haveaction)return TRUE;
			EndDialog(hDlg,wParam);
			return TRUE;
		case IDC_SETHOME:
			RP_SetZero();
			zero_ok=TRUE;
			return TRUE;
		case IDC_START_PROFILE:
			{
				long pos[3];BOOL trans;
				for(int i=0;i<3;i++)pos[i]=GetDlgItemInt(hDlg,IDC_POS1+i,&trans,TRUE);
				RP_MoveTo(pos);
				haveaction=TRUE;
			EnableWindow(GetDlgItem(hDlg,IDC_SETHOME),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDCANCEL   ),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDOK		),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_BAK_TO_HOME),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_START_PROFILE),FALSE);
				return TRUE;
			}
		case IDC_BAK_TO_HOME:
			{
				long pos[3];
				RP_ZeroAbsolutePos(pos);
				pos[0]=-AppParam.lDist_O1_O-pos[0];
				pos[1]=-pos[1];pos[2]=-pos[2];
				for(int i=0;i<3;i++)SetDlgItemInt(hDlg,IDC_POS1+i,pos[i],TRUE);		
				return TRUE;
			}
		case IDC_STOP_PROFILE:
			RP_StopMove();
			EnableWindow(GetDlgItem(hDlg,IDC_SETHOME),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDCANCEL   ),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDOK		),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_BAK_TO_HOME),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_START_PROFILE),TRUE);
			haveaction=FALSE;
			return TRUE;
		default:return FALSE;
		}
		return FALSE;
	case WM_TIMER:
		{
			long pos[3];
			if(haveaction){
				if(RP_CardState(pos)==0){
					EnableWindow(GetDlgItem(hDlg,IDC_SETHOME),TRUE);
					EnableWindow(GetDlgItem(hDlg,IDCANCEL   ),TRUE);
					EnableWindow(GetDlgItem(hDlg,IDOK		),TRUE);
					EnableWindow(GetDlgItem(hDlg,IDC_BAK_TO_HOME),TRUE);
					EnableWindow(GetDlgItem(hDlg,IDC_START_PROFILE),TRUE);
					haveaction=FALSE;
				}
			}else
				RP_CardState(pos);	
			SetDlgItemInt(hDlg,IDC_POS4,pos[0],TRUE);
			SetDlgItemInt(hDlg,IDC_POS5,pos[1],TRUE);
			SetDlgItemInt(hDlg,IDC_POS6,pos[2],TRUE);
			return TRUE;
		}
	default:return FALSE;
	}
	return FALSE;

}

BOOL ZeroAjust(){
	if(!InitRpcDll()||!RP_InitCards())return FALSE;
	HWND hparent=AfxGetMainWnd()->m_hWnd;
	int e=DialogBox(NULL,MAKEINTRESOURCE(IDD_ZEROAJUST_DLG),hparent,(DLGPROC)ZeroDlgProc);
	if(e==IDCANCEL)return FALSE;
	else if(!zero_ok)return FALSE;
	else return TRUE;
}
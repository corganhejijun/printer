// RP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RP.h"
#include "ChildFrm.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AppFileClose();
BOOL AppFileOpen(LPSTR szFile);
BOOL GetAutoCADPath(LPSTR path);

/////////////////////////
static char szFile[MAX_PATH];
char szFileTitle[256];
/////////////////////////////////////////////////////////////////////////////
// CRPApp

BEGIN_MESSAGE_MAP(CRPApp, CWinApp)
	//{{AFX_MSG_MAP(CRPApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILECLOSE, OnFileclose)
	ON_COMMAND(ID_EMULATE, OnEmulate)
	ON_COMMAND(ID_PRODUCE, OnProduce)
	ON_COMMAND(ID_CENTERAJUST, OnCenterAjust)
	ON_COMMAND(ID_PARAMS, OnParams)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateItems)
	ON_COMMAND(ID_CARD_TEST, OnCardTest)
	ON_COMMAND(ID_EMULATE_QUICK, OnEmulateQuick)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_COMMAND(ID_FILE_OPEN_WITH_CAD, OnFileOpenWithCad)
	ON_COMMAND(ID_EMULATE_PAUSE, OnEmulatePause)
	ON_COMMAND(ID_EMULATE_STOP, OnEmulateStop)
	ON_UPDATE_COMMAND_UI(ID_EMULATE_PAUSE, OnUpdateEmulatePause)
	ON_UPDATE_COMMAND_UI(ID_EMULATE_STOP, OnUpdateEmulateStop)
	ON_COMMAND(ID_PRODUCE_PAUSE, OnProducePause)
	ON_UPDATE_COMMAND_UI(ID_PRODUCE_PAUSE, OnUpdateProducePause)
	ON_COMMAND(ID_PRODUCE_STOP, OnProduceStop)
	ON_UPDATE_COMMAND_UI(ID_PRODUCE_STOP, OnUpdateProduceStop)
	ON_UPDATE_COMMAND_UI(ID_FILECLOSE, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_PRODUCE, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_PARAMS, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_CENTERAJUST, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_EMULATE, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_CARD_TEST, OnUpdateItems)
	ON_UPDATE_COMMAND_UI(ID_EMULATE_QUICK, OnUpdateItems)
	ON_COMMAND(ID_EXPORT_PUL, OnExportPul)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRPApp construction

CRPApp::CRPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRPApp object

CRPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRPApp initialization
BOOL InitRpcDll();
BOOL CRPApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("中科大现代制造技术实验室"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)
	InitParams();
	CMDIFrameWnd* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	////

//	InitRpcDll();
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 
	////////////
	HINSTANCE hInst = AfxGetResourceHandle();

	/////////////
	char path[MAX_PATH];
	::GetModuleFileName(hInst,path,MAX_PATH);
    int i = strlen(path) - 1;
	for(;path[i]!='\\';i--);
	path[i]='\0';
	::SetCurrentDirectory(path);

	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_RPTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_RPTYPE));

	// The main window has been initialized, so show and update it.
	m_nCmdShow=SW_SHOWMAXIMIZED;
	ShowChildWindow(VIEW_2D_SHAPE);	
	

	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	// create  2d 3d MDI child window

//	ShowChildWindow(VIEW_3D_DESIGN);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRPApp message handlers

int CRPApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	SaveParams();
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);
	return CWinApp::ExitInstance();
}

void CRPApp::OnFileNew() 
{
	/////////////////////////AUTOCAD
	char path[MAX_PATH];
	if(GetAutoCADPath(path))
		ShellExecute(NULL,"open","acad.exe",NULL,path,SW_SHOWNORMAL);
	
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX); 
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CRPApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRPApp message handlers


void CRPApp::OnFileOpen() 
{
		/////////////////////打开一个DXF文件 
		static char szOpenFile[MAX_PATH];
		static OPENFILENAMEEX ofn={ sizeof(OPENFILENAME),m_pMainWnd->m_hWnd,NULL,
		"所有支持文件(*.dxf,*.dwg)\0*.dxf;*.dwg\0AutoCAD 2000 DXF文件(*.dxf)\0*.dxf\0AutoCAD 2000 dwg文件(*.dwg)\0*.dwg\0所有文件(*.*)\0*.*\0\0",
		(LPSTR)NULL,0L,1,szOpenFile,sizeof(szOpenFile),szFileTitle,sizeof(szFileTitle),
		NULL,(LPSTR)NULL,OFN_FILEMUSTEXIST,0,0,"dxf",
		NULL,NULL,NULL
		};

		if (GetOpenFileName((LPOPENFILENAME)&ofn)){
			if(strcmp(szFile,szOpenFile)!=0){
				strcpy(szFile,szOpenFile);
				if(AppFileOpen(szFile)){
					((CMainFrame*)m_pMainWnd)->OnUpdateFrameTitle(FALSE);
					AddToRecentFileList(szFile);
				}else{
					if(	AfxMessageBox(
						IDS_OPENFILE_ERROR
						,MB_YESNO   )==IDYES)
					{
						CString path(szOpenFile);
						path=path.Left(path.ReverseFind('\\'));
						ShellExecute(NULL,"open",szFileTitle,
							NULL,path,SW_SHOWNORMAL);
					}
					strcpy(szFileTitle,"\0");
					((CMainFrame*)m_pMainWnd)->glRebuildView(VIEW_2D|VIEW_3D);
				}
			}
		}
}



void CRPApp::OnFileclose() 
{
	// TODO: Add your command handler code here
	AppFileClose();
	strcpy(szFileTitle,"\0");
	((CMainFrame*)m_pMainWnd)->OnUpdateFrameTitle(FALSE);
}



CWnd* CRPApp::ShowChildWindow(int ViewType)
{
	return ((CMainFrame*)m_pMainWnd)
		->ShowChildWindow(ViewType,m_hMDIMenu,m_hMDIAccel);
}





float CRPApp::GetProfileFloat(LPCTSTR lpszSection, LPCTSTR lpszEntry, float fDefault)
{
	float fv;
	int * pInt=(int*)(&fv);
	*pInt=GetProfileInt(lpszSection,lpszEntry,*((int*)(&fDefault)));
	return fv;
}

BOOL CRPApp::WriteProfileFloat(LPCTSTR lpszSection, LPCTSTR lpszEntry, float fValue)
{
	return WriteProfileInt(lpszSection,lpszEntry,*((int*)(&fValue)));
}

CDocument* CRPApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	CFileFind ffd;
	if(strcmp(szFile,lpszFileName)==0)return (CDocument*)1;
	else if(ffd.FindFile(lpszFileName)){
		if(AppFileOpen((LPSTR)lpszFileName)){
			ffd.FindNextFile();
			strcpy(szFile,lpszFileName);
			strcpy(szFileTitle,ffd.GetFileName());
			((CMainFrame*)m_pMainWnd)->OnUpdateFrameTitle(FALSE);
				AddToRecentFileList(lpszFileName);
		return (CDocument*)1;
		}
	}
	return NULL;
}




void CRPApp::OnFileOpenWithCad() 
{
	if(strlen(szFileTitle)==0){
		char path[MAX_PATH];
		if(GetAutoCADPath(path))
			ShellExecute(NULL,"open","acad.exe",NULL,path,SW_SHOWNORMAL);
	}else{
		CString path(szFile);
		path=path.Left(path.ReverseFind('\\'));
			ShellExecute(NULL,"open",szFileTitle,
				NULL,path,SW_SHOWNORMAL);


	}
	
}




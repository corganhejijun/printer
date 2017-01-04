// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RP.h"

#include "childfrm.h"
#include "MainFrm.h"
#include "ctrlbar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////// 
extern char szFileTitle[256];
int InitCards(BOOL m_AjustCenter);
int m_CreateViewType=VIEW_2D_SHAPE;
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_COMMAND_EX(CG_ID_VIEW_CONTROLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_CONTROLBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDC_POS_WATCH, OnPosWatch)
	ON_COMMAND(ID_CARD_DEBUG, OnCardDebug)
	ON_UPDATE_COMMAND_UI(ID_CARD_DEBUG, OnUpdateCardDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	for(int i=0;i<VIEW_TYPE_NUM;i++)ppFrame[i]=NULL;
//	m_bAutoMenuEnable =FALSE;

}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndMenuBar.CreateEx(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_RAISEDBORDER) ||
			!m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME))
	{
			TRACE0("Failed to create menubar\n");
			return -1;      // fail to create
	}		
	/////////toolbar
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Add true color tool bar
	m_wndToolBar.LoadTrueColorToolBar( 32, IDB_NORMAL, IDB_NORMAL, IDB_DISABLED);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize dialog bar m_ControlBar
	m_ControlBar.Create(this,(LPCTSTR)CG_IDD_CONTROLBAR,CSize(150,500),
		CG_ID_VIEW_CONTROLBAR);
	m_ControlBar.SetBarStyle(m_ControlBar.GetBarStyle()| CBRS_TOOLTIPS | CBRS_FLYBY);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDockingEx(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_ControlBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_ControlBar, AFX_IDW_DOCKBAR_LEFT);
	m_ControlBar.ShowWindow(SW_SHOW);
	//////
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	RecalcLayout(TRUE);
}




void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!
	CString strTitle;
	if(strlen(szFileTitle)>0){
		strTitle=szFileTitle;
		strTitle+=" - Rapid Prototype";}
	else strTitle="Rapid Prototype ";
	SetWindowText(strTitle);
}



CWnd* CMainFrame::ShowChildWindow(int ViewType,HMENU m_hMDIMenu,HACCEL m_hMDIAccel)
{
		int i=ViewType&0xf;
		if(i>=VIEW_TYPE_NUM||i<0)return NULL;
		m_CreateViewType=ViewType;
		if(ppFrame[i]==NULL){
			CChildFrame*pf=(CChildFrame*)CreateNewChild(
			RUNTIME_CLASS(CChildFrame), IDR_APPCHILD1+i, 
			m_hMDIMenu, m_hMDIAccel);
			ppFrame[i]=pf;
			pf->ppFrame=ppFrame+i;
			pf->ShowWindow(SW_SHOWNORMAL);
		}else{
			ppFrame[i]->ShowWindow(SW_RESTORE);
			ppFrame[i]->ActivateFrame();
		}
		return &(ppFrame[i]->m_wndView);
}

CChildFrame* CMainFrame::GetChildFrame(int ViewType)
{
		int i=ViewType&0xf;
		if(i>=VIEW_TYPE_NUM||i<0)return NULL;
		return ppFrame[i];
}


void CMainFrame::glRebuildView(int ViewTypes)
{
	if(ViewTypes==0)return;
	for(int i=0;i<VIEW_TYPE_NUM;i++){
		if(ppFrame[i]&&(ppFrame[i]->m_wndView.m_ViewType&ViewTypes))
			ppFrame[i]->m_wndView.glRebuild(FALSE);
	}	
}



void CMainFrame::glResetCordination(int ViewTypes)
{
	if(ViewTypes==0)return;
	for(int i=0;i<VIEW_TYPE_NUM;i++){
		if(ppFrame[i]&&(ppFrame[i]->m_wndView.m_ViewType&ViewTypes))
			ppFrame[i]->m_wndView.glResetCord();
	}
}

void CMainFrame::DestroyChildWindowsOnOpen()
{
	if(ppFrame[VIEW_3D_FORCAST&0x0f])ppFrame[VIEW_3D_FORCAST&0x0f]->DestroyWindow();
	if(ppFrame[VIEW_3D_DESIGN&0x0f])ppFrame[VIEW_3D_DESIGN&0x0f]->DestroyWindow();
	if(ppFrame[VIEW_3D_EXTREME&0x0f])ppFrame[VIEW_3D_EXTREME&0x0f]->DestroyWindow();
}




BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (m_wndMenuBar.TranslateFrameMessage(pMsg))
		return TRUE;
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}



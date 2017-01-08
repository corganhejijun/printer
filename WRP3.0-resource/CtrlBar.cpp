// CtrlBar.cpp : implementation file
//

#include "stdafx.h"
#include "RP.h"
#include "CtrlBar.h"

#include "afxocc.h"


#ifndef _AFX_NO_OCC_SUPPORT
#include <../include/ocdb.h>
#include <../src/mfc/occimpl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////
BOOL AppLayerChange(int LayerIndex);
/////////////////////////////////////////////////////////////////////////////
// CCtrlBar dialog


CCtrlBar::CCtrlBar() 
{
	//{{AFX_DATA_INIT(CCtrlBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ImageList.Create(IDB_IMAGELIST, 13, 1, RGB(0,255,0));
	SetBtnImageList (&m_ImageList);
}


void CCtrlBar::DoDataExchange(CDataExchange* pDX)
{
	CMyControlBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCtrlBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCtrlBar, CMyControlBar)
	//{{AFX_MSG_MAP(CCtrlBar)
	ON_EN_CHANGE(IDC_LAYERINDEX, OnChangeLayerindex)
	ON_NOTIFY(NM_CLICK, IDC_TREE_VIEW, OnClickTreeView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_VIEW, OnSelchangedTreeView)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_VIEW, OnDblclkTreeView)
	//}}AFX_MSG_MAP
#ifndef _AFX_NO_OCC_SUPPORT
		ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrlBar message handlers

#ifndef _AFX_NO_OCC_SUPPORT
LRESULT CCtrlBar::HandleInitDialog(WPARAM, LPARAM)
{
	Default();  // allow default to initialize first (common dialogs/etc)

	// create OLE controls
	COccManager* pOccManager = afxOccManager;
	if ((pOccManager != NULL) && (m_pOccDialogInfo != NULL))
	{
        /*
		if (!pOccManager->CreateDlgControls(this, m_lpszTemplateName,
			m_pOccDialogInfo))
		{
			TRACE0("Warning: CreateDlgControls failed during dialog bar init.\n");
			return FALSE;
		}
        */
	}

	return TRUE;
}
BOOL CCtrlBar::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo)
{
	m_pOccDialogInfo = pOccDialogInfo;
	return TRUE;
}
#endif

BOOL CCtrlBar::Create(CWnd *pParentWnd, LPCTSTR lpszTemplateName, CSize sizeDefault, UINT nID, DWORD dwStyle)
{
    ASSERT_VALID(pParentWnd);   // must have a parent
    ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

    // save the style
    m_dwStyle = dwStyle;
	m_dwStyle &= CBRS_ALL;
    m_sizeHorz = sizeDefault;
    m_sizeVert = sizeDefault;
    m_sizeFloat = sizeDefault;
	////////创建DIALOG
    CString wndclass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW),
        CreateSolidBrush(GetSysColor(COLOR_BTNFACE)), 0);
	if(!CreateDlg(lpszTemplateName, pParentWnd))return FALSE;
	SetDlgCtrlID(nID);
	SetWindowText("控制区");
	///////创建TREE项目
//	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, 
//		int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
//		HTREEITEM hParent/*TVI_ROOT*/ , HTREEITEM hInsertAfter/*TVI_LAST */ );
	HTREEITEM hitem;UINT mask=TVIF_TEXT|TVIF_PARAM;
	CTreeCtrl *ptree=(CTreeCtrl*)GetDlgItem(IDC_TREE_VIEW);
	hitem=ptree->InsertItem(mask,"文件图形",0,0,0,0,VIEW_3D_FILE,TVI_ROOT,TVI_LAST);
	hitem=ptree->InsertItem(mask,"三维视图",0,0,0,0,0x0f,TVI_ROOT,TVI_LAST);
	ptree->InsertItem(mask,"设计形体",0,0,0,0,VIEW_3D_DESIGN,hitem,TVI_LAST);
	ptree->InsertItem(mask,"加工极限形体",0,0,0,0,VIEW_3D_EXTREME,hitem,TVI_LAST);
	ptree->InsertItem(mask,"加工预测形体",0,0,0,0,VIEW_3D_FORCAST,hitem,TVI_LAST);
	ptree->InsertItem(mask,"加工状态",0,0,0,0,VIEW_3D_PROCESS,hitem,TVI_LAST);
	ptree->Expand(hitem,TVE_EXPAND);
	hitem=ptree->InsertItem(mask,"平面视图",0,0,0,0,0x0f,TVI_ROOT,TVI_LAST);
	ptree->InsertItem(mask,"平面轮廓",0,0,0,0,VIEW_2D_SHAPE,hitem,TVI_LAST);
	ptree->InsertItem(mask,"扫描路径",0,0,0,0,VIEW_2D_FORCAST,hitem,TVI_LAST);
	ptree->InsertItem(mask,"加工状态",0,0,0,0,VIEW_2D_PROCESS,hitem,TVI_LAST);
	ptree->Expand(hitem,TVE_EXPAND);


    return TRUE;
}

void CCtrlBar::OnChangeLayerindex() 
{
	CSpinButtonCtrl* pspin=(CSpinButtonCtrl*)GetDlgItem(IDC_LAYERSPIN);
	int LayerIndex=pspin->GetPos();
	AppLayerChange(LayerIndex);
}

void CCtrlBar::OnClickTreeView(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	CTreeCtrl*ptree=(CTreeCtrl*)GetDlgItem(IDC_TREE_VIEW);
//	HTREEITEM hitem=ptree->GetSelectedItem();
//	if(hitem==NULL)return;
//	((CRPApp*)AfxGetApp())->ShowChildWindow(
//		ptree->GetItemData(hitem) );
	
	*pResult = 0;
}

void CCtrlBar::OnSelchangedTreeView(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	((CRPApp*)AfxGetApp())->ShowChildWindow(
		pNMTreeView->itemNew.lParam );	
	
	*pResult = 0;
}

void CCtrlBar::OnDblclkTreeView(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTreeCtrl*ptree=(CTreeCtrl*)GetDlgItem(IDC_TREE_VIEW);
	HTREEITEM hitem=ptree->GetSelectedItem();
	if(hitem==NULL)return;
	((CRPApp*)AfxGetApp())->ShowChildWindow(
		ptree->GetItemData(hitem) );	
	*pResult = 0;
}

void CCtrlBar::SetPos(long x, long y)
{
	SetDlgItemInt(IDC_STATIC_X,x);
	SetDlgItemInt(IDC_STATIC_Y,y);

}

void CCtrlBar::SetPos(long z)
{
	SetDlgItemInt(IDC_STATIC_Z,z);
}

void CCtrlBar::SetGS(BOOL open)
{
	CStatic* pIcon=(CStatic*)GetDlgItem(IDC_STATIC_GATE);
	static HICON hIoff=::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_GATEOFF));
	static HICON hIon=::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_GATEON));
	if(open)pIcon->SetIcon(hIon);
	else pIcon->SetIcon(hIoff);
	pIcon->Invalidate();
}

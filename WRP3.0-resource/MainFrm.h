// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__7EC8B5C9_C347_4CF5_9558_4CCBBB5FB6FF__INCLUDED_)
#define AFX_MAINFRM_H__7EC8B5C9_C347_4CF5_9558_4CCBBB5FB6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "viewinc\mytoolbar.h"
#include "viewinc\menubar.h"
#include "ctrlbar.h"

class CChildFrame;
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

	CChildFrame * ppFrame[VIEW_TYPE_NUM];
// Operations 
public: 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CMYToolBar    m_wndToolBar;
	CMenuBar    m_wndMenuBar;
public:
	void DestroyChildWindowsOnOpen();
	void glResetCordination(int ViewTypes);
	void glRebuildView(int ViewTypes);
	CChildFrame* GetChildFrame(int ViewType);
	CWnd* ShowChildWindow(int ViewType,HMENU m_hMDIMenu,HACCEL m_hMDIAccel);
	CCtrlBar m_ControlBar;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPosWatch();
	afx_msg void OnCardDebug();
	afx_msg void OnUpdateCardDebug(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7EC8B5C9_C347_4CF5_9558_4CCBBB5FB6FF__INCLUDED_)

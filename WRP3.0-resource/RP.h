// RP.h : main header file for the RP application
//

#if !defined(AFX_RP_H__EE2810CD_B3C9_41F6_896C_B878A06D3268__INCLUDED_)
#define AFX_RP_H__EE2810CD_B3C9_41F6_896C_B878A06D3268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "const.h"
/////////////////////////////////////////////////////////////////////////////
// CRPApp:
// See RP.cpp for the implementation of this class
//

typedef struct tagOFNEX { 
  DWORD         lStructSize; 
  HWND          hwndOwner; 
  HINSTANCE     hInstance; 
  LPCTSTR       lpstrFilter; 
  LPTSTR        lpstrCustomFilter; 
  DWORD         nMaxCustFilter; 
  DWORD         nFilterIndex; 
  LPTSTR        lpstrFile; 
  DWORD         nMaxFile; 
  LPTSTR        lpstrFileTitle; 
  DWORD         nMaxFileTitle; 
  LPCTSTR       lpstrInitialDir; 
  LPCTSTR       lpstrTitle; 
  DWORD         Flags; 
  WORD          nFileOffset; 
  WORD          nFileExtension; 
  LPCTSTR       lpstrDefExt; 
  LPARAM        lCustData; 
  LPOFNHOOKPROC lpfnHook; 
  LPCTSTR       lpTemplateName; 
  void *        pvReserved;
  DWORD         dwReserved;
  DWORD         FlagsEx;
} OPENFILENAMEEX, *LPOPENFILENAMEEX; 

class CRPApp : public CWinApp
{
public: 
	CRPApp();   

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;
public:
	BOOL WriteProfileFloat( LPCTSTR lpszSection, LPCTSTR lpszEntry, float fValue );
	float GetProfileFloat( LPCTSTR lpszSection, LPCTSTR lpszEntry, float fDefault );
	void SaveParams();
	void InitParams();
	CWnd* ShowChildWindow(int ViewType);
	//{{AFX_MSG(CRPApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileclose();
	afx_msg void OnEmulate();
	afx_msg void OnProduce();
	afx_msg void OnCenterAjust();
	afx_msg void OnParams();
	afx_msg void OnUpdateItems(CCmdUI* pCmdUI);
	afx_msg void OnCardTest();
	afx_msg void OnEmulateQuick();
	afx_msg void OnProperty();
	afx_msg void OnFileOpenWithCad();
	afx_msg void OnEmulatePause();
	afx_msg void OnEmulateStop();
	afx_msg void OnUpdateEmulatePause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEmulateStop(CCmdUI* pCmdUI);
	afx_msg void OnProducePause();
	afx_msg void OnUpdateProducePause(CCmdUI* pCmdUI);
	afx_msg void OnProduceStop();
	afx_msg void OnUpdateProduceStop(CCmdUI* pCmdUI);
	afx_msg void OnExportPul();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RP_H__EE2810CD_B3C9_41F6_896C_B878A06D3268__INCLUDED_)

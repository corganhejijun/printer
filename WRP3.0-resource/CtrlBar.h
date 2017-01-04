#if !defined(AFX_CTRLBAR_H__8ACBB7CF_6B38_4035_9B9D_3A0CB513D5B2__INCLUDED_)
#define AFX_CTRLBAR_H__8ACBB7CF_6B38_4035_9B9D_3A0CB513D5B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtrlBar.h : header file
//
#include "Viewinc\cmycontrolbar.h"
/////////////////////////////////////////////////////////////////////////////
// CCtrlBar dialog

class CCtrlBar : public CMyControlBar
{
// Construction
private:
	CImageList m_ImageList;
public:
	void SetGS(BOOL open);
	void SetPos(long z);
	void SetPos(long x,long y); 
	virtual BOOL Create(CWnd* pParentWnd,LPCTSTR lpszTemplateName, CSize sizeDefault, UINT nID, DWORD dwStyle =  WS_CHILD| WS_VISIBLE|CBRS_LEFT|CBRS_SIZE_DYNAMIC);
	CCtrlBar();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCtrlBar)
	enum { IDD = CG_IDD_CONTROLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCtrlBar)
	afx_msg void OnChangeLayerindex();
	afx_msg void OnClickTreeView(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeView(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeView(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	LPCTSTR m_lpszTemplateName;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRLBAR_H__8ACBB7CF_6B38_4035_9B9D_3A0CB513D5B2__INCLUDED_)

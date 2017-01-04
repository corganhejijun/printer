#if !defined(AFX_PROPERTYLAYERPAGE_H__9B7BD25A_82A0_4D8D_840C_A43D3713EEC1__INCLUDED_)
#define AFX_PROPERTYLAYERPAGE_H__9B7BD25A_82A0_4D8D_840C_A43D3713EEC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyLayerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyLayerPage dialog

class CPropertyLayerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertyLayerPage)

// Construction
public:
	CPropertyLayerPage();
	~CPropertyLayerPage();

// Dialog Data
	//{{AFX_DATA(CPropertyLayerPage)
	enum { IDD = IDD_PROPERTY_LAYER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertyLayerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertyLayerPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLayerindex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYLAYERPAGE_H__9B7BD25A_82A0_4D8D_840C_A43D3713EEC1__INCLUDED_)

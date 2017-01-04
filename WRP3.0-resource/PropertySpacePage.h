#if !defined(AFX_PROPERTYSPACEPAGE_H__7D6AE7EC_AAEF_4257_8B52_2ACAA10B96AB__INCLUDED_)
#define AFX_PROPERTYSPACEPAGE_H__7D6AE7EC_AAEF_4257_8B52_2ACAA10B96AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertySpacePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertySpacePage dialog

class CPropertySpacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertySpacePage)

// Construction
public:
	CPropertySpacePage();
	~CPropertySpacePage();

// Dialog Data
	//{{AFX_DATA(CPropertySpacePage)
	enum { IDD = IDD_PROPERTY_SPACE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertySpacePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertySpacePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYSPACEPAGE_H__7D6AE7EC_AAEF_4257_8B52_2ACAA10B96AB__INCLUDED_)

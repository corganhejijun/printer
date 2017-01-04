#if !defined(AFX_PARAMVIEWCOLORPAGE_H__0E79CB70_91DB_4DD9_8F00_F5727391B891__INCLUDED_)
#define AFX_PARAMVIEWCOLORPAGE_H__0E79CB70_91DB_4DD9_8F00_F5727391B891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamViewColorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamViewColorPage dialog



class CParamViewColorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CParamViewColorPage)

// Construction
public:
	CGLView* m_pView;
	CParamViewColorPage(); 
	~CParamViewColorPage();

// Dialog Data
	//{{AFX_DATA(CParamViewColorPage)
	enum { IDD = IDD_PARAM_VIEW_COLOR };
	CColorBtn	m_ColorBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CParamViewColorPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CParamViewColorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLineKind();
	afx_msg void OnColor();
	afx_msg void OnInvisible();
	afx_msg void OnWidth();
	afx_msg void OnRandom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMVIEWCOLORPAGE_H__0E79CB70_91DB_4DD9_8F00_F5727391B891__INCLUDED_)

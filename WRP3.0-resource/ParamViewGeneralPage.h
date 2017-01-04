#if !defined(AFX_PARAMVIEWGENERALPAGE_H__21790F41_A711_4A3B_A8AD_7B35571EB6BD__INCLUDED_)
#define AFX_PARAMVIEWGENERALPAGE_H__21790F41_A711_4A3B_A8AD_7B35571EB6BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamViewGeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamViewGeneralPage dialog

class CParamViewGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CParamViewGeneralPage)

// Construction
public:
	int m_ParamModified;
	CGLView* m_pView; 
	CParamViewGeneralPage();  
	~CParamViewGeneralPage(); 
 
// Dialog Data
	//{{AFX_DATA(CParamViewGeneralPage)
	enum { IDD = IDD_PARAM_VIEW_GENERAL };
	CColorBtn	m_BkColorBtn;
	float	m_RotateX;
	float	m_RotateY;
	float	m_RotateZ;
	float	m_Scale;
	float	m_TransX;
	float	m_TransY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CParamViewGeneralPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CParamViewGeneralPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnResetCord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_sqLen;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMVIEWGENERALPAGE_H__21790F41_A711_4A3B_A8AD_7B35571EB6BD__INCLUDED_)

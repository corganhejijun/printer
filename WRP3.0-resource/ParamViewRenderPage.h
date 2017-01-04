#if !defined(AFX_PARAMVIEWRENDERPAGE_H__E98DFC37_0D5B_4ECC_8437_DC642A5D1BE1__INCLUDED_)
#define AFX_PARAMVIEWRENDERPAGE_H__E98DFC37_0D5B_4ECC_8437_DC642A5D1BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamViewRenderPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamViewRenderPage dialog

class CParamViewRenderPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CParamViewRenderPage)

// Construction
public:
	CGLView* m_pView;
	CParamViewRenderPage();
	~CParamViewRenderPage();
	float    v[7][4];
	float	 pos[4];
// Dialog Data
	//{{AFX_DATA(CParamViewRenderPage)
	enum { IDD = IDD_PARAM_VIEW_RENDER };
	CColorBtn	m_MatColor;
	CColorBtn	m_LightColor;
	int		m_Shininess;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CParamViewRenderPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CParamViewRenderPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLightColors();
	afx_msg void OnSelchangeMatColors();
	afx_msg void OnLightColor();
	afx_msg void OnMatColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void OnApplyParam();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMVIEWRENDERPAGE_H__E98DFC37_0D5B_4ECC_8437_DC642A5D1BE1__INCLUDED_)

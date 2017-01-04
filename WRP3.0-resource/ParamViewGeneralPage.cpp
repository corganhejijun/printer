// ParamViewGeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "rp.h"
#include "childview.h"

#include "colorbtn.h"
#include "ParamViewGeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamViewGeneralPage property page

IMPLEMENT_DYNCREATE(CParamViewGeneralPage, CPropertyPage)

CParamViewGeneralPage::CParamViewGeneralPage() : CPropertyPage(CParamViewGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CParamViewGeneralPage)
	m_RotateX = 0.0f;
	m_RotateY = 0.0f;
	m_RotateZ = 0.0f;
	m_Scale = 0.0f;
	m_TransX = 0.0f;
	m_TransY = 0.0f;
	//}}AFX_DATA_INIT
}

CParamViewGeneralPage::~CParamViewGeneralPage()
{ 
}

void CParamViewGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamViewGeneralPage)
	DDX_Control(pDX, IDC_BKCOLOR, m_BkColorBtn);
	DDX_Text(pDX, IDC_ROTATE_X, m_RotateX);
	DDX_Text(pDX, IDC_ROTATE_Y, m_RotateY);
	DDX_Text(pDX, IDC_ROTATE_Z, m_RotateZ);
	DDX_Text(pDX, IDC_SCALE_XYZ, m_Scale);
	DDV_MinMaxFloat(pDX, m_Scale, 1.e-007f, 1.e+010f);
	DDX_Text(pDX, IDC_TRANSLATE_X, m_TransX);
	DDX_Text(pDX, IDC_TRANSLATE_Y, m_TransY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamViewGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CParamViewGeneralPage)
	ON_BN_CLICKED(IDC_RESET_CORD, OnResetCord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamViewGeneralPage message handlers

BOOL CParamViewGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_ParamModified=1;
	m_BkColorBtn.currentcolor=m_pView->m_bkColor;
	m_RotateX=m_pView->m_RotateX;
	m_RotateY=m_pView->m_RotateY;
	m_RotateZ=m_pView->m_RotateZ;
	m_Scale=m_pView->m_Scale;

	CRect rect;	m_pView->GetClientRect(rect);
	if(rect.right>rect.bottom)m_sqLen=rect.bottom;
	else m_sqLen=rect.right;

	m_TransX=m_pView->m_TransX*m_sqLen;
	m_TransY=m_pView->m_TransY*m_sqLen;
	if(m_pView->m_ViewType&VIEW_PROCESS){
		GetDlgItem(IDC_USE_LIST)->EnableWindow(FALSE);
	}
	else if(m_pView->m_ViewType&VIEW_USE_LIST)
		((CButton*)GetDlgItem(IDC_USE_LIST))->SetCheck(1);
	else ((CButton*)GetDlgItem(IDC_USE_LIST))->SetCheck(0);
	UpdateData(FALSE);
	return TRUE;  
}

BOOL CParamViewGeneralPage::OnApply() 
{
	if(!UpdateData())return FALSE;
	m_pView->m_bkColor=m_BkColorBtn.currentcolor;
	m_pView->m_RotateX=	m_RotateX;
	m_pView->m_RotateY	=m_RotateY;
	m_pView->m_RotateZ=	m_RotateZ;
	m_pView->m_TransX=	m_TransX/m_sqLen;
	m_pView->m_TransY=	m_TransY/m_sqLen;
	m_pView->m_Scale=	m_Scale;
	int uselist=((CButton*)GetDlgItem(IDC_USE_LIST))->GetCheck(),olduselist;
	olduselist=m_pView->m_ViewType&VIEW_USE_LIST;
	if(uselist>1)uselist=1;if(olduselist>1)olduselist=1;
	if(uselist!=olduselist){
		m_ParamModified=1;
		m_pView->m_ViewType^=VIEW_USE_LIST;
	}
	return CPropertyPage::OnApply();
}

void CParamViewGeneralPage::OnResetCord() 
{
	m_RotateX=m_RotateY=m_RotateZ=0;
	m_TransX=m_TransY=0;
	m_Scale=1;
	UpdateData(FALSE);
}

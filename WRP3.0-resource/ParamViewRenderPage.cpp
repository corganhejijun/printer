// ParamViewRenderPage.cpp : implementation file
//

#include "stdafx.h"
#include "rp.h"
#include "gl/gl.h"
#include "childview.h"

#include "colorbtn.h"
#include "ParamViewRenderPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamViewRenderPage property page

IMPLEMENT_DYNCREATE(CParamViewRenderPage, CPropertyPage)

CParamViewRenderPage::CParamViewRenderPage() : CPropertyPage(CParamViewRenderPage::IDD)
{
	//{{AFX_DATA_INIT(CParamViewRenderPage)
	m_Shininess = 0;
	//}}AFX_DATA_INIT
}

CParamViewRenderPage::~CParamViewRenderPage()
{
}

void CParamViewRenderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamViewRenderPage)
	DDX_Control(pDX, IDC_MAT_COLOR, m_MatColor);
	DDX_Control(pDX, IDC_LIGHT_COLOR, m_LightColor);
	DDX_Text(pDX, IDC_MAT_SHININESS, m_Shininess);
	DDV_MinMaxInt(pDX, m_Shininess, 0, 128);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LIGHT_POS_X, pos[0]);
	DDX_Text(pDX, IDC_LIGHT_POS_Y, pos[1]);
	DDX_Text(pDX, IDC_LIGHT_POS_Z, pos[2]);
}


BEGIN_MESSAGE_MAP(CParamViewRenderPage, CPropertyPage)
	//{{AFX_MSG_MAP(CParamViewRenderPage)
	ON_CBN_SELCHANGE(IDC_LIGHT_COLORS, OnSelchangeLightColors)
	ON_CBN_SELCHANGE(IDC_MAT_COLORS, OnSelchangeMatColors)
	ON_BN_CLICKED(IDC_LIGHT_COLOR, OnLightColor)
	ON_BN_CLICKED(IDC_MAT_COLOR, OnMatColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamViewRenderPage message handlers

BOOL CParamViewRenderPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_pView->m_mtglMutex.Lock();
	wglMakeCurrent(m_pView->m_pDC->GetSafeHdc(), m_pView->hrcmtgl);
	glGetLightfv(GL_LIGHT0,GL_AMBIENT,v[0]);
	glGetLightfv(GL_LIGHT0,GL_DIFFUSE,v[1]);
	glGetLightfv(GL_LIGHT0,GL_SPECULAR,v[2]);
	glGetMaterialfv(GL_FRONT,GL_AMBIENT,v[3]);
	glGetMaterialfv(GL_FRONT,GL_DIFFUSE,v[4]);
	glGetMaterialfv(GL_FRONT,GL_SPECULAR,v[5]);
	glGetMaterialfv(GL_FRONT,GL_EMISSION,v[6]);
	glGetMaterialiv(GL_FRONT,GL_SHININESS,&m_Shininess);
	glLoadIdentity();
	glGetLightfv(GL_LIGHT0,GL_POSITION,pos);
	wglMakeCurrent(NULL,NULL);
	m_pView->m_mtglMutex.Unlock();
	UpdateData(FALSE);
	CComboBox*pcbox=(CComboBox*)GetDlgItem(IDC_LIGHT_COLORS);
	for(int i=0;i<3;i++)pcbox->SetItemData(i,i);
	pcbox->SetCurSel(0);
	OnSelchangeLightColors();
	pcbox=(CComboBox*)GetDlgItem(IDC_MAT_COLORS);pcbox->SetCurSel(0);
	for(int i=0;i<4;i++)pcbox->SetItemData(i,i+3);
	pcbox->SetCurSel(0);	
	OnSelchangeMatColors();

	return TRUE; 
}

BOOL CParamViewRenderPage::OnApply() 
{
	UpdateData();
	if(m_pView->m_ViewType&VIEW_PROCESS){
		m_pView->m_pProcessData->phrcMutex->Lock();
		wglMakeCurrent(m_pView->m_pDC->GetSafeHdc(), m_pView->hrc);
		OnApplyParam();
		wglMakeCurrent(NULL,NULL);
		m_pView->m_pProcessData->phrcMutex->Unlock();
	}
	m_pView->m_mtglMutex.Lock();
	wglMakeCurrent(m_pView->m_pDC->GetSafeHdc(), m_pView->hrcmtgl);
	OnApplyParam();
	wglMakeCurrent(NULL,NULL);
	m_pView->m_mtglMutex.Unlock();

	return CPropertyPage::OnApply();
}

void CParamViewRenderPage::OnSelchangeLightColors() 
{

	CComboBox*pcbox=(CComboBox*)GetDlgItem(IDC_LIGHT_COLORS);
	int i=pcbox->GetCurSel();i=pcbox->GetItemData(i);
	COLORREF c=0;for(int k=0;k<3;k++){c<<=8;c+=(int)(v[i][2-k]*255);}
	m_LightColor.SetCurrentColor(c);
}

void CParamViewRenderPage::OnSelchangeMatColors() 
{
	CComboBox*pcbox=(CComboBox*)GetDlgItem(IDC_MAT_COLORS);
	int i=pcbox->GetCurSel();i=pcbox->GetItemData(i);
	COLORREF c=0;for(int k=0;k<3;k++){c<<=8;c+=(int)(v[i][2-k]*255);}
	m_MatColor.SetCurrentColor(c);
	
}

void CParamViewRenderPage::OnLightColor() 
{
	CComboBox*pcbox=(CComboBox*)GetDlgItem(IDC_LIGHT_COLORS);
	int i=pcbox->GetCurSel();i=pcbox->GetItemData(i);
	COLORREF c=m_LightColor.currentcolor;
	v[i][0]=(c&0xff)/255.0f;v[i][1]=((c&0xff00)>>8)/255.0f;v[i][2]=(c>>16)/255.0f;
}

void CParamViewRenderPage::OnMatColor() 
{
	CComboBox*pcbox=(CComboBox*)GetDlgItem(IDC_MAT_COLORS);
	int i=pcbox->GetCurSel();i=pcbox->GetItemData(i);
	COLORREF c=m_MatColor.currentcolor;
	v[i][0]=(c&0xff)/255.0f;v[i][1]=((c&0xff00)>>8)/255.0f;v[i][2]=(c>>16)/255.0f;
	
}

void CParamViewRenderPage::OnApplyParam()
{

	glLightfv(GL_LIGHT0,GL_AMBIENT,v[0]);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,v[1]);
	glLightfv(GL_LIGHT0,GL_SPECULAR,v[2]);
	glMaterialfv(GL_FRONT,GL_AMBIENT,v[3]);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,v[4]);
	glMaterialfv(GL_FRONT,GL_SPECULAR,v[5]);
	glMaterialfv(GL_FRONT,GL_EMISSION,v[6]);
	glMaterialiv(GL_FRONT,GL_SHININESS,&m_Shininess);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,pos);

}

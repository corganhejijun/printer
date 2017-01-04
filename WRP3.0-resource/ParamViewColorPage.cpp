// ParamViewColorPage.cpp : implementation file
//

#include "stdafx.h"
#include "rp.h"
#include "childview.h"

#include "ColorBtn.h"
#include "ParamViewColorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamViewColorPage property page

IMPLEMENT_DYNCREATE(CParamViewColorPage, CPropertyPage)

CParamViewColorPage::CParamViewColorPage() : CPropertyPage(CParamViewColorPage::IDD)
{
	//{{AFX_DATA_INIT(CParamViewColorPage)
	//}}AFX_DATA_INIT
}

CParamViewColorPage::~CParamViewColorPage()
{
}

void CParamViewColorPage::DoDataExchange(CDataExchange* pDX)
{

	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CParamViewColorPage)
	DDX_Control(pDX, IDC_COLOR, m_ColorBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamViewColorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CParamViewColorPage)
	ON_CBN_SELCHANGE(IDC_LINE_KIND, OnSelchangeLineKind)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_INVISIBLE, OnInvisible)
	ON_BN_CLICKED(IDC_WIDTH, OnWidth)
	ON_BN_CLICKED(IDC_RANDOM, OnRandom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamViewColorPage message handlers

BOOL CParamViewColorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	int index=m_pView->m_ViewType&0x0f;
	static char str_contour[]="原始轮廓线条";
	static char str_offset[]="偏移轮廓线条";
	static char str_scanon[]="开光闸扫描线";
	static char str_scanoff[]="关光闸扫描线";
	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);
	if(index==(VIEW_2D_SHAPE&0x0f)){
		pLines->AddString(str_contour);
		pLines->AddString(str_offset);
	}else if(index==(VIEW_3D_FILE&0x0f)){
		pLines->AddString(str_contour);
	}else{
		pLines->AddString(str_scanon);
		pLines->AddString(str_scanoff);
		pLines->AddString(str_offset);
	}
	for(int i=0;i<m_pView->m_ColorCount;i++)
	pLines->SetItemData(i,m_pView->m_pColor[i].colorref);
	pLines->SetCurSel(0);
	OnSelchangeLineKind();
	return TRUE;
}

BOOL CParamViewColorPage::OnApply() 
{
	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);	
	for(int i=0;i<m_pView->m_ColorCount;i++)
	{
		MYCOLOR mc;mc.colorref=pLines->GetItemData(i);
		m_pView->m_pColor[i].colorref=mc.colorref;
	}
	return CPropertyPage::OnApply();
}

void CParamViewColorPage::OnSelchangeLineKind() 
{

	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);	
	int sel=pLines->GetCurSel();if(sel<0)return;
	CColorBtn*pcbtn=(CColorBtn*)GetDlgItem(IDC_COLOR);
	MYCOLOR mc;mc.colorref=pLines->GetItemData(sel);
	pcbtn->SetCurrentColor(mc.colorref&0xffffff);
	CButton*pchk=(CButton*)GetDlgItem(IDC_INVISIBLE);
	pchk->EnableWindow(mc.can_invisible);pchk->SetCheck(!mc.is_visible);
	pchk=(CButton*)GetDlgItem(IDC_WIDTH);
	pchk->EnableWindow(mc.can_havewidth);pchk->SetCheck(mc.is_havewidth);
	pchk=(CButton*)GetDlgItem(IDC_RANDOM);
	pchk->EnableWindow(mc.can_random);pchk->SetCheck(mc.is_random);
}



void CParamViewColorPage::OnColor() 
{
	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);	
	int sel=pLines->GetCurSel();if(sel<0)return;
	CColorBtn*pcbtn=(CColorBtn*)GetDlgItem(IDC_COLOR);
	MYCOLOR mc;mc.colorref=pLines->GetItemData(sel);
	mc.colorref&=0xff000000;mc.colorref|=pcbtn->currentcolor;
	pLines->SetItemData(sel,mc.colorref);
}

void CParamViewColorPage::OnInvisible() 
{
	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);	
	int sel=pLines->GetCurSel();if(sel<0)return;
	CColorBtn*pcbtn=(CColorBtn*)GetDlgItem(IDC_COLOR);
	MYCOLOR mc;mc.colorref=pLines->GetItemData(sel);
	mc.is_visible=!((CButton*)GetDlgItem(IDC_INVISIBLE))->GetCheck();
	pLines->SetItemData(sel,mc.colorref);	
}

void CParamViewColorPage::OnWidth() 
{
	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);	
	int sel=pLines->GetCurSel();if(sel<0)return;
	CColorBtn*pcbtn=(CColorBtn*)GetDlgItem(IDC_COLOR);
	MYCOLOR mc;mc.colorref=pLines->GetItemData(sel);
	mc.is_havewidth=((CButton*)GetDlgItem(IDC_WIDTH))->GetCheck();
	pLines->SetItemData(sel,mc.colorref);		
}

void CParamViewColorPage::OnRandom() 
{
	CComboBox*pLines=(CComboBox*)GetDlgItem(IDC_LINE_KIND);	
	int sel=pLines->GetCurSel();if(sel<0)return;
	CColorBtn*pcbtn=(CColorBtn*)GetDlgItem(IDC_COLOR);
	MYCOLOR mc;mc.colorref=pLines->GetItemData(sel);
	mc.is_random =((CButton*)GetDlgItem(IDC_RANDOM))->GetCheck();
	pLines->SetItemData(sel,mc.colorref);		
}

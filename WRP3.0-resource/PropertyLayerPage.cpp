// PropertyLayerPage.cpp : implementation file
//

#include "stdafx.h"
#include "rp.h"
#include "PropertyLayerPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyLayerPage property page

IMPLEMENT_DYNCREATE(CPropertyLayerPage, CPropertyPage)

CPropertyLayerPage::CPropertyLayerPage() : CPropertyPage(CPropertyLayerPage::IDD)
{
	//{{AFX_DATA_INIT(CPropertyLayerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropertyLayerPage::~CPropertyLayerPage()
{
}

void CPropertyLayerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyLayerPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyLayerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyLayerPage)
	ON_EN_CHANGE(IDC_LAYERINDEX, OnChangeLayerindex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyLayerPage message handlers



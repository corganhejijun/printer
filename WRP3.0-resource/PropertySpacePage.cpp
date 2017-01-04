// PropertySpacePage.cpp : implementation file
//

#include "stdafx.h"
#include "rp.h"
#include "PropertySpacePage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertySpacePage property page

IMPLEMENT_DYNCREATE(CPropertySpacePage, CPropertyPage)

CPropertySpacePage::CPropertySpacePage() : CPropertyPage(CPropertySpacePage::IDD)
{
	//{{AFX_DATA_INIT(CPropertySpacePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



CPropertySpacePage::~CPropertySpacePage()
{
}

void CPropertySpacePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertySpacePage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertySpacePage, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertySpacePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertySpacePage message handlers



// ChildView.h : interface of the CGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__26F11565_C23F_4C73_98FA_BFF0D5E2C674__INCLUDED_)
#define AFX_CHILDVIEW_H__26F11565_C23F_4C73_98FA_BFF0D5E2C674__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////

/////////////////////////////////////////////////////////////////////////////
// CGLView window
#define PAINT_NONE		0
#define PAINT_TOPAINT	1
#define PAINT_INBUILD	2
#define PAINT_EXIT		3
#define PAINT_ASSERT_PAINT	4		
#define PAINT_RESIZE		5

typedef union _MY_COLOR{
	COLORREF colorref;
	struct {
		unsigned r:8;
		unsigned g:8;
		unsigned b:8;
		unsigned reserved:2;
		unsigned is_random:1;
		unsigned can_random:1;
		unsigned is_havewidth:1;
		unsigned can_havewidth:1;
		unsigned is_visible:1;
		unsigned can_invisible:1;
	};
}MYCOLOR; 


typedef struct _PROCESS_DATA{
	CMutex*phrcMutex;
	HANDLE hPaintAssert;
	int ilayer;
	int icnc;
}PROCESS_DATA;
class CGLView : public CWnd
{
// Construction
public:
	CGLView();
// Attributes
public: 
	CDC*m_pDC;
	CWinThread*m_pPaintThread;
	int	m_rePaint;
	CMutex  m_rePaintMutex;
	CMutex  m_mtglMutex;
	HANDLE  m_PaintThreadResume;
	PROCESS_DATA*	m_pProcessData;
	HGLRC		hrc,hrcmtgl;
	int m_ViewType;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void glClearBeforePaint();
	void glCordinationTransform();
	void glResetCord();
	void glRebuild(BOOL reset=TRUE);
	virtual ~CGLView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnResetCord();
	afx_msg void OnViewParam();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void glMatAndLight();
	HGLRC glCreateRC(HDC hdc);
	void glSize(int cx,int cy);
	double m_dAspectRatio;

	///////gl variables
	BOOL m_LButtonDown;
	int  m_LBMode;
	POINT m_LButtonDownPoint;
public:
	void glDoClear();
	float m_RotateX,m_RotateY,m_RotateZ;
	float m_TransX,m_TransY,m_Scale;
	COLORREF m_bkColor;
	int m_ColorCount;
	MYCOLOR*m_pColor;
	//////
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__26F11565_C23F_4C73_98FA_BFF0D5E2C674__INCLUDED_)

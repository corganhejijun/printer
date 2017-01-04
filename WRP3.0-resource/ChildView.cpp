// ChildView.cpp : implementation of the CGLView class
//

#include "stdafx.h"
#include "RP.h"
#include "ChildView.h"
#include "childFrm.h"

#include "colorbtn.h"
#include "paramviewgeneralpage.h"
#include "paramviewrenderpage.h"
#include "paramviewcolorpage.h"

#include "math.h"
#include "gl\gl.h"
#include "gl\glu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////
#define PI 3.1415926535897932384626433832795
#define TWOPI 6.283185307179586476925286766559
#define LIST_MATANDLIGHT 0
void glDrawView(CGLView*pView);
void glDrawViewBusy(int ViewType);
void glMatAndLight();
void glCreateView(CGLView*pView);
void glCreateViewMultiThread(CGLView*pView);
void preMultiThread(CGLView*pView);
void glPaintViewProcessThreadProc(CGLView*pView);
///////////////////
extern int  m_CreateViewType;
/////////////////////////////////////////////////////////////////////////////
// CGLView

CGLView::CGLView()
{
	m_ViewType=m_CreateViewType;
	m_LBMode=0;
	m_RotateX=m_RotateY=m_RotateZ=0;
	m_TransX=m_TransY=0;
	m_Scale=1;
	m_pDC=NULL;
	m_bkColor=0xffffff;
	m_pPaintThread=NULL;
	m_rePaint=PAINT_NONE;
	if(m_ViewType&VIEW_NEED_MATANDLIGHT){
		m_ColorCount=0;
		m_pColor=NULL;
	}else{
		if(m_ViewType==VIEW_3D_FILE){
			m_ColorCount=1;m_pColor=new MYCOLOR;DWORD*pdw=(DWORD*)m_pColor;
			*pdw=0x48000000;
		}else if(m_ViewType==VIEW_2D_SHAPE){
			m_ColorCount=2;m_pColor=new MYCOLOR[2];DWORD*pdw=(DWORD*)m_pColor;
			*pdw=0x48000000;*(pdw+1)=0x7c000000;
		}else{
			m_ColorCount=3;m_pColor=new MYCOLOR[3];DWORD*pdw=(DWORD*)m_pColor;
			
			if(m_ViewType==VIEW_2D_PROCESS){
				*pdw=0x70ef0000;*(pdw+1)=0xc0ff0000;*(pdw+2)=0x70ff0000;
			}
			else{
				*pdw=0x78ef0000;*(pdw+1)=0xc8ff0000;*(pdw+2)=0x78ff0000;
			}
		}
	}
}

CGLView::~CGLView()
{
	if(m_pColor)delete[]m_pColor;
}


BEGIN_MESSAGE_MAP(CGLView,CWnd )
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDM_RESET_CORD, OnResetCord)
	ON_COMMAND(ID_VIEW_PARAM, OnViewParam)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,//|CS_OWNDC, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void glPaintViewThreadProc(CGLView*pView){
	
	while(TRUE){
		pView->m_rePaintMutex.Lock();
		if(pView->m_rePaint==PAINT_TOPAINT){
			pView->m_rePaint=FALSE;
			pView->m_rePaintMutex.Unlock();
			pView->m_mtglMutex.Lock();
			wglMakeCurrent(pView->m_pDC->GetSafeHdc(),pView->hrcmtgl);
			pView->glClearBeforePaint();
			///////坐标变换
			pView->glCordinationTransform();
			glPushMatrix();			
			glDrawView(pView);
			glPopMatrix();
			glFlush();
			glFinish();
			pView->m_rePaintMutex.Lock();
			if(pView->m_rePaint==PAINT_TOPAINT){
				pView->m_rePaintMutex.Unlock();
				glPushMatrix();			
				glDrawViewBusy(pView->m_ViewType);
				glPopMatrix();
			}else pView->m_rePaintMutex.Unlock();
			SwapBuffers(pView->m_pDC->GetSafeHdc());
			wglMakeCurrent(NULL, NULL);
			pView->m_mtglMutex.Unlock();
		}else if(pView->m_rePaint==PAINT_INBUILD){
			pView->m_rePaintMutex.Unlock();			
			pView->m_mtglMutex.Lock();
			wglMakeCurrent(pView->m_pDC->GetSafeHdc(),pView->hrcmtgl);
			pView->glClearBeforePaint();
			///////坐标变换
			pView->glCordinationTransform();
			glDrawViewBusy(pView->m_ViewType);
			glFlush();
			glFinish();
			SwapBuffers(pView->m_pDC->GetSafeHdc());
			wglMakeCurrent(NULL, NULL);
			pView->m_mtglMutex.Unlock();			
		}else if(pView->m_rePaint==PAINT_EXIT){
			pView->m_rePaintMutex.Unlock();
			AfxEndThread(0);
		}else{
			pView->m_rePaintMutex.Unlock();
			WaitForSingleObject(pView->m_PaintThreadResume,INFINITE);
		}
	}

}

void CGLView::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	
	m_rePaintMutex.Lock();
	if(m_rePaint==PAINT_NONE||m_rePaint==PAINT_RESIZE)m_rePaint=PAINT_TOPAINT;
	m_rePaintMutex.Unlock();
	SetEvent(m_PaintThreadResume);
	
	MSG msg;
	while(PeekMessage(&msg,m_hWnd,WM_PAINT,WM_PAINT,PM_REMOVE)){};
}


int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pDC=new CClientDC(this);
	hrc=glCreateRC(m_pDC->GetSafeHdc());
	/////////////////MTGL

	hrcmtgl=glCreateRC(m_pDC->GetSafeHdc());
	wglShareLists(hrc,hrcmtgl);
	wglMakeCurrent(m_pDC->GetSafeHdc(),hrcmtgl);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	if(m_ViewType&VIEW_NEED_MATANDLIGHT)glMatAndLight();
	if(m_ViewType&VIEW_2D) glDisable(GL_DEPTH_TEST);
	if(m_ViewType&VIEW_PROCESS)glDrawBuffer(GL_FRONT);
	else glDrawBuffer(GL_BACK);
	wglMakeCurrent(NULL,NULL);	

	/////////gl this thread
//	if(m_ViewType&(VIEW_PROCESS|VIEW_USE_TESS|VIEW_USE_LIST)){

		wglMakeCurrent(m_pDC->GetSafeHdc(),hrc);
		if(m_ViewType&VIEW_NEED_MATANDLIGHT)glMatAndLight();
		if(m_ViewType&VIEW_2D)glDisable(GL_DEPTH_TEST);
		if(m_ViewType&VIEW_PROCESS)glDrawBuffer(GL_FRONT);
		else glDrawBuffer(GL_BACK);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);
		wglMakeCurrent(NULL,NULL);
//	}
	m_PaintThreadResume= CreateEvent(NULL, FALSE, FALSE, NULL);
	glRebuild(FALSE);
	if(m_ViewType&VIEW_PROCESS){
		m_pProcessData=new PROCESS_DATA;
		m_pProcessData->phrcMutex=new CMutex;
		m_pProcessData->hPaintAssert=CreateEvent(NULL,FALSE,FALSE,NULL);
		m_pPaintThread=AfxBeginThread((AFX_THREADPROC)glPaintViewProcessThreadProc,this);
	}else{
		m_pProcessData=NULL;
		m_pPaintThread=AfxBeginThread((AFX_THREADPROC)glPaintViewThreadProc,this);
	}
	return 0;
}


void CGLView::OnDestroy() 
{
	if(m_pPaintThread){
		m_rePaintMutex.Lock();
		m_rePaint=PAINT_EXIT;
		m_rePaintMutex.Unlock();
		m_mtglMutex.Lock();
		TerminateThread(m_pPaintThread->m_hThread,0);	
		m_mtglMutex.Unlock();
	}
	if(hrc)VERIFY(wglDeleteContext(hrc));
	if(hrcmtgl)VERIFY(wglDeleteContext(hrcmtgl));
	if(m_pDC)delete m_pDC;
	CWnd ::OnDestroy();
	
}
void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	if(cy>0){
// update the rect and the aspect ratio
		m_rePaintMutex.Lock();
		if(m_rePaint==PAINT_NONE)m_rePaint=PAINT_RESIZE;
		m_rePaintMutex.Unlock();

		m_dAspectRatio=double(cx)/double(cy);

		if(m_ViewType&VIEW_PROCESS){
			m_pProcessData->phrcMutex->Lock();
			wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
			glSize(cx,cy);
			wglMakeCurrent(NULL,NULL);
			m_pProcessData->phrcMutex->Unlock();
		}
		{
			m_mtglMutex.Lock();
			wglMakeCurrent(m_pDC->GetSafeHdc(), hrcmtgl);
//			DWORD dw=GetLastError();
//			ASSERT(dw==0);
			glSize(cx,cy);
			wglMakeCurrent(NULL,NULL);
			m_mtglMutex.Unlock();
			
		}
		Invalidate(FALSE);
	}
	
}

void CGLView::glSize(int cx,int  cy)
{
// set correspondence between window and OGL viewport
// update the camera
	if(m_ViewType&VIEW_3D){
		float sx=1.0f,sy=1.0f;
		if(cx<cy){sy=((float)cy)/cx;}
		else{sx=((float)cx)/cy;}
		glViewport(0,0,cx,cy);
 			glPushMatrix();
				glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
///////////////////////////透视
//					gluPerspective(40.0,m_dAspectRatio,0.1f, 10.0f);
//					glTranslatef(0.0f,0.0f,-4.f);
////////////////////////// 正交
					glOrtho(-sx,sx,-sy,sy,0.1f,10.f);
					glTranslatef(0.0f,0.0f,-5.0f);
///////////////////////////////
				glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
		else{
			glViewport(0,0,cx,cy);
			float sx=1.0f,sy=1.0f;
			if(cx<cy){sy=((float)cy)/cx;}
			else{sx=((float)cx)/cy;}
 			glPushMatrix();
				glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluOrtho2D(-sx,sx,-sy,sy);
//					gluOrtho2D(0,2*sx,0,2*sy);
				glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

}




BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


void CGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LButtonDown=TRUE;
	m_LButtonDownPoint=point;
	SetFocus();
	SetCapture();
	if(nFlags&MK_CONTROL){m_LBMode=1;return;}
	else if(nFlags&MK_SHIFT){m_LBMode=3;return;}
	else {m_LBMode=2;return;}
}

void CGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_LButtonDown=FALSE;
	m_LBMode=0;
	ReleaseCapture();
}

void CGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_LButtonDown||!m_LBMode)return;
	CRect rect;
	GetClientRect(rect);
//	if(!rect.PtInRect(point)) {OnLButtonUp(0,point);	return;	}
//	MSG msg;
//	while(PeekMessage(&msg,m_hWnd,WM_MOUSEMOVE,WM_MOUSEMOVE,PM_REMOVE)){};
	if(m_LBMode==1)
	{
		if(nFlags&MK_CONTROL)
		{
			if(m_ViewType&VIEW_3D){
				m_RotateY+=(float)(m_LButtonDownPoint.x - point.x)/3.0f;
				m_RotateX+= (float)(m_LButtonDownPoint.y - point.y)/3.0f;
				m_LButtonDownPoint = point;	
			}else{
				double da;
				da=atan2((double)(point.y-rect.CenterPoint().y), (double)(point.x-rect.CenterPoint().x))
					-atan2((double)(m_LButtonDownPoint.y - rect.CenterPoint().y),
					(double)(m_LButtonDownPoint.x-rect.CenterPoint().x));
				while(da>=PI)da-=TWOPI;
				while(da<-PI)da+=TWOPI;
				m_RotateZ-=float(da*180/PI);
				m_LButtonDownPoint = point;	
			}
		}
		else	m_LButtonDown=FALSE;	
	}else if(m_LBMode==2)
	{
			float sx=2.0f,sy=2.0f;
			if(rect.bottom>rect.right)sy=2.0f*rect.bottom/rect.right;
			else sx=2.0f*rect.right/rect.bottom;
			m_TransX-=(float)( m_LButtonDownPoint.x-point.x )*sx/rect.right;
			m_TransY+=(float)( m_LButtonDownPoint.y-point.y )*sy/rect.bottom;
			m_LButtonDownPoint = point;
	}else if(m_LBMode==3){
		if(nFlags&MK_SHIFT)
		{
			float dy=float(m_LButtonDownPoint.y - point.y);
			m_Scale*=float(pow((float)1.1,(float)(dy/10)));
			m_LButtonDownPoint = point;
		}
		else	m_LButtonDown=FALSE;
	}
	Invalidate(FALSE);
}

BOOL CGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(zDelta>0)m_Scale*=1.1f;
	else m_Scale/=1.1f;
	Invalidate(FALSE);
	return CWnd ::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==107||nChar==187)m_Scale*=1.1f;
	else if(nChar==109||nChar==189)m_Scale/=1.1f;
	else return;
	Invalidate(FALSE);
}




void CGLView::glRebuild(BOOL reset)
{
	if(reset)OnResetCord();
	if(m_ViewType&VIEW_PROCESS){}
	else {
		m_rePaintMutex.Lock();
		m_rePaint=PAINT_INBUILD;
		m_rePaintMutex.Unlock();
		preMultiThread(this);
		AfxBeginThread((AFX_THREADPROC)glCreateViewMultiThread,this);
		Invalidate(FALSE);
	}
	
}



void CGLView::glResetCord()
{
	OnResetCord();
}

HGLRC CGLView::glCreateRC(HDC hdc)
{
    int pixelformat;HGLRC hrc;
	static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),1,                             
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,		
        PFD_TYPE_RGBA,24,0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,16,0,0,                          
        PFD_MAIN_PLANE,0,0, 0, 0                     
    };
    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) ==0)return NULL;
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)return NULL;
	pixelformat =::GetPixelFormat(hdc);
	::DescribePixelFormat(hdc, pixelformat, sizeof(pfd), &pfd);
    hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	wglMakeCurrent(NULL, NULL);
	PostMessage(WM_PAINT);
    return hrc;

}

void CGLView::OnResetCord() 
{
	m_RotateX=m_RotateY=m_RotateZ=0;
	m_TransX=m_TransY=0;
	m_Scale=1;
	Invalidate(FALSE);
	
}

void CGLView::glCordinationTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(m_TransX,m_TransY,0.0f);
	glScalef(m_Scale,m_Scale,m_Scale);
//	glTranslatef(m_TransX/m_Scale,m_TransY/m_Scale,0.0f);
	glRotatef(m_RotateX,1.0f,0.0f,0.0f);
	glRotatef(m_RotateY,0.0f,1.0f,0.0f);
	glRotatef(m_RotateZ,0.0f,0.0f,1.0f);
}

void CGLView::OnViewParam() 
{
	CPropertySheet sheet("显示设置");
	CParamViewGeneralPage pvgpage;pvgpage.m_pView=this;
	sheet.AddPage(&pvgpage);
	if(m_ViewType&VIEW_NEED_MATANDLIGHT){
		CParamViewRenderPage pvrpage;pvrpage.m_pView=this;
		sheet.AddPage(&pvrpage);
		if(sheet.DoModal()==IDOK){
			if(pvgpage.m_ParamModified)glRebuild(FALSE);
			m_bkColor=pvgpage.m_BkColorBtn.currentcolor;
		}
		Invalidate(FALSE);
	}else{
//		AfxMessageBox("add");
		CParamViewColorPage pvcpage;pvcpage.m_pView=this;
//		AfxMessageBox("add");
		sheet.AddPage(&pvcpage);
		if(sheet.DoModal()==IDOK){
			if(pvgpage.m_ParamModified)glRebuild(FALSE);
			m_bkColor=pvgpage.m_BkColorBtn.currentcolor;
		}
		Invalidate(FALSE);
	}
	
}

void CGLView::glClearBeforePaint()
{
	glClearColor((m_bkColor&0xff)/255.0f,((m_bkColor&0xff00)>>8)/255.0f,
		(m_bkColor>>16)/255.0f,0.0f);
	glClearDepth(1.0f);
 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void CGLView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnViewParam();
}

void CGLView::glMatAndLight()
{
	/////////光照////////////
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
/*	GLfloat position[]={1.0f,1.0f,4.0f,.0f};
	GLfloat ambient[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat specular[]={.5f,.5f,.5f,1.0f};
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,specular);
	/////材质///////////////
	GLfloat materialAD[]={0.4f,0.2f,0.05f,1.0f};
	GLfloat diffuse[]={1.0f,0.7f,0.1f};
	GLfloat materialSP[]={0.8f,0.8f,0.8f,1.0f};

	glMaterialfv(GL_FRONT,GL_AMBIENT,materialAD);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSP);
	glMaterialf(GL_FRONT,GL_SHININESS,60.0);
	ASSERT(glGetError()==0);*/
}



void CGLView::glDoClear()
{
	m_mtglMutex.Lock();
		wglMakeCurrent(m_pDC->GetSafeHdc(),hrcmtgl);
		glClearBeforePaint();
		glFlush();
		glFinish();
		wglMakeCurrent(NULL, NULL);
	m_mtglMutex.Unlock();
}


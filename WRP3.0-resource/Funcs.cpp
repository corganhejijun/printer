#include "stdafx.h"
#include <afxtempl.h>
#include "rp.h"
#include "childfrm.h"
#include "mainfrm.h"
#include "types.h"
#include "math.h"

#include "propertyspacepage.h"
#include "propertylayerpage.h"

#include "funcs.h"

#define MACHINE_HAVE_POWER	1

#define DATA_IN_THISFILE
#include "types2.h"
/////glinclude
#include "gl\gl.h"
#include "gl\glu.h"
#include "glfuncs.h"
////////参数

_APP_PARAM AppParam={
	0x06,1,0,1,0,0,0,0,1,1,0,
	5.0f,10.0f,0,HALFPI,10.0f,
	100000,5.0f,1.0f,1.0f,10.0f,50
};

//////////线条实体信息
int m_LayerIndex=1;///当前选择的层的序号　从1开始
////////////////////实体空间
ESpace EntSpace={0,0,0,0,0,0,0,0,0,0,0,0,0};
EMULATE_STATUS	EmulateSts={new CMutex(),EMU_PRD_NONE,0,0};

//////////////////
////////互斥
static CMutex threadcountmutex;
static int threadcount=0;/////线程计算
//////

//////获得特定的窗口
CGLView*AppGetView(int ViewType){

return &(((((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetChildFrame(ViewType)))
		->m_wndView);

}
/////获得控制栏的某个控件
CWnd*AppGetCtrlDlgItem(UINT nID){
	return ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_ControlBar.GetDlgItem(nID);
}
/////获得控制栏
CCtrlBar*AppGetCtrlDlg(){
	return &(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_ControlBar);
}
////层序号变体化
BOOL AppLayerChange(int LayerIndex){
	if(LayerIndex<=0||LayerIndex>EntSpace.LayerNum)return FALSE;
	CString str;
	m_LayerIndex=LayerIndex;
	str.Format("%.1f",EntSpace.pLayer[m_LayerIndex-1].z);
	AppGetCtrlDlgItem(IDC_LAYERZ)->SetWindowText(str);
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->glRebuildView(VIEW_2D);
	return TRUE;
}

void VarReleaseArray(CPtrArray&ar){
	int i,size=ar.GetSize();
	for(i=0;i<size;i++)if(ar[i]) delete ar[i];
	ar.RemoveAll();
}
/////重置控制栏
void AppCtrldlgReset(){
	AppGetCtrlDlgItem(IDC_LAYERINDEX)->EnableWindow(EntSpace.LayerNum);
	CSpinButtonCtrl* pspin=(CSpinButtonCtrl*)AppGetCtrlDlgItem(IDC_LAYERSPIN);
	pspin->EnableWindow(EntSpace.LayerNum);
	pspin->SetRange(1,EntSpace.LayerNum);
	pspin->SetPos(1);
}
void ReleasePolygons(GL_BEGIN*pPolygons){
	if(pPolygons==NULL)return;
	delete pPolygons->pVertex;
	delete pPolygons;
}
/////删除层
void ReleaseLayer(ELayer*pLayer){
	ReleaseLoop(pLayer->pLoopOriginalBTree);
	ReleaseLoop(pLayer->pLoopOriginalTree,FALSE);
	ReleaseLoop(pLayer->pLoopExtremeTree);
	ReleaseLoop(pLayer->pLoopOffsetBTree);
	ReleaseLoop(pLayer->pLoopOffsetTree,FALSE);
	if(pLayer->iNcEntNum>0)delete[]pLayer->pNcEnt;
	if(pLayer->iPolygonsExtreme>0)ReleasePolygons(pLayer->pPolygonsExtreme);
	if(pLayer->iPolygonsOriginal>0)ReleasePolygons(pLayer->pPolygonsOriginal);
}
////关闭当前文件
void AppFileClose(){
	if(EntSpace.LayerNum>0){
		for(int i=0;i<EntSpace.LayerNum;i++)ReleaseLayer(EntSpace.pLayer+i);
		delete []EntSpace.pLayer;
		EntSpace.pLayer=NULL;
	}
	if(EntSpace.pEntRaw){delete[]EntSpace.pEntRaw;	EntSpace.pEntRaw=NULL;}
	EntSpace.EntNum=EntSpace.LayerNum=0;
	AppCtrldlgReset();
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->glRebuildView(VIEW_2D);
	EmulateSts.emode=EMU_PRD_NONE;
	EmulateSts.icnc=EmulateSts.ilayer=0;
}
//////生成层
void BuildLayer(ELayer*pLayer,float cncAngle=0,float z=0){
		///////////////////////原始图形
			pLayer->z=z*AppParam.fScalez;
			pLayer->pLoopOriginalBTree=BuildLayerLoopOriginal(pLayer);
			if(pLayer->pLoopOriginalBTree)LoopArrangeDirection(pLayer->pLoopOriginalBTree);
			pLayer->pLoopOriginalTree=LoopTreeCopyNoEnt(pLayer->pLoopOriginalBTree);
			BuildLoopTreeFromBTree(pLayer->pLoopOriginalTree);
		////////////////////////极限图形
			pLayer->pLoopExtremeTree=LoopTreeCopy(pLayer->pLoopOriginalBTree,AppParam.fScale);
			//////////偏移环上的实体
			pLayer->pLoopOffsetBTree=BuildLayerLoopOffset(pLayer->pLoopExtremeTree,AppParam.fOffsetDist,TRUE);	
			//////检查环上的自交情况
			if(AppParam.bCheckSelfXAfterOffset&&pLayer->pLoopOffsetBTree)
				LoopSfxCheck(pLayer->pLoopOffsetBTree,TRUE);
			else LoopTreeGetExt(pLayer->pLoopOffsetBTree);
			pLayer->pLoopOffsetTree=LoopTreeCopyNoEnt(pLayer->pLoopOffsetBTree);
			BuildLoopTreeFromBTree(pLayer->pLoopExtremeTree);
			BuildLoopTreeFromBTree(pLayer->pLoopOffsetTree);
			/////生成扫描路径
			BuildLayerNC(pLayer,cncAngle,AppParam.fCncLineDist,AppParam.fCncLenMin,
				AppParam.fOffsetDist,AppParam.bCncOrder,AppParam.bCncLineCross );
			pLayer->pPolygonsExtreme=pLayer->pPolygonsOriginal=NULL;
			pLayer->iPolygonsExtreme=pLayer->iPolygonsOriginal=0;
}
/////////////层成所有层
void BuildLayers(){
	float angle=AppParam.fCncFirstAngle;
	for(int i=1;i<EntSpace.LayerNum;i++){
		angle+=AppParam.fCncAngleDelta;
		if(angle>=PI) angle-=PI;
		else if(angle<0)angle+=PI;
//		ASSERT(i!=2);
		BuildLayer(EntSpace.pLayer+i,angle,EntSpace.pz[i]);
	}
}
//////打开文件
BOOL AppFileOpen(LPSTR szFile){
		threadcountmutex.Lock();
		if(threadcount>0){
			threadcountmutex.Unlock();
			return FALSE;
		}else threadcountmutex.Unlock();

		CWaitCursor wc;CString str=szFile;
		int npos;
		npos=str.ReverseFind('.');
		if(npos==-1)return FALSE;
		str.Delete(0,npos+1);
		if(str.CompareNoCase("dxf")==0){
			if(!ReadDxfFile(szFile,&EntSpace))return FALSE;
		}else  if(str.CompareNoCase("dwg")==0){
			if(!ReadDwgFile(szFile,&EntSpace))return FALSE;
		}else return FALSE;
		AppCtrldlgReset();
		if(EntSpace.LayerNum==0||EntSpace.EntNum==0){
			return FALSE;
		}else{ 
			BuildLayer(EntSpace.pLayer,AppParam.fCncFirstAngle);
			AppLayerChange(1);
			CMainFrame*pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			pFrame->glResetCordination(VIEW_2D);
//			pFrame->DestroyChildWindowsOnOpen();
			BuildLayers();
			pFrame->glRebuildView(VIEW_3D);
			pFrame->glResetCordination(VIEW_3D);
		}
		CSpinButtonCtrl* pspin=(CSpinButtonCtrl*)AppGetCtrlDlgItem(IDC_LAYERSPIN);
		pspin->SetRange32(1,EntSpace.LayerNum);
		return TRUE;
}
void buildLayerThreadProc(){



}
/////////////////OPENGL LLLL
/////////////////GLGLLGLGLLGLGLLGLLG
#define USE_TIME_COUNTER	1
#if USE_TIME_COUNTER

#define TIME_COUNT 50
class CTimeCounter{
private:
	DWORD msec;
public:
	CTimeCounter(){
		msec=GetTickCount();
	}
	~CTimeCounter(){}
	BOOL IsCountEnd(){
		if((GetTickCount()-msec)>TIME_COUNT)return TRUE;
		else return FALSE;
	}
};
#endif

#define glColorNoRandom(pmc)	glColor3f((pmc)->r/256.0f,(pmc)->g/256.0f,(pmc)->b/256.0f)

void glColor(MYCOLOR* pmc){
	if(pmc->is_random)
//		glColor3b(rand()%256,rand()%256,rand()%256);
		glColor3f(rand()%256/255.0f,rand()%256/255.0f,rand()%256/255.0f);
	else 
		glColor3f(pmc->r/256.0f,pmc->g/256.0f,pmc->b/256.0f);
//		glColor3b(pView->m_pColor->r,pView->m_pColor->g,pView->m_pColor->b);

}

/*
static void glCreate2dShape(CGLView*pView){
	if(m_LayerIndex>EntSpace.LayerNum||m_LayerIndex<1)return;
	ELayer*pLayer=EntSpace.pLayer+m_LayerIndex-1;
	ASSERT(pLayer);
	//生成画轮廓的执行表
	glDrawLoop(pLayer->pLoopOffsetTree,GL_DRAW_FACE,AppParam.fOffsetDist );
	glColor3f(0.0f,0.0f,0.0f);
	glLineWidth(1.0f);	
	glScalef(AppParam.fScale,AppParam.fScale,AppParam.fScale);
//		glDrawLoop(pLayer->pLoopOriginalTree,GL_DRAW_LINE);
	for(int i=0;i<pLayer->iEntNum;i++)
		glDrawEntLine(pLayer->pEntRaw+i);

}
static void glCreate2dForcast(CGLView*pView){
	int i;float Px=0,Py=0;EntFull e;
	if(m_LayerIndex>EntSpace.LayerNum||m_LayerIndex<1)return;
	ELayer*pLayer=EntSpace.pLayer+m_LayerIndex-1;
	ASSERT(pLayer);
	glColor3f(0.0f,.0f,1.0f);
	for(i=0;i<pLayer->iNcEntNum;i++){
		if(NcToEntFull(pLayer->pNcEnt+i,Px,Py,e)){
			if(pLayer->pNcEnt[i].ms==0)glDrawEntLine(&e);
			else{
//				if(pLayer->pNcEnt[i].on_contour)glColor3f(0.0f,.7f,0.0f);
				glDrawEntFace(&e,AppParam.fOffsetDist);
//				glDrawEntLine(&e);
//				if(pLayer->pNcEnt[i].on_contour) glColor3f(0.0f,.0f,0.7f);
			}
		}
	}
}*/
static void glDraw2dShape(CGLView*pView){
	if(m_LayerIndex>EntSpace.LayerNum||m_LayerIndex<1)return;
	ELayer*pLayer=EntSpace.pLayer+m_LayerIndex-1;
	ASSERT(pLayer);
	if(pView->m_pColor[1].is_visible){
		if(!pView->m_pColor[1].is_random)
			glColor3f(pView->m_pColor[1].r/256.0f,pView->m_pColor[1].g/256.0f,pView->m_pColor[1].b/256.0f);
		if(pView->m_pColor[1].is_havewidth)
			glDrawLoop(pLayer->pLoopOffsetTree,GL_DRAW_FACE,AppParam.fOffsetDist,
				pView->m_pColor[1].is_random);
		else
			glDrawLoop(pLayer->pLoopOffsetTree,GL_DRAW_LINE,0,
				pView->m_pColor[1].is_random);

	}
	
	glLineWidth(1.0f);	
	glScalef(AppParam.fScale,AppParam.fScale,AppParam.fScale);
//		glDrawLoop(pLayer->pLoopOriginalTree,GL_DRAW_LINE);
	glColor(pView->m_pColor);
	for(int i=0;i<pLayer->iEntNum;i++)
		glDrawEntLine(pLayer->pEntRaw+i);

}
static void glDraw2dForcast(CGLView*pView){
	int i;float Px=0,Py=0;EntFull e;
	if(m_LayerIndex>EntSpace.LayerNum||m_LayerIndex<1)return;
	ELayer*pLayer=EntSpace.pLayer+m_LayerIndex-1;
	ASSERT(pLayer);	
	for(i=0;i<3;i++){
		if(pView->m_pColor[i].is_random){
			pView->m_pColor[i].r=rand()%256;
			pView->m_pColor[i].g=rand()%256;
			pView->m_pColor[i].b=rand()%256;
		}
	}
	for(i=0;i<pLayer->iNcEntNum;i++){
		if(NcToEntFull(pLayer->pNcEnt+i,Px,Py,e)){
			if(pLayer->pNcEnt[i].ms){
				if(pLayer->pNcEnt[i].on_contour){
					glColorNoRandom(pView->m_pColor+2);
					if(pView->m_pColor[2].is_havewidth)
						glDrawEntFace(&e,AppParam.fOffsetDist);
					else glDrawEntLine(&e);	
				}else{
					glColorNoRandom(pView->m_pColor);					
					if(pView->m_pColor->is_havewidth)
						glDrawEntFace(&e,AppParam.fOffsetDist);
					else glDrawEntLine(&e);					
				}
			}else{
				if(pView->m_pColor[1].is_visible){
					glColorNoRandom(pView->m_pColor+1);
					glDrawEntLine(&e);
				}
			}
		}
	}
}
static void glDrawLayerForcast(ELayer*pLayer,int iz){
		int i,u;float Px=0,Py=0,z,zh;
		EntFull e;NcEnt*pEnt;BOOL prems=FALSE;
		pEnt=pLayer->pNcEnt;
		u=pLayer->iNcEntNum;
		z=pLayer->z;
		zh=(pLayer+1)->z;
		for(i=0;i<u;i++){
			if(NcToEntFull(pEnt+i,Px,Py,e)&&pEnt[i].ms)
				glDrawNcEntSolid(&e,AppParam.fOffsetDist,z,zh,prems);
			prems=pEnt[i].ms;
		}	
}
static void glDrawLayerForcast(ELayer*pLayer,int iz,int iu){
		int i;float Px=0,Py=0,z,zh;
		EntFull e;NcEnt*pEnt;BOOL prems=FALSE;
		pEnt=pLayer->pNcEnt;
		z=pLayer->z;
		zh=(pLayer+1)->z;
		for(i=0;i<iu;i++){
			if(NcToEntFull(pEnt+i,Px,Py,e)&&pEnt[i].ms)
				glDrawNcEntSolid(&e,AppParam.fOffsetDist,z,zh,prems);
			prems=pEnt[i].ms;
		}	
}
static void glCreate3dForcast(CGLView*pView){
	ELayer*pLayer;int iz;
	//生成画轮廓的执行表	
	glColor3f(0.0f,0.0f,0.0f);
	for(iz=0,pLayer=EntSpace.pLayer;iz<EntSpace.LayerNum-1;iz++,pLayer++){
		glDrawLayerForcast(pLayer,iz);
	}

}
void glCreate3dDesignTess(){
	GLUtesselator *tess=gluNewTess();	ASSERT(tess);
	POLYGONS_DATA pgsd;pgsd.vNum=0;int gsize,i;
	glpreTessellate(tess);
//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	ELayer*pLayer;
	for(i=0,pLayer=EntSpace.pLayer;i<EntSpace.LayerNum-1;i++,pLayer++){
		if(pLayer->pPolygonsOriginal)break;
		ELoop*ploop=pLayer->pLoopOriginalTree;
		while(ploop){
			glTessRegionFace(ploop,tess,&pgsd);
			ploop=ploop->next;
		}
		gsize=pgsd.pgar.GetSize();
		if(gsize>0){
			pLayer->pPolygonsOriginal=new GL_BEGIN[gsize];
			memcpy(pLayer->pPolygonsOriginal,pgsd.pgar.GetData(),gsize*sizeof(GL_BEGIN));
			pLayer->iPolygonsOriginal=gsize;
			pgsd.pgar.RemoveAll();
		}
	}
	gluDeleteTess(tess);	
}
static void glDrawLayerDesign(ELayer*pLayer,int i){
		int j,k;GL_BEGIN*pgb;float z,zh,*pf;
		ELoop*ploop=pLayer->pLoopOriginalTree;
		z=EntSpace.pz[i];zh=EntSpace.pz[i+1];
		if(ploop)glDrawLoopCylinder(ploop,z,zh);
		if(pLayer->iPolygonsOriginal>0){
			glNormal3f(0.0f,0.0f,-1.0f);
			for(j=0,pgb=pLayer->pPolygonsOriginal;j<pLayer->iPolygonsOriginal;j++,pgb++){
				glBegin(pgb->type);
				for(k=0,pf=pgb->pVertex;k<pgb->vNum;k++,pf+=2)
					glVertex3f(pf[0],pf[1],z);
				glEnd();
			}
			glNormal3f(0.0f,0.0f,1.0f);
			for(j=0,pgb=pLayer->pPolygonsOriginal;j<pLayer->iPolygonsOriginal;j++,pgb++){
				glBegin(pgb->type);
				for(k=0,pf=pgb->pVertex;k<pgb->vNum;k++,pf+=2)
					glVertex3f(pf[0],pf[1],zh);
				glEnd();
			}
		}
}
static void glCreate3dDesign(CGLView*pView){
	ELayer*pLayer;int i;
	for(i=0,pLayer=EntSpace.pLayer;i<EntSpace.LayerNum-1;i++,pLayer++){
		glDrawLayerDesign(pLayer,i);
	}
}
void glCreate3dExtremeTess(){
	GLUtesselator *tess=gluNewTess();	ASSERT(tess);
	POLYGONS_DATA pgsd;pgsd.vNum=0;int gsize;
	glpreTessellate(tess);
//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	ELayer*pLayer;
	for(int i=0;i<EntSpace.LayerNum-1;i++){
		pLayer=EntSpace.pLayer+i;
		if(pLayer->pPolygonsExtreme)break;
		ELoop*ploop=pLayer->pLoopExtremeTree;
		while(ploop){
			glTessRegionFace(ploop,tess,&pgsd);
			ploop=ploop->next;
		}
		gsize=pgsd.pgar.GetSize();
		pLayer->pPolygonsExtreme=new GL_BEGIN[gsize];
		memcpy(pLayer->pPolygonsExtreme,pgsd.pgar.GetData(),gsize*sizeof(GL_BEGIN));
		pLayer->iPolygonsExtreme=gsize;
		pgsd.pgar.RemoveAll();
	}
	gluDeleteTess(tess);	
}
static void glDrawLayerExtreme(ELayer*pLayer,int i){
		int j,k;GL_BEGIN*pgb;float z,zh,*pf;
		ELoop*ploop=pLayer->pLoopExtremeTree;
		z=pLayer->z;zh=(pLayer+1)->z;
		if(ploop)glDrawLoopCylinder(ploop,z,zh);
		if(pLayer->iPolygonsExtreme>0){
			glNormal3f(0.0f,0.0f,-1.0f);
			for(j=0,pgb=pLayer->pPolygonsExtreme;j<pLayer->iPolygonsExtreme;j++,pgb++){
				glBegin(pgb->type);
				for(k=0,pf=pgb->pVertex;k<pgb->vNum;k++,pf+=2)
					glVertex3f(pf[0],pf[1],z);
				glEnd();
			}
			glNormal3f(0.0f,0.0f,1.0f);
			for(j=0,pgb=pLayer->pPolygonsExtreme;j<pLayer->iPolygonsExtreme;j++,pgb++){
				glBegin(pgb->type);
				for(k=0,pf=pgb->pVertex;k<pgb->vNum;k++,pf+=2)
					glVertex3f(pf[0],pf[1],zh);
				glEnd();
			}

		}

}

static void glCreate3dExtreme(CGLView*pView){
	ELayer*pLayer;int i;
	for(i=0,pLayer=EntSpace.pLayer;i<EntSpace.LayerNum-1;i++,pLayer++){
		glDrawLayerExtreme(pLayer,i);
	}
}
static void glDrawLayerFile(ELayer*pLayer,int iz){
			int u,i;float z;
			EntRaw*pEnt=EntSpace.pLayer[iz].pEntRaw;
			u=pLayer->iEntNum;
			z=EntSpace.pz[iz];
			for(i=0;i<u;i++)glDrawEntLine(pEnt+i,z);

}
static void glCreate3dFile(CGLView*pView){
	int iz;ELayer*pLayer;
	//生成画轮廓的执行表
	glColor(pView->m_pColor);
	for(iz=0,pLayer=EntSpace.pLayer ;iz<EntSpace.LayerNum;iz++,pLayer++){
		glDrawLayerFile(pLayer,iz);
	}
}
static void glDoNothing(CGLView*pView){};
typedef void (*GLCREATEVIEWLIST)(CGLView*pView);
static const GLCREATEVIEWLIST iglCreateViewList[VIEW_TYPE_NUM] =
{
	glDraw2dShape,
	glCreate3dDesign,
	glCreate3dForcast,
	glCreate3dFile,
	glCreate3dExtreme,
	glDraw2dForcast,
	glDoNothing,
	glDoNothing
};


//SYSTEMTIME t;long sec,msec;
//	GetSystemTime(&t);sec=t.wSecond;msec=t.wMilliseconds;
///OPERATION
//	GetSystemTime(&t);msec=t.wMilliseconds-msec+1000*(t.wSecond-sec);
//	AppGetCtrlDlg()->SetDlgItemInt(IDC_CURRENT_OPERATION,msec);
void glCreateView(CGLView*pView){
	if(glIsList(1))glDeleteLists(1,1);
	if(pView->m_ViewType&VIEW_USE_TESS){
		if((pView->m_ViewType&0x0f)==(VIEW_3D_DESIGN&0x0f)){
			glCreate3dDesignTess();
		}else{
			glCreate3dExtremeTess();
		}
	}
	if(pView->m_ViewType&VIEW_USE_LIST){
		if(EntSpace.EntNum==0||EntSpace.LayerNum==0)return;
		glNewList(1,GL_COMPILE);
		iglCreateViewList[pView->m_ViewType&0x0f](pView);
		glEndList();
	}
}

static void glPreDraw2D(){
	float scale=1.0f;
	float xext=1.1f*(EntSpace.xmax-EntSpace.xmin),yext=1.1f*(EntSpace.ymax-EntSpace.ymin);
	if(xext==0||yext==0)scale=1.0f;
	else if(xext>yext)scale=2/xext;
	else scale=2/yext;
	scale/=AppParam.fScale;
	glScalef(scale,scale,1.0f);


}
static void glPreDraw3D(BOOL useZoom=TRUE)
{
	glRotatef(-45,1.0f,.0f,0.0f);
	glRotatef(30,.0f,.0f,1.0f);
//	glColor3f(0.0f,0.5f,0.9f);
	/////////////////////
	float scale=1.0f;
//	float ext=1.4f*(EntSpace.xmax-EntSpace.xmin),xyz;
//	xyz=1.4f*(EntSpace.ymax-EntSpace.ymin);	if(xyz>ext)ext=xyz;
//	xyz=1.4f*(EntSpace.zmax-EntSpace.zmin); if(xyz>ext)ext=xyz;
	float ext=(1.2f)*(SQRT(SQUARE(EntSpace.xmax-EntSpace.xmin)+
		SQUARE(EntSpace.ymax-EntSpace.ymin)+SQUARE(EntSpace.zmax-EntSpace.zmin)));
	if(ext==0)scale=1;
	else	scale=2/ext;
	if(useZoom)scale/=AppParam.fScale;
	glTranslatef(.0f,.0f,-.6f);
	glScalef(scale,scale,scale);
}


static void glDraw3dForcast(CGLView*pView){
	int iz;ELayer*pLayer;
#if USE_TIME_COUNTER	
	CTimeCounter tc;
#endif
	//生成画轮廓的执行表
	for(iz=0,pLayer=EntSpace.pLayer;iz<EntSpace.LayerNum-1;iz++,pLayer++){
		glDrawLayerForcast(pLayer,iz);
		pView->m_rePaintMutex.Lock();
		if(pView->m_rePaint
#if USE_TIME_COUNTER			
			&&tc.IsCountEnd()
#endif			
			){
			pView->m_rePaintMutex.Unlock();
//S\			SwapBuffers(pView->m_pDC->GetSafeHdc());
			return;
		}
		pView->m_rePaintMutex.Unlock();
	}

}

static void glDraw3dDesign(CGLView*pView){
	ELayer*pLayer;int i;
#if USE_TIME_COUNTER	
	CTimeCounter tc;
#endif
	for(i=0,pLayer=EntSpace.pLayer;i<EntSpace.LayerNum-1;i++,pLayer++){
		glDrawLayerDesign(pLayer,i);
		pView->m_rePaintMutex.Lock();
		if(pView->m_rePaint
#if USE_TIME_COUNTER			
			&&tc.IsCountEnd()
#endif					
			){
			pView->m_rePaintMutex.Unlock();
//S\			SwapBuffers(pView->m_pDC->GetSafeHdc());
			return;
		}
		pView->m_rePaintMutex.Unlock();
	}
}

static void glDraw3dExtreme(CGLView*pView){
	ELayer*pLayer;int i;
#if USE_TIME_COUNTER	
	CTimeCounter tc;
#endif
	for(i=0,pLayer=EntSpace.pLayer;i<EntSpace.LayerNum-1;i++,pLayer++){
		glDrawLayerExtreme(pLayer,i);
		pView->m_rePaintMutex.Lock();
		if(pView->m_rePaint
#if USE_TIME_COUNTER			
			&&tc.IsCountEnd()
#endif				
			){
			pView->m_rePaintMutex.Unlock();
//S\			SwapBuffers(pView->m_pDC->GetSafeHdc());
			return;
		}
		pView->m_rePaintMutex.Unlock();
	}
}

static void glDraw3dFile(CGLView*pView){
	int iz=0;ELayer*pLayer;
#if USE_TIME_COUNTER	
	CTimeCounter tc;
#endif
	//生成画轮廓的执行表
	glColor(pView->m_pColor);
	for(iz=0,pLayer=EntSpace.pLayer;iz<EntSpace.LayerNum;iz++,pLayer++){
			glDrawLayerFile(pLayer,iz);
			pView->m_rePaintMutex.Lock();
			if(pView->m_rePaint
#if USE_TIME_COUNTER			
			&&tc.IsCountEnd()
#endif					
				){
				pView->m_rePaintMutex.Unlock();
//S\				SwapBuffers(pView->m_pDC->GetSafeHdc());
				return;
			}
			pView->m_rePaintMutex.Unlock();
	}
}
static void glDraw3dProcess(CGLView*pView){
	int iz;ELayer*pLayer;
#if USE_TIME_COUNTER	
	CTimeCounter tc;
#endif
	//生成画轮廓的执行表
	for(iz=0,pLayer=EntSpace.pLayer;iz<pView->m_pProcessData->ilayer;iz++,pLayer++){
		glDrawLayerForcast(pLayer,iz);
		pView->m_rePaintMutex.Lock();
		if(pView->m_rePaint
#if USE_TIME_COUNTER			
			&&tc.IsCountEnd()
#endif			
			){
			pView->m_rePaintMutex.Unlock();
//S\			SwapBuffers(pView->m_pDC->GetSafeHdc());
			return;
		}
		pView->m_rePaintMutex.Unlock();
	}
	if(pView->m_pProcessData->icnc>0)
		glDrawLayerForcast(pLayer,iz,pView->m_pProcessData->icnc);

}
static void glDraw2dProcess(CGLView*pView){
	int i;float Px=0,Py=0;EntFull e;
	ELayer*pLayer=EntSpace.pLayer+pView->m_pProcessData->ilayer;
	if(pLayer==NULL)return;	
	for(i=0;i<3;i++){
		if(pView->m_pColor[i].is_random){
			pView->m_pColor[i].r=rand()%256;
			pView->m_pColor[i].g=rand()%256;
			pView->m_pColor[i].b=rand()%256;
		}
	}
	for(i=0;i<pView->m_pProcessData->icnc;i++){
		if(NcToEntFull(pLayer->pNcEnt+i,Px,Py,e)){
			if(pLayer->pNcEnt[i].ms){
				if(pLayer->pNcEnt[i].on_contour){
					glColorNoRandom(pView->m_pColor+2);
					if(pView->m_pColor[2].is_havewidth)
						glDrawEntFace(&e,AppParam.fOffsetDist);
					else glDrawEntLine(&e);	
				}else{
					glColorNoRandom(pView->m_pColor);					
					if(pView->m_pColor->is_havewidth)
						glDrawEntFace(&e,AppParam.fOffsetDist);
					else glDrawEntLine(&e);					
				}
			}else{
				if(pView->m_pColor[1].is_visible){
					glColorNoRandom(pView->m_pColor+1);
					glDrawEntLine(&e);
				}
			}
		}
	}
}
static void glDrawNull(CGLView*pView){};
typedef void (*GLDRAWVIEW)(CGLView*pView);
static const GLDRAWVIEW iglDrawView[VIEW_TYPE_NUM] =
{
	glDraw2dShape,
	glDraw3dDesign,
	glDraw3dForcast,
	glDraw3dFile,
	glDraw3dExtreme,
	glDraw2dForcast,
	glDraw3dProcess,
	glDraw2dProcess
};
void glDrawView(CGLView*pView){
	if(pView->m_ViewType&VIEW_2D)glPreDraw2D();
	else glPreDraw3D(pView->m_ViewType&VIEW_NOT_ORIGINAL);
	///调用列表
	if(pView->m_ViewType&VIEW_USE_LIST)glCallList(1);
	else	iglDrawView[pView->m_ViewType&0x0f](pView);
		
}

void glDrawViewBusy(int ViewType){
	if(EntSpace.EntNum==0||EntSpace.LayerNum==0)return;
	if(ViewType&VIEW_2D){
		if(m_LayerIndex>EntSpace.LayerNum||m_LayerIndex<1)return;
		glPreDraw2D();
		glScalef(AppParam.fScale,AppParam.fScale,AppParam.fScale);
		ELayerInfo ELI;
		LayerGetInfo(EntSpace.pLayer+m_LayerIndex-1,&ELI);
		glColor3f(0.0f,0.0f,0.0f);
		glBegin(GL_LINE_LOOP );
			glVertex2f(ELI.xmin,ELI.ymin);
			glVertex2f(ELI.xmax,ELI.ymin);
			glVertex2f(ELI.xmax,ELI.ymax);
			glVertex2f(ELI.xmin,ELI.ymax);
		glEnd();
	}else{
		glPreDraw3D(FALSE);	
		glBegin(GL_LINE_LOOP);
			glVertex3f(EntSpace.xmin,EntSpace.ymin,0);
			glVertex3f(EntSpace.xmax,EntSpace.ymin,0);
			glVertex3f(EntSpace.xmax,EntSpace.ymax,0);
			glVertex3f(EntSpace.xmin,EntSpace.ymax,0);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f(EntSpace.xmin,EntSpace.ymin,EntSpace.zmax);
			glVertex3f(EntSpace.xmax,EntSpace.ymin,EntSpace.zmax);
			glVertex3f(EntSpace.xmax,EntSpace.ymax,EntSpace.zmax);
			glVertex3f(EntSpace.xmin,EntSpace.ymax,EntSpace.zmax);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(EntSpace.xmin,EntSpace.ymin,0);
			glVertex3f(EntSpace.xmin,EntSpace.ymin,EntSpace.zmax);
			glVertex3f(EntSpace.xmax,EntSpace.ymin,0);
			glVertex3f(EntSpace.xmax,EntSpace.ymin,EntSpace.zmax);
			glVertex3f(EntSpace.xmax,EntSpace.ymax,0);
			glVertex3f(EntSpace.xmax,EntSpace.ymax,EntSpace.zmax);
			glVertex3f(EntSpace.xmin,EntSpace.ymax,0);	
			glVertex3f(EntSpace.xmin,EntSpace.ymax,EntSpace.zmax);			
		glEnd();

	}
}
///////////////////////////MULTITHREAD

void preMultiThread(CGLView*pView){
	AfxGetMainWnd()->GetSystemMenu(FALSE)->
		EnableMenuItem (SC_CLOSE,MF_BYCOMMAND | MF_GRAYED);
	if(pView)pView->GetParent()->GetSystemMenu(FALSE)->
		EnableMenuItem (SC_CLOSE,MF_BYCOMMAND | MF_GRAYED);
	threadcountmutex.Lock();
	threadcount++;
	threadcountmutex.Unlock();
}
void postMultiThread(CGLView*pView){
	threadcountmutex.Lock();
	threadcount--;
	threadcountmutex.Unlock();
	if(pView)pView->GetParent()->GetSystemMenu(FALSE)
		->EnableMenuItem (SC_CLOSE,MF_BYCOMMAND | MF_ENABLED);
	if(threadcount==0)AfxGetMainWnd()->GetSystemMenu(FALSE)->
		EnableMenuItem (SC_CLOSE,MF_BYCOMMAND | MF_ENABLED);
}
void glCreateViewMultiThread(CGLView*pView){
	wglMakeCurrent(pView->m_pDC->GetSafeHdc(),pView->hrc);
	glCreateView(pView);
	wglMakeCurrent(NULL,NULL);
	pView->m_rePaintMutex.Lock();
	pView->m_rePaint=PAINT_NONE;
	pView->m_rePaintMutex.Unlock();
	pView->Invalidate(FALSE);
	postMultiThread(pView);
}



BOOL glDrawEntOn3dView(CGLView*pView,EntFull*pEnt,float z,float zh,BOOL prems,BOOL ms){
	CRect rect;
	pView->GetParent()->GetClientRect(rect);
	if(rect.right>0){
		if(ms){
			pView->m_mtglMutex.Lock();
			wglMakeCurrent(pView->m_pDC->GetSafeHdc(),pView->hrcmtgl);
			pView->glCordinationTransform();
			glPreDraw3D();
			glDrawNcEntSolid(pEnt,AppParam.fOffsetDist,z,zh,prems);
			glFlush();		
			glFinish();		
			wglMakeCurrent(NULL,NULL);
			pView->m_mtglMutex.Unlock();
		}
		return FALSE;
	}else 
		return TRUE;
}


BOOL glDrawEntOn2dView(CGLView*pView,EntFull*pEnt,BOOL ms,BOOL on_contour){
	CRect rect;
	pView->GetParent()->GetClientRect(rect);
	if(rect.right>0){
		pView->m_mtglMutex.Lock();
		wglMakeCurrent(pView->m_pDC->GetSafeHdc(),pView->hrcmtgl);
		pView->glCordinationTransform();
		glPreDraw2D();
/*		if(ms)glDrawEntFace(pEnt,AppParam.fOffsetDist);
		else	glDrawEntLine(pEnt);*/
		////new
			if(ms){
				if(on_contour){
					glColorNoRandom(pView->m_pColor+2);
					if(pView->m_pColor[2].is_havewidth)
						glDrawEntFace(pEnt,AppParam.fOffsetDist);
					else glDrawEntLine(pEnt);	
				}else{
					glColorNoRandom(pView->m_pColor);					
					if(pView->m_pColor->is_havewidth)
						glDrawEntFace(pEnt,AppParam.fOffsetDist);
					else glDrawEntLine(pEnt);					
				}
			}else{
				if(pView->m_pColor[1].is_visible){
					glColorNoRandom(pView->m_pColor+1);
					glDrawEntLine(pEnt);
				}
			}



		////
		glFlush();		
		glFinish();		
		wglMakeCurrent(NULL,NULL);
		pView->m_mtglMutex.Unlock();
		return FALSE;
	}else
		return TRUE;
}

static BOOL AppEmulateToExit(){
		EmulateSts.pMutex->Lock();
		if(EmulateSts.emode==EMULATE_PAUZ||
			EmulateSts.emode==EMULATE_STOP)
		{
			EmulateSts.pMutex->Unlock();
			return TRUE;
		}
		EmulateSts.pMutex->Unlock();
		return FALSE;
}
static void AppEmulateSetPos(int i,int iz){
	EmulateSts.pMutex->Lock();
	EmulateSts.ilayer=iz;
	EmulateSts.icnc=i;
	EmulateSts.pMutex->Unlock();
}
static int   AppEmulateTestRepaint(CGLView*pView2d,CGLView*pView3d,int i,int iz){
	BOOL posSet=FALSE;int rt=0;
	pView2d->m_rePaintMutex.Lock();
	if(pView2d->m_rePaint==PAINT_ASSERT_PAINT){
		pView2d->glDoClear();
		AppEmulateSetPos(i,iz);posSet=TRUE;
		SetEvent(pView2d->m_pProcessData->hPaintAssert);
		pView2d->m_rePaintMutex.Unlock();	
		rt|=0x01;
	}else	pView2d->m_rePaintMutex.Unlock();
	pView3d->m_rePaintMutex.Lock();
	if(pView3d->m_rePaint==PAINT_ASSERT_PAINT){
		pView3d->glDoClear();
		if(!posSet)AppEmulateSetPos(i,iz);
		SetEvent(pView3d->m_pProcessData->hPaintAssert);
		pView3d->m_rePaintMutex.Unlock();	
		rt|=0x10;
	}else	pView3d->m_rePaintMutex.Unlock();
	return rt;
}

void AppEmulateThreadProc(WPARAM param){
	int i,iz,u,ib=0,izb=0;CRect rect;double len,dist;float adist;
	NcEnt*pEnt;EntFull e;float z,zh;float Px,Py;BOOL bp1,bp2,bover=FALSE;
	CGLView*pView3d=AppGetView(VIEW_3D_PROCESS);
	CGLView*pView2d=AppGetView(VIEW_2D_PROCESS);
	CCtrlBar*pcbar=AppGetCtrlDlg();
	//////////////////////////////
	ib=EmulateSts.icnc;izb=EmulateSts.ilayer;
	for(iz=izb;iz<EntSpace.LayerNum-1;iz++){
		BOOL prems=FALSE;
		pEnt=EntSpace.pLayer[iz].pNcEnt;
		u=EntSpace.pLayer[iz].iNcEntNum;
		z=EntSpace.pLayer[iz].z;
		zh=EntSpace.pLayer[iz+1].z;
		pcbar->SetPos((long)z);
		/////////////////////////2d window  clear on layer change
		if(ib==0){
			pView2d->glDoClear();
			Px=Py=0;
		}else{
			if(pEnt[ib-1].type==NCARC_CENTER){
				Px=pEnt[ib-2].x;
				Py=pEnt[ib-2].y;
			}else{
				Px=pEnt[ib-1].x;
				Py=pEnt[ib-1].y;
			}
		}
		////////////////////////////////////////////////////////

		for(i=ib;i<u;i++){
			if(!NcToEntFull(pEnt+i,Px,Py,e))continue;
			pcbar->SetGS(pEnt[i].ms);
			
				if(e.flag&ENT_ISLINE){
					len=sqrt(SQUARE(e.xend-e.xstart)+SQUARE(e.yend-e.ystart));
					dist=AppParam.fEstep;
					if(dist<len){
						e.xend=e.xstart;e.yend=e.ystart;
						do{
							e.xend+=(float)(AppParam.fEstep*cos(e.gout));
							e.yend+=(float)(AppParam.fEstep*sin(e.gout));
							////////////PAINTLINE
							bp1=glDrawEntOn3dView(pView3d,&e,z,zh,prems,pEnt[i].ms);
							bp2=glDrawEntOn2dView(pView2d,&e,pEnt[i].ms,pEnt[i].on_contour);
							if(bp1&&bp2)goto threadend;
							prems=TRUE;
							////////////////PAINTEND
							pcbar->SetPos((long)e.xend ,(long)e.yend);
							e.xstart=e.xend;e.ystart=e.yend;
							dist+=AppParam.fEstep;
						}while(dist<len);
						e.xend=pEnt[i].x;e.yend=pEnt[i].y;
					}

					////////////////PAINTEND					
				}else{
					float astep=AppParam.fEstep/e.r;
					float gend=e.gend;len=fabs(e.angle);
					if(e.angle<0)astep=-astep;
					adist=e.angle=astep;
					e.gend=e.gstart+astep;
					while(fabs(adist)<len){
						e.xend=float(e.xc+e.r*cos(e.gend));
						e.yend=float(e.yc+e.r*sin(e.gend));
						////////////PAINTARC
						bp1=glDrawEntOn3dView(pView3d,&e,z,zh,prems,pEnt[i].ms);
						bp2=glDrawEntOn2dView(pView2d,&e,pEnt[i].ms,pEnt[i].on_contour);
						if(bp1&&bp2)goto threadend;
						prems=TRUE;
						////////////////PAINTEND
						pcbar->SetPos((long)e.xend ,(long)e.yend);
						e.xstart=e.xend;e.ystart=e.yend;						
						adist+=astep;e.gstart=e.gend;e.gend+=astep;
					};
					e.gend=gend;e.angle=e.gend-e.gstart;
					e.xend=pEnt[i].x;e.yend=pEnt[i].y;
					////////////////PAINTEND
				}
				////////////PAINTLINE
				bp1=glDrawEntOn3dView(pView3d,&e,z,zh,prems,pEnt[i].ms);
				bp2=glDrawEntOn2dView(pView2d,&e,pEnt[i].ms,pEnt[i].on_contour);
				AppEmulateTestRepaint(pView2d,pView3d,i+1,iz);
				if((bp1&&bp2)||AppEmulateToExit())goto threadend;
				pcbar->SetPos((long)e.xend ,(long)e.yend);
				prems=pEnt[i].ms;
		}
		ib=0;
	}
	bover=TRUE;
threadend:
	pcbar->SetGS(FALSE);

	pView2d->m_rePaintMutex.Lock();
	pView3d->m_rePaintMutex.Lock();
	EmulateSts.pMutex->Lock();
	if(bover||EmulateSts.emode==EMULATE_STOP)EmulateSts.emode=EMU_PRD_NONE;
	else if(EmulateSts.emode==EMULATE_DUIN)EmulateSts.emode=EMULATE_PAUZ;
	EmulateSts.icnc=i;
	if(bover)EmulateSts.ilayer=iz-1;
	else EmulateSts.ilayer=iz;
	if(pView2d->m_rePaint==PAINT_ASSERT_PAINT)
		SetEvent(pView2d->m_pProcessData->hPaintAssert);
	if(pView3d->m_rePaint==PAINT_ASSERT_PAINT)
		SetEvent(pView3d->m_pProcessData->hPaintAssert);

	EmulateSts.pMutex->Unlock();
	pView3d->m_rePaintMutex.Unlock();
	pView2d->m_rePaintMutex.Unlock();

	postMultiThread(pView2d);
	postMultiThread(pView3d);
}
void CRPApp::OnEmulate() {
	if(EntSpace.EntNum==0||EntSpace.LayerNum==0)return;
	////////////////////SHOW　TWO　PROCESS　WINDOW
	CChildFrame*pFrame;
	for(int i=0;i<VIEW_TYPE_NOTPROCESS_NUM  ;i++){
		pFrame=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetChildFrame(i);
		if(pFrame)pFrame->CloseWindow();
	}
	CGLView*pView2d=(CGLView*)ShowChildWindow(VIEW_2D_PROCESS);
	preMultiThread(pView2d);
	CGLView*pView3d=(CGLView*)ShowChildWindow(VIEW_3D_PROCESS);
	preMultiThread(pView3d);
	((CMDIFrameWnd*) m_pMainWnd)->MDITile(MDITILE_VERTICAL);
	////////////////////////////////////
	if(EmulateSts.emode!=EMULATE_PAUZ){
		pView2d->glDoClear();
		pView3d->glDoClear();
		EmulateSts.icnc=0;
		EmulateSts.ilayer=0;
	}
	EmulateSts.emode=EMULATE_DUIN;
	AfxBeginThread((AFX_THREADPROC)AppEmulateThreadProc,NULL);
}
void AppEmulateQuickThreadProc(WPARAM param){
	int i,iz,u,ib,izb;CRect rect;BOOL bp1,bp2,bover=FALSE;
	NcEnt*pEnt;EntFull e;float z,zh;float Px,Py;
	CGLView*pView3d=AppGetView(VIEW_3D_PROCESS);
	CGLView*pView2d=AppGetView(VIEW_2D_PROCESS);
	CCtrlBar*pcbar=AppGetCtrlDlg();
	/////////////////////////3d window  clear on begin
	ib=EmulateSts.icnc;izb=EmulateSts.ilayer;
	//////////////////////////////
	for(iz=izb;iz<EntSpace.LayerNum-1;iz++){
		BOOL prems=FALSE;
		pEnt=EntSpace.pLayer[iz].pNcEnt;
		u=EntSpace.pLayer[iz].iNcEntNum;
		z=EntSpace.pLayer[iz].z;
		zh=EntSpace.pLayer[iz+1].z;
////////////////////////////////////////////////////////
		pcbar->SetPos((long)z);
		if(ib==0){
			pView2d->glDoClear();
			Px=Py=0;
		}else{
			if(pEnt[ib-1].type==NCARC_CENTER){
				Px=pEnt[ib-2].x;
				Py=pEnt[ib-2].y;
			}else{
				Px=pEnt[ib-1].x;
				Py=pEnt[ib-1].y;
			}
		}
		//////////////////////////////
		for(i=ib;i<u;i++){
			if(!NcToEntFull(pEnt+i,Px,Py,e))continue;
			pcbar->SetGS(pEnt[i].ms);
			pcbar->SetPos((long)Px,(long)Py);
			bp1=glDrawEntOn3dView(pView3d,&e,z,zh,prems,pEnt[i].ms);
			bp2=glDrawEntOn2dView(pView2d,&e,pEnt[i].ms,pEnt[i].on_contour);
			AppEmulateTestRepaint(pView2d,pView3d,i+1,iz);
			if((bp1&&bp2)||AppEmulateToExit())goto threadend;
			prems=pEnt[i].ms;
		}
		ib=0;
	}
	bover=TRUE;
threadend:
	pcbar->SetGS(FALSE);

	pView2d->m_rePaintMutex.Lock();
	pView3d->m_rePaintMutex.Lock();
	EmulateSts.pMutex->Lock();
	if(bover||EmulateSts.emode==EMULATE_STOP)EmulateSts.emode=EMU_PRD_NONE;
	else if(EmulateSts.emode==EMULATE_DUIN)EmulateSts.emode=EMULATE_PAUZ;
	EmulateSts.icnc=i;
	if(bover)EmulateSts.ilayer=iz-1;
	else EmulateSts.ilayer=iz;
	if(pView2d->m_rePaint==PAINT_ASSERT_PAINT)
		SetEvent(pView2d->m_pProcessData->hPaintAssert);
	if(pView3d->m_rePaint==PAINT_ASSERT_PAINT)
		SetEvent(pView3d->m_pProcessData->hPaintAssert);

	EmulateSts.pMutex->Unlock();
	pView3d->m_rePaintMutex.Unlock();
	pView2d->m_rePaintMutex.Unlock();

	postMultiThread(pView2d);
	postMultiThread(pView3d);
}			


void CRPApp::OnEmulateQuick() 
{
	if(EntSpace.EntNum==0||EntSpace.LayerNum==0)return;
	////////////////////SHOW　TWO　PROCESS　WINDOW
	CChildFrame*pFrame;
	for(int i=0;i<VIEW_TYPE_NOTPROCESS_NUM  ;i++){
		pFrame=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetChildFrame(i);
		if(pFrame)pFrame->CloseWindow();
	}
	CGLView*pView2d=(CGLView*)ShowChildWindow(VIEW_2D_PROCESS);
	preMultiThread(pView2d);
	CGLView*pView3d=(CGLView*)ShowChildWindow(VIEW_3D_PROCESS);
	preMultiThread(pView3d);
	((CMDIFrameWnd*) m_pMainWnd)->MDITile(MDITILE_VERTICAL);
	//////////////////////
	if(EmulateSts.emode!=EMULATE_PAUZ){
		pView2d->glDoClear();
		pView3d->glDoClear();
		EmulateSts.icnc=0;
		EmulateSts.ilayer=0;
	}
	EmulateSts.emode=EMULATE_DUIN;
	////////////////////////////////////
	AfxBeginThread((AFX_THREADPROC)AppEmulateQuickThreadProc,NULL);
	
}
void CRPApp::OnEmulatePause() 
{
	EmulateSts.pMutex->Lock();
	ASSERT(EmulateSts.emode=EMULATE_DUIN);
	EmulateSts.emode=EMULATE_PAUZ;
	EmulateSts.pMutex->Unlock();
}

void CRPApp::OnEmulateStop() 
{
	EmulateSts.pMutex->Lock();
	ASSERT(EmulateSts.emode=EMULATE_DUIN||EmulateSts.emode==EMULATE_PAUZ);
	EmulateSts.emode=EMULATE_STOP;
	EmulateSts.pMutex->Unlock();	
	
}

//////////////////////////////PRODUCE

//////////////////////MANUAFACTUR


static int AppProduceToExit(){
		EmulateSts.pMutex->Lock();
		if(EmulateSts.emode==PRODUCE_PAUZ){
			EmulateSts.pMutex->Unlock();
			return PRODUCE_PAUZ;			
		}else if(EmulateSts.emode==PRODUCE_STOP)
		{
			EmulateSts.pMutex->Unlock();
			return PRODUCE_STOP;
		}
		EmulateSts.pMutex->Unlock();
		return FALSE;
}
static int AppProduceExitOnWait(){
		EmulateSts.pMutex->Lock();
		if(EmulateSts.emode==PRODUCE_PAUZ){
//*			CardFunctions.PauseIpol();
			EmulateSts.pMutex->Unlock();
			return PRODUCE_PAUZ;			
		}else if(EmulateSts.emode==PRODUCE_STOP)
		{
//*			CardFunctions.StopIpol();
			EmulateSts.pMutex->Unlock();
			return PRODUCE_STOP;
		}
		EmulateSts.pMutex->Unlock();
		return FALSE;
}

void CardIpolEnt(EntFull*pEnt,float z,BOOL gzon=FALSE){
	double srcpos[3],dstpos[3],cntpos[3];
	srcpos[2]=dstpos[2]=cntpos[2]=z;
	srcpos[0]=pEnt->xstart;srcpos[1]=pEnt->ystart;
	dstpos[0]=pEnt->xend;dstpos[1]=pEnt->yend;
	if(pEnt->flag&ENT_NC_G00){
		long dst[3]={(long)dstpos[0],(long)dstpos[1],(long)dstpos[2]};
		RP_MoveTo(dst);		
	}else if(pEnt->flag&ENT_ISLINE){
		RP_LineXY(srcpos,dstpos,gzon);								
	}else{	
		cntpos[0]=pEnt->xc;cntpos[1]=pEnt->yc;	
		if(pEnt->angle>0)
			RP_ArcXY(srcpos,dstpos,cntpos,2);				
		else	
			RP_ArcXY(srcpos,dstpos,cntpos,1);											
			
	}

}
/*
int CardIpolZ(float z,float zdst){
	long pos[3];int rte;
	RP_MoveZ((long)zdst);
	while(RP_CardState(pos)){
		AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_Z,pos[2]);
		Sleep(110);
	}
	if(rte=AppProduceExitOnWait())return rte;
	else return 0;
	
}*/


BOOL AppTestCenter(){
#if MACHINE_HAVE_POWER
	if(AppParam.bCenterAjustEveryTime){
		if(!ZeroAjust())return FALSE;
	}else{
		long pos[3];

		if(zero_ok)
			pos[0]=pos[1]=pos[2]=0;
		else{
			RP_ZeroAbsolutePos(pos);
			pos[0]=-AppParam.lDist_O1_O-pos[0];
			pos[1]=-pos[1];pos[2]=-pos[2];
		}
		AppGetCtrlDlgItem(IDC_CURRENT_OPERATION)->SetWindowText("正在返回零点...");
		RP_MoveTo(pos);
		//////////////////////
		CCtrlBar*pcbar=AppGetCtrlDlg();
		while(RP_CardState(pos)){
			Sleep(110);
			pcbar->SetPos(pos[0],pos[1]);
			pcbar->SetPos(pos[2]);
		}
		//////////////////
		RP_SetZero();
		int rtn=AfxMessageBox("请确定到达了零点!",MB_YESNOCANCEL);
		if(rtn==IDNO){
			zero_ok=TRUE;
			if(!ZeroAjust())return FALSE;
		}else  return FALSE;
	}
	zero_ok=TRUE;
#else
	RP_SetZero();
#endif
	return TRUE;

}


void AppProduceLayer(ELayer*pLayer,CGLView*pView2d,CGLView*pView3d,int istart){



}
#define DID_NOTHING  0
#define DID_SOMETHING 1
#define DID_ALLTHING 2
void ParamMakeDir(EntFull*pEnt);
void AppManuFactureThreadProc(WPARAM param){
	float Px=0,Py=0;EntFull e;NcEnt*pNcEnt;
	float z,h;double nowpos[3]={0,0,0};
	short AxisNO[3]={0,0,0},iovalue=0;long pos[3];
	int i,u,iz=0,ests,cts,didwhat=DID_NOTHING,pzpos=PROD_PAUZ_NONE,prems=FALSE;
	
	CGLView*pView3d=AppGetView(VIEW_3D_PROCESS);
	CGLView*pView2d=AppGetView(VIEW_2D_PROCESS);
	CCtrlBar*pcbar=AppGetCtrlDlg();
	RP_BeginWork();
#if MACHINE_HAVE_POWER
#else 
	RP_SetZero();
#endif
	didwhat=DID_SOMETHING;
	for(iz=0;iz<EntSpace.LayerNum-1;iz++){
		///////////////////////////
		pNcEnt=EntSpace.pLayer[iz].pNcEnt;
		u=EntSpace.pLayer[iz].iNcEntNum;
		nowpos[2]=z=EntSpace.pLayer[iz].z;
		h=EntSpace.pLayer[iz+1].z-z;
		pcbar->SetPos((long)z);
		pView2d->glDoClear();
#if MACHINE_HAVE_POWER
		RP_BeginLayer();
		while(RP_CardState(pos)){
			AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_X,pos[0]);
			AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_Y,pos[1]);
			e.xend=(float)pos[0];e.yend=(float)pos[1];
			glDrawEntOn2dView(pView2d,&e,FALSE,FALSE);
			e.xstart=e.xend;e.ystart=e.yend;
			AppEmulateTestRepaint(pView2d,pView3d,i,iz);
			Sleep(110);		
		}
		if(ests=AppProduceExitOnWait()){
			pzpos=PROD_PAUZ_O1_TO_O;
			goto threadend;}
#endif
		//////实体加工指令
		nowpos[0]=nowpos[1]=0;
		for(i=0;i<u;i++){
			pcbar->SetGS(pNcEnt[i].ms);
			if(NcToEntFull(pNcEnt+i,Px,Py,e)){
				float xstart,ystart,gstart;
				xstart=e.xstart;ystart=e.ystart;gstart=e.gstart;
				CardIpolEnt(&e,z,pNcEnt[i].ms);
				///////直线
				e.flag|=ENT_ISLINE;
				////////////////////等待结束，不断检测位置
				Sleep(AppParam.ifreebackinterval );
				while(TRUE){
					cts=RP_CardState(pos);
					AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_X,pos[0]);
					AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_Y,pos[1]);
//					if(e.flag&ENT_ISLINE){///直线
						e.xend=(float)pos[0];e.yend=(float)pos[1];ParamMakeDir(&e);
						glDrawEntOn2dView(pView2d,&e,pNcEnt[i].ms,pNcEnt[i].on_contour);
						glDrawEntOn3dView(pView3d,&e,z,z+h,prems,pNcEnt[i].ms);
						ests=AppEmulateTestRepaint(pView2d,pView3d,i,iz);
						if(ests){e.xstart=xstart;e.ystart=ystart;							
						}else{e.xstart=e.xend;e.ystart=e.yend;}
/*					}else{//////////////////圆弧
						e.xend=(float)pos[0];e.yend=(float)pos[1];
						float ag=(float)atan2(pos[1]-e.yc,pos[0]-e.xc);
						e.angle=ag-e.gstart;
/*  					if(pNcEnt[i].type==NCARC_CCW){
							if(e.angle<0)e.angle+=TWOPI;
							else if(e.angle>=TWOPI)e.angle-=TWOPI;
						}else {
							if(e.angle>0)e.angle-=TWOPI;
							else if(e.angle<=-TWOPI)e.angle+=TWOPI;
						}*/
						while(e.angle>PI)e.angle-=TWOPI;
						while(e.angle<-PI)e.angle+=TWOPI;
						e.gend=e.angle+e.gstart;
						glDrawEntOn2dView(pView2d,&e,pNcEnt[i].ms,pNcEnt[i].on_contour);
						glDrawEntOn3dView(pView3d,&e,z,z+h,prems,pNcEnt[i].ms);						
/*						ests=AppEmulateTestRepaint(pView2d,pView3d,i,iz);
						if(ests){
							e.xstart=xstart;e.ystart=ystart;e.gstart=gstart;
							e.angle=e.gend-e.gstart;
							if(pNcEnt[i].type==NCARC_CCW){
								if(e.angle<0)e.angle+=TWOPI;
								else if(e.angle>=TWOPI)e.angle-=TWOPI;
							}else {
								if(e.angle>0)e.angle-=TWOPI;
								else if(e.angle<=-TWOPI)e.angle+=TWOPI;
							}
							e.gstart=e.gend-e.angle;
							if(ests&0x01)glDrawEntOn2dView(pView2d,&e,pNcEnt[i].ms,pNcEnt[i].on_contour);
							if(ests&0x10)glDrawEntOn3dView(pView3d,&e,z,z+h,FALSE,pNcEnt[i].ms);
						}*/
//						e.xstart=e.xend;e.ystart=e.yend;e.gstart=e.gend;
//					}
					prems=TRUE;
					if(cts==0)break;
					Sleep(AppParam.ifreebackinterval );
				}
				AppEmulateTestRepaint(pView2d,pView3d,i+1,iz);
				if(ests=AppProduceExitOnWait()){
					pzpos=PROD_PAUZ_IPOL_ENT;
					goto threadend;
				}
				nowpos[0]=e.xend;nowpos[1]=e.yend;
			}
			prems=pNcEnt[i].ms;
		}
		pcbar->SetGS(FALSE);
		//////////////////++++++++++O--->O1
#if MACHINE_HAVE_POWER
		RP_EndLayer();
		while(RP_CardState(pos)){
			AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_X,pos[0]);
			AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_Y,pos[1]);
			e.xend=(float)pos[0];e.yend=(float)pos[1];
			glDrawEntOn2dView(pView2d,&e,FALSE,FALSE);
			e.xstart=e.xend;e.ystart=e.yend;
			AppEmulateTestRepaint(pView2d,pView3d,i+1,iz);
			Sleep(50);		
		}
		if(ests=AppProduceExitOnWait()){
			pzpos=PROD_PAUZ_O_TO_O1;
			goto threadend;
		}

#else
		RP_SetZero();
#endif
		if(iz==EntSpace.LayerNum-2)break;
		////////////////////////上升
		RP_NextLayer(long(z+h));
		while(RP_CardState(pos)){
			AppGetCtrlDlg()->SetDlgItemInt(IDC_STATIC_Z,pos[2]);
			Sleep(110);
		}
		if(ests=AppProduceExitOnWait())
		{
			pzpos=PROD_PAUZ_IPOL_Z;
			goto threadend;
		}

	}
	didwhat=DID_ALLTHING;
	RP_EndWork();
threadend:
	EmulateSts.pMutex->Lock();
	if(didwhat==DID_ALLTHING||EmulateSts.emode==PRODUCE_STOP)EmulateSts.emode=EMU_PRD_NONE;
	else if(EmulateSts.emode==PRODUCE_DUIN)EmulateSts.emode=EMULATE_PAUZ;
	if(didwhat!=DID_NOTHING){
		EmulateSts.icnc=i;
		if(didwhat=DID_ALLTHING)EmulateSts.ilayer=iz-1;
		else EmulateSts.ilayer=iz;
	}
	EmulateSts.pMutex->Unlock();
	postMultiThread(pView2d);
	postMultiThread(pView3d);
}

void CRPApp::OnProduce() {
	if(EntSpace.EntNum==0||EntSpace.LayerNum==0){
		AfxMessageBox("缺少图形数据!");	return;}
	if(!InitRpcDll()||!RP_InitCards())return ;
	if(!AppTestCenter())return ;
	////////////////////SHOW　TWO　PROCESS　WINDOW
	CChildFrame*pFrame;
	for(int i=0;i<VIEW_TYPE_NOTPROCESS_NUM  ;i++){
		pFrame=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetChildFrame(i);
		if(pFrame)pFrame->CloseWindow();
	}
	CGLView*pView2d=(CGLView*)ShowChildWindow(VIEW_2D_PROCESS);
	preMultiThread(pView2d);
	CGLView*pView3d=(CGLView*)ShowChildWindow(VIEW_3D_PROCESS);
	preMultiThread(pView3d);
	((CMDIFrameWnd*) m_pMainWnd)->MDITile(MDITILE_VERTICAL);

	if(EmulateSts.emode!=PRODUCE_PAUZ){
		pView2d->glDoClear();
		pView3d->glDoClear();
		EmulateSts.icnc=0;
		EmulateSts.ilayer=0;
	}
	EmulateSts.emode=PRODUCE_DUIN;
	////////////////////////////////////
	AfxBeginThread((AFX_THREADPROC)AppManuFactureThreadProc,NULL);
}
////////////////////////////////////
//
//
//
///////////////////////////////////
void CRPApp::OnCenterAjust() {
	if(!InitRpcDll()||!RP_InitCards())return ;
	ZeroAjust();
}
BOOL CPropertySpacePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString str;
	str.Format("%f",(EntSpace.xmax-EntSpace.xmin)*AppParam.fScale);
	SetDlgItemText(IDC_RANGE_X,str);
	str.Format("%f",(EntSpace.ymax-EntSpace.ymin)*AppParam.fScale);
	SetDlgItemText(IDC_RANGE_Y,str);
	str.Format("%f",(EntSpace.zmax-EntSpace.zmin)*AppParam.fScale);
	SetDlgItemText(IDC_RANGE_Z,str);
	SetDlgItemInt(IDC_LAYERNUM,EntSpace.LayerNum);
	SetDlgItemInt(IDC_ENTNUM,EntSpace.EntNum);
	return TRUE; 
}
BOOL CPropertyLayerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CSpinButtonCtrl* pspin=(CSpinButtonCtrl*)GetDlgItem(IDC_LAYERSPIN);
	if(EntSpace.LayerNum>0){
	pspin->SetRange(1,EntSpace.LayerNum);
	pspin->SetPos(m_LayerIndex);
	}else{
		pspin->SetRange(1,1);
		pspin->SetPos(1);
		pspin->EnableWindow(FALSE);
		GetDlgItem(IDC_LAYERINDEX)->EnableWindow(FALSE);
	}
	return TRUE; 
}
void CPropertyLayerPage::OnChangeLayerindex() 
{
	if(EntSpace.LayerNum<1){
		
	}else{
		CSpinButtonCtrl* pspin=(CSpinButtonCtrl*)GetDlgItem(IDC_LAYERSPIN);
		int iLayerIndex=pspin->GetPos()-1;
		if(iLayerIndex<0||iLayerIndex>=EntSpace.LayerNum )return;
		ELayerInfo eli;CString str;
		str.Format("%f",EntSpace.pLayer[iLayerIndex].z);
		SetDlgItemText(IDC_LAYERZ,str);
		SetDlgItemInt(IDC_ENTNUM,EntSpace.pLayer[iLayerIndex].iEntNum);
		LayerGetInfo(EntSpace.pLayer+iLayerIndex,&eli);
		SetDlgItemInt(IDC_NCENTNUM,eli.iNcCount);
		SetDlgItemInt(IDC_LOOPNUM,eli.iLoopCount);
		if(eli.iLoopCount>0){
			str.Format("(%.2f)--(%.2f)",eli.xmin,eli.xmax);
			SetDlgItemText(IDC_RANGE_X,str);
			str.Format("(%.2f)--(%.2f)",eli.ymin,eli.ymax);
			SetDlgItemText(IDC_RANGE_Y,str);
		}
	}
}
void CRPApp::OnProperty() 
{
	CPropertySheet sheet("属性");
	CPropertySpacePage propspacepage;
	sheet.AddPage(&propspacepage);
	CPropertyLayerPage proplayerpage;
	if(EntSpace.LayerNum>0)sheet.AddPage(&proplayerpage);
	sheet.DoModal();
	
}
void CRPApp::OnExportPul() 
{

	if(EntSpace.LayerNum ==0)return;
	char szOpenFile[MAX_PATH]="\0";
	char szFileTitle[_MAX_FNAME]="\0";
	char szFilter[]="PUL文件(*.pul)\0*.pul\0所有文件(*.*)\0*.*\0\0";
	OPENFILENAMEEX ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAMEEX));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=m_pMainWnd->m_hWnd;
	ofn.lpstrFile=szOpenFile;
	ofn.nMaxFile=sizeof(szOpenFile); 
	ofn.lpstrFileTitle=szFileTitle;; 
	ofn.nMaxFileTitle=sizeof(szFileTitle); 
	ofn.Flags=OFN_OVERWRITEPROMPT; 
	ofn.nFilterIndex=1;
	ofn.lpstrFilter=szFilter;
	ofn.lpstrDefExt="pul";

	if(!GetSaveFileName((LPOPENFILENAME)&ofn))return;

	CString puls;
	CWaitCursor wc;
	::NcEntsToPul(&EntSpace,puls);
	CStdioFile pulfile(szOpenFile,CFile::modeWrite|CFile::modeCreate);
	pulfile.WriteString(puls);
	pulfile.Close();
}
void CMainFrame::OnPosWatch() 
{
	if(InitRpcDll())CardPosTest(m_hWnd,FALSE);
}
void CRPApp::OnCardTest() 
{	
	if(InitRpcDll())CardTest(m_pMainWnd->m_hWnd);
}
void OnParamsTest(int *PropModified);

void CRPApp::OnParams() {
	int pModified[3]={0,0,0};
	OnParamsTest(pModified);

	CWaitCursor wc;
	ELayer * pLayer;int i;
	if(pModified[0]==3){
		pLayer=EntSpace.pLayer;
		for(i=0;i<EntSpace.LayerNum;i++){
			ReleaseLoop(pLayer->pLoopOriginalBTree);
			ReleaseLoop(pLayer->pLoopOriginalTree,FALSE);
			pLayer->pLoopOriginalBTree=BuildLayerLoopOriginal(pLayer);
			if(pLayer->pLoopOriginalBTree)LoopArrangeDirection(pLayer->pLoopOriginalBTree);
			pLayer->pLoopOriginalTree=LoopTreeCopyNoEnt(pLayer->pLoopOriginalBTree);
			BuildLoopTreeFromBTree(pLayer->pLoopOriginalTree);
			pLayer++;
		}
	}
	if(pModified[1]>1||pModified[0]>1){
		pLayer=EntSpace.pLayer;
		for(i=0;i<EntSpace.LayerNum;i++){
			/////////////删除原来的树
			ReleaseLoop(pLayer->pLoopExtremeTree);
			ReleaseLoop(pLayer->pLoopOffsetBTree);
			ReleaseLoop(pLayer->pLoopOffsetTree,FALSE);
			////////////////
			pLayer->z=EntSpace.pz[i]*AppParam.fScale;
			pLayer->pLoopExtremeTree=LoopTreeCopy(pLayer->pLoopOriginalBTree,AppParam.fScale);
			//////////偏移环上的实体
			pLayer->pLoopOffsetBTree=BuildLayerLoopOffset(pLayer->pLoopExtremeTree,AppParam.fOffsetDist,TRUE);	
			//////检查环上的自交情况
			if(AppParam.bCheckSelfXAfterOffset &&pLayer->pLoopOffsetBTree)
				LoopSfxCheck(pLayer->pLoopOffsetBTree,TRUE);
			else LoopTreeGetExt(pLayer->pLoopOffsetBTree);
			pLayer->pLoopOffsetTree=LoopTreeCopyNoEnt(pLayer->pLoopOffsetBTree);
			BuildLoopTreeFromBTree(pLayer->pLoopExtremeTree);
			BuildLoopTreeFromBTree(pLayer->pLoopOffsetTree);
			if(pLayer->iPolygonsExtreme>0)ReleasePolygons(pLayer->pPolygonsExtreme);
			pLayer->pPolygonsExtreme=NULL;pLayer->iPolygonsExtreme=0;
			pLayer++;
			
		}
	}
	if(pModified[1]||pModified[0]){
		float angle=AppParam.fCncFirstAngle;
		pLayer=EntSpace.pLayer;
		for(i=0;i<EntSpace.LayerNum;i++){
			if(pLayer->iNcEntNum>0)delete[]pLayer->pNcEnt;
			/////生成扫描路径
			BuildLayerNC(pLayer,angle,AppParam.fCncLineDist,AppParam.fCncLenMin,
				AppParam.fOffsetDist,AppParam.bCncOrder,AppParam.bCncLineCross );
			angle+=AppParam.fCncAngleDelta;
			if(angle>=PI) angle-=PI;
			else if(angle<0)angle+=PI;
			pLayer++;
		}	
		CMainFrame*pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		pFrame->glRebuildView(VIEW_NOT_ORIGINAL);
	}

}

HKEY MyGetAppRegistryKey(HKEY HROOT=HKEY_LOCAL_MACHINE){
	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	if (RegOpenKeyEx(HROOT, _T("software"), 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hSoftKey, "中科大现代制造技术实验室", 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			RegCreateKeyEx(hCompanyKey, "Rapid Prototype", 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw);
		}
	}
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);
	return hAppKey;

}


void CRPApp::InitParams()
{
	/////////////////////硬件参数
	DWORD dw;DWORD dwValue;	DWORD dwType;DWORD dwCount = sizeof(DWORD);
	long lResult;BYTE bt;
	HKEY hAppKey=MyGetAppRegistryKey();HKEY hSecKey;
	if(hAppKey==NULL) return;
	if (RegCreateKeyEx(hAppKey, "硬件参数", 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSecKey, &dw) == ERROR_SUCCESS)
	{ 
		lResult=RegQueryValueEx(hSecKey,"原点距离" , NULL, &dwType,
			(LPBYTE)&dwValue, &dwCount);
		if(lResult==ERROR_SUCCESS)AppParam.lDist_O1_O=dwValue;		
		RegCloseKey(hSecKey);
	}
	RegCloseKey(hAppKey);
	//////////////////////软件参数

	AppParam.bCenterAjustEveryTime=(BYTE)GetProfileInt("参数","每次都调整中心",AppParam.bCenterAjustEveryTime);
	AppParam.fScale=GetProfileFloat("参数","缩放比例",AppParam.fScale);
	AppParam.fScalez=GetProfileFloat("参数","Z轴缩放比例",AppParam.fScalez);
	AppParam.fLayerDist=GetProfileFloat("参数","层叠厚度",AppParam.fLayerDist);
	AppParam.fOffsetDist=GetProfileFloat("参数","光斑半径",AppParam.fOffsetDist);
	AppParam.fCncLineDist=GetProfileFloat("参数","扫描线间距",AppParam.fCncLineDist);
	AppParam.fCncAngleDelta=GetProfileFloat("参数","层间角度变化",AppParam.fCncAngleDelta);
	AppParam.fCncFirstAngle=GetProfileFloat("参数","初始角度",AppParam.fCncFirstAngle);
	AppParam.bAutoSaveParam=GetProfileInt("参数","保存参数",AppParam.bAutoSaveParam);
	AppParam.ifreebackinterval=GetProfileInt("参数","反馈间隔",AppParam.ifreebackinterval);
	bt=GetProfileInt("参数","扫描方式",AppParam.bCncOrder);
	if(bt==0x01||bt==0x02||bt==0x06||bt==0x09)AppParam.bCncOrder=bt;
	AppParam.bCncLineCross=GetProfileInt("参数","十字网格平行栅",AppParam.bCncLineCross);
	dw=GetProfileInt("参数","自相交",0);
	if(dw&0x0001)AppParam.bCheckSelfXOnLoopBuild=1;
	if(dw&0x0010)AppParam.bCheckSelfXAfterLoopBuild=1;
	if(dw&0x0100)AppParam.bCheckSelfXAfterOffset=1;
	if(dw&0x1000)AppParam.bCheckCoXAfterOffset=1;
}

void CRPApp::SaveParams()
{
	////////////////////
	WriteProfileInt("参数","保存参数",AppParam.bAutoSaveParam);
	if(AppParam.bAutoSaveParam==0)return;
	/////////////////////硬件参数

	DWORD dw;long lResult;
	HKEY hAppKey=MyGetAppRegistryKey();HKEY hSecKey;
	if(hAppKey==NULL) goto regerror;
	if (RegCreateKeyEx(hAppKey, "硬件参数", 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSecKey, &dw) == ERROR_SUCCESS)
	{ 

		dw=AppParam.lDist_O1_O;
		lResult = RegSetValueEx(hSecKey, "原点距离", NULL, REG_DWORD,
			(LPBYTE)&dw, sizeof(DWORD));		
		RegCloseKey(hSecKey);
	}
	RegCloseKey(hAppKey);
	//////////////////////软件参数
	WriteProfileInt("参数","每次都调整中心",AppParam.bCenterAjustEveryTime);
	WriteProfileFloat("参数","缩放比例",AppParam.fScale);
	WriteProfileFloat("参数","Z轴缩放比例",AppParam.fScalez);
	WriteProfileFloat("参数","层叠厚度",AppParam.fLayerDist);
	WriteProfileFloat("参数","光斑半径",AppParam.fOffsetDist);
	WriteProfileFloat("参数","扫描线间距",AppParam.fCncLineDist);
	WriteProfileFloat("参数","层间角度变化",AppParam.fCncAngleDelta);
	WriteProfileFloat("参数","初始角度",AppParam.fCncFirstAngle);
	WriteProfileInt("参数","扫描方式",AppParam.bCncOrder);
	WriteProfileInt("参数","反馈间隔",AppParam.ifreebackinterval);
	dw=0;
	if(AppParam.bCheckSelfXOnLoopBuild)dw|=0x0001;
	if(AppParam.bCheckSelfXAfterLoopBuild)dw|=0x0010;
	if(AppParam.bCheckSelfXAfterOffset)dw|=0x0100;
	if(AppParam.bCheckCoXAfterOffset)dw|=0x1000;
	WriteProfileInt("参数","十字网格平行栅",AppParam.bCncLineCross);
	WriteProfileInt("参数","自相交",dw);
	return;
regerror:
	AfxMessageBox("注册表读取错误");
}


void CRPApp::OnUpdateItems(CCmdUI* pCmdUI) 
{
	threadcountmutex.Lock();
	if(threadcount>0)pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
	threadcountmutex.Unlock();
}
void CRPApp::OnUpdateEmulatePause(CCmdUI* pCmdUI) 
{
	EmulateSts.pMutex->Lock();
	if(EmulateSts.emode==EMULATE_DUIN)pCmdUI->Enable();
	else pCmdUI->Enable(FALSE);
	EmulateSts.pMutex->Unlock();
	
}

void CRPApp::OnUpdateEmulateStop(CCmdUI* pCmdUI) 
{
	EmulateSts.pMutex->Lock();
	if(EmulateSts.emode==EMULATE_DUIN||EmulateSts.emode==EMULATE_PAUZ)
		pCmdUI->Enable();
	else pCmdUI->Enable(FALSE);
	EmulateSts.pMutex->Unlock();
}


void CRPApp::OnProducePause() 
{

	EmulateSts.pMutex->Lock();
	ASSERT(EmulateSts.emode=PRODUCE_DUIN);
	EmulateSts.emode=PRODUCE_PAUZ;
	EmulateSts.pMutex->Unlock();
}

void CRPApp::OnUpdateProducePause(CCmdUI* pCmdUI) 
{
	EmulateSts.pMutex->Lock();
	if(EmulateSts.emode==PRODUCE_DUIN)pCmdUI->Enable();
	else pCmdUI->Enable(FALSE);
	EmulateSts.pMutex->Unlock();	
	
}

void CRPApp::OnProduceStop() 
{
	EmulateSts.pMutex->Lock();
	ASSERT(EmulateSts.emode=PRODUCE_DUIN||EmulateSts.emode==PRODUCE_PAUZ);
	EmulateSts.emode=PRODUCE_STOP;
	EmulateSts.pMutex->Unlock();	
}

void CRPApp::OnUpdateProduceStop(CCmdUI* pCmdUI) 
{
	EmulateSts.pMutex->Lock();
	if(EmulateSts.emode==PRODUCE_DUIN||EmulateSts.emode==PRODUCE_PAUZ)
		pCmdUI->Enable();
	else pCmdUI->Enable(FALSE);
	EmulateSts.pMutex->Unlock();
	
}





void glPaintViewProcessThreadProc(CGLView*pView){
	while(TRUE){
		pView->m_rePaintMutex.Lock();
		if(pView->m_rePaint==PAINT_TOPAINT){
			EmulateSts.pMutex->Lock();
			BOOL doclear=TRUE;
			if(EmulateSts.emode&EMU_RPD_DUIN){
				pView->m_rePaint=PAINT_ASSERT_PAINT;
				EmulateSts.pMutex->Unlock();
				pView->m_rePaintMutex.Unlock();
				WaitForSingleObject(pView->m_pProcessData->hPaintAssert,INFINITE);
				EmulateSts.pMutex->Lock();
				doclear=FALSE;

			}else pView->m_rePaintMutex.Unlock();
			pView->m_pProcessData->ilayer=EmulateSts.ilayer;
			pView->m_pProcessData->icnc=EmulateSts.icnc;
			EmulateSts.pMutex->Unlock();
			pView->m_rePaintMutex.Lock();
			pView->m_rePaint=PAINT_NONE;
			pView->m_rePaintMutex.Unlock();
			pView->m_pProcessData->phrcMutex->Lock();
			wglMakeCurrent(pView->m_pDC->GetSafeHdc(),pView->hrc);
			if(doclear)pView->glClearBeforePaint();
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
			wglMakeCurrent(NULL, NULL);
			pView->m_pProcessData->phrcMutex->Unlock();
		}else if(pView->m_rePaint==PAINT_EXIT){
			pView->m_rePaintMutex.Unlock();
			AfxEndThread(0);
		}else{
			pView->m_rePaintMutex.Unlock();
			WaitForSingleObject(pView->m_PaintThreadResume,INFINITE);
		}
	}

}
void CMainFrame::OnCardDebug() 
{
	if(rp_dllok)	RP_Debug();
}
void CMainFrame::OnUpdateCardDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(rp_dllok);
}

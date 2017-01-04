#include "stdafx.h"
#include "types.h"
#include "ent.h"
#include "math.h"
#include "epolyline.h"
#include <afxtempl.h>

///////////
//////MACRO DEFINITION
#define DELETEINVALID 1
#define NEWBEGIN 1
#define  FULLTEST 0

//////////////////////local functions

float PtLineAngle(float x,float y,EntFull *pEnt);
/////////返回线条 pEnt相对于点(X,Y)的张角
////////若(X,Y)在 pEnt上则返回 ANGLEERROR

int PtInLoop(float x,float y, ELoop *pLoop);
//////////看点(X，Y)是否在环pLoop内
//////////是	返回	1，
////////不是	返回	0，
//无法判断则	返回	2
////\/\//\/\/\\//\\//\\/\/\/\/\/\/////

int LoopRelationSimple(ELoop*pLoop1,ELoop*pLoop2);
static int LoopRelationSimple2(ELoop *pLoop1,ELoop * pLoop2);
///返回值：LOOP_BEOUT		pLoop2	在		pLoop1外
////////// LOOP_CONTAIN		pLoop1	包含	pLoop2
////////// LOOP_CONTAINED	pLoop2	包含	pLoop1 
////////// LOOP_UNKONW		无法用此办法判断
////////////\/\/\/\\/\/\/\\/\/\/\\/\////
static ELoop* BuildLoopBTree(CTypedPtrArray<CPtrArray,ELoop*>&ELoopArray);
/////////生成包含环内外关系的二叉树
void BuildLoopTreeFromBTree(ELoop*pLoopBtree);
/////////生成包含区域内外关系的二层树
//////////////////////////////////
void LoopGetExt(ELoop*ploop);
/////////////////////////
ELoop* BuildLayerLoopOriginal(ELayer*pLayer){
	int i,LoopNum=0;
	ELoop *pELoopTree=NULL;EntFull *pEf=NULL;
	//////计算该层的线条的起始
	////////生成环/\/\/\/\\/\/\/\/\\/\/\/\\\/\/\/
	CTypedPtrArray<CPtrArray,EPolyLine*> PlArray,LoopArray;
	CTypedPtrArray<CPtrArray,ELoop*>ELoopArray;	
	EPolyLine* pEpl;
	int j,appendok=0;
	for(i=0;i<pLayer->iEntNum;i++){	
		///////生成该层的环元素
		pEf=new EntFull;
		EntGetFull(pEf,pLayer->pEntRaw+i);
		pEpl=new EPolyLine(pEf);
		if(pEf->flag==ENT_ISCIRCLE){
			LoopArray.Add(pEpl);
			continue;
		}else
		{
			appendok=0;
			do{
				for(j=0;j<PlArray.GetSize();j++){
					if(appendok=PlArray[j]->Append(pEpl))
						break;
				}
				if(appendok==0) PlArray.Add(pEpl);
				else if(appendok==2) {
					pEpl=PlArray[j];
					PlArray.RemoveAt(j);
					LoopArray.Add(pEpl);
				}
				else if(appendok==1){
					if(PlArray.GetSize()<=1)break;
					pEpl=PlArray[j];
					PlArray.RemoveAt(j);				
				}else if(appendok==3){
	//				ASSERT(FALSE);
					LoopArray.Add(pEpl);
				}
			}while(appendok==1&&PlArray.GetSize());
		}
	}

	if(PlArray.GetSize()>0){
//		AfxMessageBox("曲线不封闭");
		for(i=0;i<PlArray.GetSize();i++){
			EPolyLine *ppp=PlArray[i];
			delete PlArray[i];
		}
	}
	LoopNum=LoopArray.GetSize();
	if(LoopNum==0){
#if _DEBUG
		AfxMessageBox("层上没有环");
		return NULL;
#else
		return NULL;
#endif

	}
	for(i=0;i<LoopNum;i++){
		ELoopArray.Add(LoopArray[i]->EGetLoop());
		delete LoopArray[i];
	}
	LoopArray.RemoveAll();
	////\/\/\/\\/\\/生成环结束 \/\/\/\/\\/\/\\/\/\/\\/\/
	//////////////处理环的内外关系\\\\\\\\\\\\\\\\\\\\\\/
	pELoopTree=BuildLoopBTree(ELoopArray);
	ELoopArray.RemoveAll();
	//////////////处理环的内外关系结束\\\\\\\\\\\\\\\\\\/
	return pELoopTree;
}
/////////生成包含环内外关系的二叉树
static ELoop* BuildLoopBTree(CTypedPtrArray<CPtrArray,ELoop*>&ELoopArray){
	CTypedPtrArray<CPtrArray,ELoop*>  inner,outer;
	int i,rela,Imax=0,size=ELoopArray.GetSize();
	if(size==1)return ELoopArray[0];
	else if(size==0)return NULL;
	float dxmax=0,dymax=0,dx,dy;
	ELoop*ploop;
	for(i=0;i<size;i++){
		ploop=ELoopArray[i];
		dx=ploop->xmax-ploop->xmin;
		dy=ploop->ymax-ploop->ymin;
		if(dx>dxmax||(dx==dxmax&&dy>dymax)||
			dx==dxmax&&dy==dymax&&LoopRelationSimple(ELoopArray[Imax],ploop)==LOOP_CONTAINED)
		{dxmax=dx;dymax=dy;Imax=i;}
		
	}
	ploop=ELoopArray[Imax];
	ELoopArray.RemoveAt(Imax);
	size--;
	for(i=0;i<size;i++){
		rela=LoopRelationSimple(ploop,ELoopArray[i]);
		if(rela==LOOP_BEOUT)outer.Add(ELoopArray[i]);
		else if(rela==LOOP_CONTAIN) inner.Add(ELoopArray[i]);
		else AfxMessageBox("区域内外关系错误");
	}
	ploop->inner=BuildLoopBTree(inner);
	ploop->next=BuildLoopBTree(outer);
	inner.RemoveAll();
	outer.RemoveAll();
	return ploop;
}
//////////生成包含区域内外关系的二层树
void BuildLoopTreeFromBTree(ELoop*pLoopBtree){
	if(pLoopBtree==NULL)return;
	ELoop*ploop=pLoopBtree;
	ELoop*pRear=pLoopBtree;
	while(pRear->next)pRear=pRear->next;
	while(ploop){
		ELoop *pl=ploop->inner;
		while(pl)
		{
			pRear->next=pl->inner;
			pl->inner=NULL;
			while(pRear->next)pRear=pRear->next;
			pl=pl->next;
		}
		ploop=ploop->next;
	}
}
/////////返回线条 pEnt相对于点(X,Y)的张角
////////若(X,Y)在 pEnt上则返回 ANGLEERROR
float PtLineAngle(float x,float y,EntFull *pEnt){
	float ag;
	float dx1,dy1,dx2,dy2,d1,d2;
	if(pEnt->flag&ENT_ISLINE){
		dx1=pEnt->xstart-x;	dy1=pEnt->ystart-y;
		dx2=pEnt->xend-x;	dy2=pEnt->yend-y;
//		if((dx1*dx2<0 && ZERO(dy2*dx1-dy1*dx2)))
		if(ZERO(dy2*dx1-dy1*dx2)&&EntPtOnLine(pEnt,x,y))
			return ANGLEERROR;
		d1=SQRT(dx1*dx1+dy1*dy1);
		d2=SQRT(dx2*dx2+dy2*dy2);
		ag=(dx1*dx2+dy1*dy2)/d1/d2;
		//////
		if(ag<-1)ag=-1;
		else if(ag>1)ag=1;
		///////
		ag=float(acos(ag));
		if((dy2*dx1-dy1*dx2)<0)ag=-ag;

	}
	else{
		if(pEnt->flag&ENT_ISCIRCLE){
			float dxc,dyc,dc;
			dxc=float(pEnt->xc-x);
			dyc=float(pEnt->yc-y);
			dc=SQRT(dxc*dxc+dyc*dyc);
			if(EQU(dc,pEnt->r))return ANGLEERROR;
			else if(dc<pEnt->r)return pEnt->angle;
			else return 0;	
		}else
		{
			float dxc,dyc,dc,ax;
			float a,b,c;///连线段的直线方程ax+by+c=0;
			dxc=float(pEnt->xc-x);
			dyc=float(pEnt->yc-y);
			dc=SQRT(dxc*dxc+dyc*dyc);
			ax=float(atan2(-dyc,-dxc));
			if(EQU(dc,pEnt->r)&&EntPtOnArc(pEnt,ax))return ANGLEERROR;
			dx1=pEnt->xstart-x;dy1=pEnt->ystart-y;
			dx2=pEnt->xend -x;dy2=pEnt->yend-y;
			d1=SQRT(dx1*dx1+dy1*dy1);
			d2=SQRT(dx2*dx2+dy2*dy2);
			ag=(dx1*dx2+dy1*dy2)/d1/d2;
			//////
			if(ag<-1)ag=-1;
			else if(ag>1)ag=1;
			///////
			ag=float(acos(ag));
			//////点和弧在边线段的同侧 且 点在圆内
			a=dy2-dy1;b=dx1-dx2;c=dx2*dy1-dx1*dy2;
			dxc=float(pEnt->xc+pEnt->r*cos((pEnt->gstart+pEnt->gend)/2)-x);
			dyc=float(pEnt->yc+pEnt->r*sin((pEnt->gstart+pEnt->gend)/2)-y);
			if(((dc-pEnt->r)/pEnt->r<-TOL)&&((a*dxc+b*dyc+c)/c>TOL))
				ag-=2*PI;
			if((dy2*dx1-dy1*dx2)<0)ag=-ag;
		}

	}

return ag;
}
//////////看点(X，Y)是否在环pLoop内
//////////是	返回	1，
////////不是	返回	0，
//无法判断则	返回	2
int PtInLoop(float x,float y, ELoop *pLoop){
	EntFull *pEnt=pLoop->loop;
	float angle=0,da;
	if(x<pLoop->xmin||x>pLoop->xmax||y<pLoop->ymin||y>pLoop->ymax)return 0;
	do{
		da=PtLineAngle(x,y,pEnt);
		if(da==ANGLEERROR)return 2;
		if(pEnt->flag&ENT_ISFLIP) angle-=da;
		else angle+=da;
		pEnt=pEnt->next;
	}while(pEnt&&pEnt!=pLoop->loop);
	if(fabs(angle)<0.5) return 0;
	else if(fabs(fabs(angle)-TWOPI)<0.5) return 1;
	else return 2;
}
///返回值：0  pLoop2	在		pLoop1外
////////// 1  pLoop1	包含	pLoop2
////////// 2  pLoop2	包含	pLoop1 
////////// 4  无法用此办法判断
int LoopRelationSimple(ELoop*pLoop1,ELoop*pLoop2){
	int rela;
	if(  pLoop1->xmax>pLoop2->xmax&&
		 pLoop1->xmin<pLoop2->xmin&&		 
		 pLoop1->ymax>pLoop2->ymax&&
		 pLoop1->ymin<pLoop2->ymin)
	{
		rela=LoopRelationSimple2(pLoop1,pLoop2);
		ASSERT(rela!=LOOP_CONTAINED);
		return rela;
	}else if(	pLoop2->xmax>pLoop1->xmax&&
				pLoop2->xmin<pLoop1->xmin&&		 
				pLoop2->ymax>pLoop1->ymax&&
				pLoop2->ymin<pLoop1->ymin)
	{
		rela=LoopRelationSimple2(pLoop1,pLoop2);
		ASSERT(rela!=LOOP_CONTAIN);
		return rela;
	}
	else return LOOP_BEOUT;

}
int LoopRelationSimple2(ELoop *pLoop1,ELoop * pLoop2){
	int inloop,intmp;
	EntFull*pEnt=pLoop1->loop;
	do{
		inloop=PtInLoop(pEnt->xstart,pEnt->ystart,pLoop2);
		if(inloop==0)break;
		else if(inloop==1)return LOOP_CONTAINED;
		pEnt=pEnt->next;
	}while(inloop==2&&pEnt!=pLoop1->loop);
	pEnt=pLoop2->loop;
	intmp=inloop;
	do{
		inloop=PtInLoop(pEnt->xstart,pEnt->ystart ,pLoop1);
		if(inloop==1&&intmp==0)return LOOP_CONTAIN;
		else if(inloop==0)return LOOP_BEOUT;
		pEnt=pEnt->next;
	}while(inloop==2&&pEnt!=pLoop2->loop);
	AfxMessageBox("无法判断环的内外关系!");
	return LOOP_UNKNOWN;
}

int LoopContainSimple(ELoop*pLoop1,ELoop*pLoop2){
	int inloop;
	EntFull*pEnt=pLoop2->loop;	
	pEnt=pLoop2->loop;
	do{
		inloop=PtInLoop(pEnt->xstart,pEnt->ystart ,pLoop1);
		if(inloop==1)return LOOP_CONTAIN;
		else if(inloop==0)return LOOP_BEOUT;
		pEnt=pEnt->next;
	}while(inloop==2&&pEnt!=pLoop2->loop);
	return LOOP_UNKNOWN;
}

//////一条环的绕向判断
//////返回1  如果此环是逆时针
//////返回-1 如果此环是顺时针
/////返回 0  如果无法判断
int LoopDirection(ELoop*ploop){
	int botype=0,by;
	float da;
	ASSERT(ploop);
	EntFull*p=ploop->loop->pre,*pEnt;
	EntFull*pEnd=ploop->loop->next;
	if(pEnd!=p)pEnd=pEnd->next;
	if(p->flag&ENT_ISCIRCLE){
		if(p->flag&ENT_ISFLIP){
			if(p->angle>0)return -1;
			else return 1;
		}else{
			if(p->angle>0)return 1;
			else return -1;
		}
	}
	do{
		by=EntHaveBottom(p,ploop->ymin,p->flag&ENT_ISFLIP);
		if(by<botype&&by==0)break;
		if(by>botype) {pEnt=p;botype=by;}
		else if(by==2 && pEnt->flag&ENT_ISLINE &&!( p->flag&ENT_ISLINE) ){
//			ASSERT(FALSE);
			pEnt=p;botype=by;}
		else if(by==2&& (pEnt->flag&ENT_ISLINE)&& (p->flag&ENT_ISLINE)){
//			ASSERT(FALSE);
			by=1;pEnt=p;botype=by;break;}
		p=p->next;
	}while(p!=pEnd);
	ASSERT(botype);
	if(botype==0)return 0;
	if(pEnt->flag&ENT_ISFLIP)FlipEnt(pEnt);
	if(pEnt->next->flag&ENT_ISFLIP)FlipEnt(pEnt->next);
	da=EntDirChange(pEnt,pEnt->next,0);
	if(botype==2||(botype==1&&AZERO(da))){
		if(pEnt->flag&ENT_ISLINE){
			if(pEnt->xstart<pEnt->xend) return 1;
			else return -1;
		}else{
			if(pEnt->angle <0)return -1;
			else return 1;
		}
	}else if (botype==1){
		if(da>0)return 1;
		else if(da<0)return -1;
	}
//	AfxMessageBox("无法找到最低点");
	ASSERT(FALSE);
	return 0;
}
//////使环首末倒置，反向
void ReverseLoop(ELoop*ploop){
	EntFull *p=ploop->loop,*t;
	do{
		p->flag^=ENT_ISFLIP;
		t=p->next;
		p->next=p->pre;
		p->pre=t;p=t;
	}while(p!=ploop->loop);
}
//////////从内存中删除一条环
void ReleaseLoop(ELoop* ploop,BOOL DeleteEnt){
	if(!ploop)return;
	if(ploop->next)ReleaseLoop(ploop->next,DeleteEnt);
	if(ploop->inner)ReleaseLoop(ploop->inner,DeleteEnt);
	if(!ploop->loop)return;
	if(DeleteEnt){
		EntFull*pf=ploop->loop,*p;
		pf->pre->next=NULL;
		while(pf){p=pf->next;delete pf;pf=p;}
	}
	delete ploop;
}
int EntInLoop(EntFull*pEnt,ELoop*ploop){


return 0;
}
static int LoopRelationFull(EntFull*ploop1,EntFull*ploop2){


return LOOP_UNKNOWN;
}

int LoopRelationFull(ELoop*pLoop1,ELoop*pLoop2){
	if(
		 pLoop1->xmin>pLoop2->xmax||
		 pLoop1->xmax<pLoop2->xmin||		 
		 pLoop1->ymin>pLoop2->ymax||
		 pLoop1->ymax<pLoop2->ymin)
			return LOOP_BEOUT;	
	else return LoopRelationFull(pLoop1->loop,pLoop2->loop);
}

////////检查空圆弧
static void LoopEntNullCheck(ELoop*ploop){
	EntFull *pEnt;
	pEnt=ploop->loop;
	do{
		if((pEnt->flag&ENT_ISLINE)&&PTEQU(pEnt->xstart,pEnt->xend,pEnt->ystart,pEnt->yend)
			||!(pEnt->flag&ENT_ISLINE)&&(pEnt->r==0||pEnt->angle==0))
		{
				pEnt->flag|=ENT_ISNULL;
			
		}
		pEnt=pEnt->next;
	}while(pEnt!=ploop->loop);
//	if(ploop->next)LoopArcCheck(ploop->next);
//	if(ploop->inner)LoopArcCheck(ploop->inner);
}
///////删除　环中的空实体
static void LoopEntNullRemove(ELoop*ploop){
	EntFull *pEnt,*pdel;

	while(ploop->loop->flag&ENT_ISNULL){
			pEnt=ploop->loop;
			ploop->loop=pEnt->next;
			ASSERT(ploop->loop);
			ploop->loop->pre=pEnt->pre;
			pEnt->pre->next=ploop->loop;
			delete pEnt;
	}
	pEnt=ploop->loop;

	do{
		if(pEnt->flag&ENT_ISNULL)
		{
			pdel=pEnt;
			pEnt->next->pre=pEnt->pre;
			pEnt->pre->next=pEnt->next;
			pEnt=pEnt->next;
			delete pdel;
		} else pEnt=pEnt->next;
	}while(pEnt!=ploop->loop);

}
////检查实体是否为空
static void LoopEntCheck(ELoop*ploop,EntFull*&p){
	ASSERT(ploop->loop);
	if(ploop->loop==p){
		if(ploop->loop->flag&ENT_ISNULL){
			ploop->loop=p->next;
			ASSERT(ploop->loop);
			ploop->loop->pre=p->pre;
			p->pre->next=ploop->loop;
			delete p;
			p=ploop->loop;
		}
	}else if(p->flag&ENT_ISNULL){
		EntFull*pnext=p->next;;
		p->next->pre=p->pre;
		p->pre->next=p->next;
		delete p;
		p=pnext;
	}
}
//////看有没有超过2pi的圆弧
static ELoop* LoopFromArc(EntFull*pArc){
	ASSERT(pArc);
	if(pArc->flag&ENT_ISLINE)return NULL;
	if(fabs(pArc->angle)>=TWOPI){
		float ag;
		if(pArc->angle>0){pArc->angle-=TWOPI;ag=PI;}
		else {pArc->angle+=TWOPI;ag=-PI;}
		ELoop*ploop=new ELoop;
		EntFull*p=new EntFull;
		ploop->loop=p;
		///////////////////////////
		EntCopy(p,pArc);
		p->gstart=0;p->gend=p->angle=ag;
		ParamMakeDir(p);ParamMakeExt(p);
		/////////////////////////////
		p->next=p->pre=new EntFull;p=p->next;
		EntCopy(p,ploop->loop);
		p->angle=ag;
		if(ag<0){p->gstart=PI;p->gend=0;}
		else{p->gstart=PI;p->gend=TWOPI;}
		ParamMakeDir(p);ParamMakeExt(p);
		//////////////////////
		p->next=p->pre=ploop->loop;
		ploop->next=ploop->inner=NULL;
		ploop->xmax=p->xc+p->r;ploop->xmin=p->xc-p->r;
		ploop->ymax=p->yc+p->r;ploop->ymin=p->yc-p->r;
		return ploop;
	}else return NULL;

}
/////得到环的范围
void LoopGetExt(ELoop*ploop){
	ASSERT(ploop);
	ASSERT(ploop->loop);
	EntFull*pEnt=ploop->loop,*pb;
	pb=pEnt;
	if(!(pEnt->flag&ENT_EXTVALID))ParamMakeExt(pEnt);
	ploop->xmin=pEnt->xmin;ploop->ymin=pEnt->ymin;
	ploop->xmax=pEnt->xmax;ploop->ymax=pEnt->ymax;
	if(pEnt->flag&ENT_ISCIRCLE){
	}else{
		while(pEnt->next!=ploop->loop){
			pEnt=pEnt->next;
			if(!(pEnt->flag&ENT_EXTVALID))
				ParamMakeExt(pEnt);		
			if(pEnt->xmax>ploop->xmax)ploop->xmax=pEnt->xmax;
			if(pEnt->ymax>ploop->ymax)ploop->ymax=pEnt->ymax;
			if(pEnt->xmin<ploop->xmin)ploop->xmin=pEnt->xmin;
			if(pEnt->ymin<ploop->ymin){ploop->ymin=pEnt->ymin;pb=pEnt;}
		}
	ploop->loop=pb;
	}
}

////////


static void LoopEntRelease(ELoop*&ploop,EntFull*ps,EntFull*pe){
	EntFull*p=ps->next,*pt;
	while(p!=pe){
		if(p==ploop->loop)ploop->loop=ps;
		pt=p;p=p->next;
		delete pt;
	}
}

//////解析环的自相交
static ELoop* LoopSelfX(ELoop*ploop,EntFull*&pEnt1,EntFull*pEnt2){
	float x,y,ag1,ag2;int p;
	if(!pEnt1||!pEnt2)return NULL;
	p=XEntEnt(pEnt1,pEnt2,x,y,ag1,ag2,TRUE);
	if(!(p==CROSSONEND||p==CROSSINCURVE||p==STARTONEND||p==STARTINCURVE))p=NOCROSS;
	if(p==NOCROSS)return NULL;
	else{
		ELoop*plp=new ELoop;
		plp->xmax=plp->xmin=plp->ymax=plp->ymin=0;
		plp->inner=plp->next=NULL;
		EntFull*ple1,*ple2,*pt;
		ple1=EntBreakEnd(pEnt1,x,y,ag1,p);
		ple2=EntBreakStart(pEnt2,x,y,ag2,p);
		
		if(pEnt1->next==pEnt2){
			ple1->next=ple2;
			ple2->pre=ple1;
		}else{
			ple1->next=pEnt1->next;
			ple2->pre=pEnt2->pre;
			ple1->next->pre=ple1;
			ple2->pre->next=ple2;
		}
		ple1->pre=ple2;ple2->next=ple1;
		plp->loop=ple1;
		pt=ple1->next;
		do{
			if(pt==ploop->loop){
				ploop->loop=pEnt2;
				break;
			}
			pt=pt->next;
		}while(pt!=plp->loop);
		if(pEnt1->flag&ENT_ISNULL){
			if(pEnt1==ploop->loop)	ploop->loop=pEnt2;
			pEnt1=pEnt1->pre;
			delete pEnt1->next;

		}
		if(pEnt2->flag&ENT_ISNULL){
			if(pEnt2==ploop->loop) ploop->loop=pEnt2->next;
			pEnt2=pEnt2->next;
			delete pEnt2->pre;
		}
		pEnt1->next=pEnt2;
		pEnt2->pre=pEnt1;
		return plp;
	}
}

void LoopSfxCheck(ELoop*&ploop,BOOL exterior){
	CTypedPtrArray<CPtrArray,ELoop*> LoopArray;
	EntFull*p=ploop->loop,*q,*ph=NULL,*pr=NULL,*pl;
	ELoop*plp;
	ELoop*inner=ploop->inner,*next=ploop->next;
	ploop->inner=ploop->next=NULL;
	int i=1,j=1,k=0;
	///////////////
	if(p->flag&ENT_ISCIRCLE){
		if(next)LoopSfxCheck(next,exterior);
		ploop->next=next;
		if(inner)LoopSfxCheck(inner,!exterior);
		ploop->inner=inner;
		return;
	}
	//////检查有没有超过2PI的圆
	do{
		plp=LoopFromArc(p);
		if(plp){
			LoopArray.Add(plp);
			if(p->angle==0)p->flag|=ENT_ISNULL;}
		p=p->next;
	}while(p!=ploop->loop);
	LoopEntNullRemove(ploop);//删除空的实体
	/////找一个有效的头
	//////开始检查环的自交情况
	//////生成一系列不自交的环
	p=ploop->loop;q=p->next;
	pl=ploop->loop;
	while(q->next!=p){
		plp=LoopSelfX(ploop,p,q);
		if(plp){
			LoopArray.Add(plp);	
#if NEWBEGIN 
			p=ploop->loop;
			q=p->next;
			i=j=1;
#else
//			p=ph=pp->next;
			ASSERT(p);
			ph=p;
			pr=q=p->next;
			j=1;k=1;
#endif

		}else if(j==i){
			p=p->next;
			q=q->next;
			if(p==ploop->loop){
				i++;
				j++;
				q=q->next;
			}
		}else if(j<i){
			if(k<j){
				p=p->next;
				q=q->next;
				k++;
			}else if(k==j){
				j++;
				p=ph=ph->pre;
				q=pr;
				k=1;
			}
		}
	}
	LoopArray.Add(ploop);
	////自交检查结束
	////开始删除多余的反向环
	int ldir,ndir;
	if(exterior)ndir=-1;
	else ndir=1;
#if DELETEINVALID

	for(i=0;i<LoopArray.GetSize();i++){
		LoopGetExt(LoopArray[i]);
		ldir=LoopDirection(LoopArray[i]);
		if(ldir==ndir){
			ReleaseLoop(LoopArray[i]);
			LoopArray.RemoveAt(i);
			i--;
		}
	}
#endif
	if(LoopArray.GetSize()>0){
		for(i=0;i<LoopArray.GetSize()-1;i++)LoopArray[i]->next=LoopArray[i+1];
		LoopArray[i]->next=NULL;
		ploop=LoopArray[0];
		if(next)LoopSfxCheck(next,exterior);
		LoopArray[i]->next=next;
		if(inner)LoopSfxCheck(inner,!exterior);
		ploop->inner=inner;
	}else ploop=NULL;

}
void LoopArrangeDirection(ELoop*ploop,BOOL exterior){
	//////判断环的绕向
	int loopdir=LoopDirection(ploop);
	///////使外环逆时针，内外顺时针
	if((loopdir==-1&&exterior)||(loopdir==1&&!exterior))ReverseLoop(ploop);
	ReOrganizeEnts(ploop->loop);
	if(ploop->next)LoopArrangeDirection(ploop->next,exterior);
	if(ploop->inner)LoopArrangeDirection(ploop->inner,!exterior);

}
ELoop*LoopTreeCopyNoEnt(ELoop*pLoopTree){
	if(pLoopTree==NULL)return NULL;
	ELoop*pLoopTreeCopy=new ELoop;
	pLoopTreeCopy->loop=pLoopTree->loop;
	pLoopTreeCopy->xmax=pLoopTree->xmax;
	pLoopTreeCopy->ymax=pLoopTree->ymax;
	pLoopTreeCopy->xmin=pLoopTree->xmin;
	pLoopTreeCopy->ymin=pLoopTree->ymin;
	pLoopTreeCopy->next=LoopTreeCopyNoEnt(pLoopTree->next);
	pLoopTreeCopy->inner=LoopTreeCopyNoEnt(pLoopTree->inner);
	return pLoopTreeCopy;
}
ELoop* LoopTreeCopy(ELoop*pLoopTree,float scale){
	if(pLoopTree==NULL)return NULL;
	EntFull*pEnt=pLoopTree->loop,*p=new EntFull,*pre=NULL;
	ELoop*pLoopTreeCopy=new ELoop;
	pLoopTreeCopy->loop=p;
	pLoopTreeCopy->xmax=pLoopTree->xmax*scale;
	pLoopTreeCopy->ymax=pLoopTree->ymax*scale;
	pLoopTreeCopy->xmin=pLoopTree->xmin*scale;
	pLoopTreeCopy->ymin=pLoopTree->ymin*scale;
	do{
		p->flag=pEnt->flag;
		p->xstart=pEnt->xstart*scale;p->ystart=pEnt->ystart*scale;
		p->xend=pEnt->xend*scale;p->yend=pEnt->yend*scale;
		p->xmin=pEnt->xmin*scale;p->ymin=pEnt->ymin*scale;
		p->xmax=pEnt->xmax*scale;p->ymax=pEnt->ymax*scale;
		p->gin=pEnt->gin;p->gout=pEnt->gout;
		if(p->flag&ENT_ISLINE){
			p->a=pEnt->a;p->b=pEnt->b;p->c=pEnt->c*scale;
		}else{
			p->xc=pEnt->xc*scale;p->yc=pEnt->yc*scale;p->r=pEnt->r*scale;
			p->angle=pEnt->angle;p->gstart=pEnt->gstart;p->gend=pEnt->gend;
		}
		p->pre=pre;pre=p;p->next=new EntFull;
		p=p->next;
		pEnt=pEnt->next;
	}while(pEnt!=pLoopTree->loop);
	delete p;pre->next=pLoopTreeCopy->loop;
	pLoopTreeCopy->loop->pre=pre;
	pLoopTreeCopy->next=LoopTreeCopy(pLoopTree->next,scale);
	pLoopTreeCopy->inner=LoopTreeCopy(pLoopTree->inner,scale);
	return pLoopTreeCopy;
}
void LayerGetInfo(ELayer*pLayer,ELayerInfo*pELI){
	pELI->iLoopCount=0;
	pELI->iNcCount=0;
	if(pLayer==NULL||pLayer->pLoopOriginalTree==NULL)return;
	pELI->iLoopCount=0;
	ELoop*ploop=pLayer->pLoopOriginalTree,*plp;
	pELI->xmin=ploop->xmin;pELI->xmax=ploop->xmax;
	pELI->ymin=ploop->ymin;pELI->ymax=ploop->ymax;
	while(ploop){
		pELI->iLoopCount++;
		plp=ploop->inner;
		while(plp){
			pELI->iLoopCount++;
			plp=plp->next;
		}
		ploop=ploop->next;
		if(ploop){
			if(pELI->xmin>ploop->xmin)pELI->xmin=ploop->xmin;
			if(pELI->xmax<ploop->xmax)pELI->xmax=ploop->xmax;
			if(pELI->ymin>ploop->ymin)pELI->ymin=ploop->ymin;
			if(pELI->ymax<ploop->ymax)pELI->ymax=ploop->ymax;
		}
	}
	if(pLayer->iNcEntNum>0){
		pELI->iNcCount=pLayer->iNcEntNum;
		for(int i=0;i<pLayer->iNcEntNum;i++)
			if(pLayer->pNcEnt[i].type==NCARC_CENTER)
				pELI->iNcCount--;
	}

}
void LoopTreeGetExt(ELoop*ploop){
	if(ploop==NULL)return;
	LoopGetExt(ploop);
	if(ploop->next)LoopTreeGetExt(ploop->next);
	if(ploop->inner)LoopTreeGetExt(ploop->inner);
}
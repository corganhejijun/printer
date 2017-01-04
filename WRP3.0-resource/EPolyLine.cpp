// EPolyLine.cpp: implementation of the EPolyLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RP.h"
#include "EPolyLine.h"
#include "types2.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////extern

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void LoopGetExt(ELoop*ploop);
EPolyLine::EPolyLine(EntFull*pEnt)
{
	pRear=pLoop=pEnt;
	xstart=pEnt->xstart;ystart=pEnt->ystart;
	xend=pEnt->xend;yend=pEnt->yend;
}

EPolyLine::~EPolyLine()
{
	EntFull*p;
	while(pLoop){
		p=pLoop;
		pLoop=pLoop->next;
		delete p;
	}
		
}
////////////////合并两条多义线
int EPolyLine::Append(EPolyLine *pEpl)
{
	EntFull*p;float x,y;
	if(PTEQU(xstart,pEpl->xstart,ystart,pEpl->ystart)
		||PTEQU(xend,pEpl->xend,yend,pEpl->yend))
	pEpl->Reverse();
	if(PTEQU(xend,pEpl->xstart,yend,pEpl->ystart)){
		pRear->next=pEpl->pLoop;
		pEpl->pLoop->pre=pRear;
		p=pRear;pRear=pEpl->pRear;
		xend=pEpl->xend;yend=pEpl->yend;
		if(AppParam.bCheckSelfXOnLoopBuild){
			while(p!=pLoop){
				if(p->flag&ENT_ISFLIP){x=p->xend;y=p->yend;}
				else {x=p->xstart;y=p->ystart;}
				if(PTEQU(xend,x,yend,y)){
					pEpl->pLoop=p;pRear=p->pre;
					p->pre=NULL;pRear->next=NULL;
					pEpl->xstart=x;pEpl->ystart=y;
					return 3;
				}
				p=p->pre;
			}
		}
	}else if(PTEQU(xstart,pEpl->xend,ystart,pEpl->yend)){
		pEpl->pRear->next=pLoop;
		pLoop->pre=pEpl->pRear;
		p=pLoop;pLoop=pEpl->pLoop;
		xstart=pEpl->xstart;ystart=pEpl->ystart;
		if(AppParam.bCheckSelfXOnLoopBuild){
			while(p!=pRear){
				if(p->flag&ENT_ISFLIP){x=p->xstart;y=p->ystart;}
				else {x=p->xend;y=p->yend;}
				if(PTEQU(xstart,x,ystart,y)){
					pEpl->pRear=p;pLoop=p->next;
					p->next=NULL;pLoop->pre=NULL;
					pEpl->xend=x;
					pEpl->yend=y;
					return 3;
				}
				p=p->next;
			}
		}
	}else return 0;
	pEpl->pLoop=NULL;
	delete pEpl;
	if(PTEQU(xstart,xend,ystart,yend))return 2;
	else return 1;
}
///将一条多义线首尾倒置
void EPolyLine::Reverse()
{
	EntFull *p=pLoop,*t;float v;
	pLoop=pRear;pRear=p;
	while(p){
		p->flag^=ENT_ISFLIP;
		t=p->next;
		p->next=p->pre;
		p->pre=t;p=t;}
	v=xstart;xstart=xend;xend=v;
	v=ystart;ystart=yend;yend=v;
}

ELoop*EPolyLine::EGetLoop()
{
	ELoop*pEL=new ELoop;
	pLoop->pre=pRear;
	pRear->next=pLoop;
	pEL->loop=pLoop;
	pEL->inner=pEL->next=NULL;
	LoopGetExt(pEL);
	pLoop=NULL;
	return pEL;
}

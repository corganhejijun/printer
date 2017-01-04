#include "stdafx.h"
#include "types.h"
#include "ent.h"
#include "math.h"
#include <afxtempl.h>
static double ax,bx;
static int xpCompare(const void*xp1,const void*xp2){
	double dx=ax*(((EXPoint*)xp1)->x-((EXPoint*)xp2)->x)
		+bx*(((EXPoint*)xp1)->y-((EXPoint*)xp2)->y);
	if(ZERO(dx))return 0;
	if(dx<0)return -1;
	return 1;
}
static int EntEndOnLineStatus(double a,double b,double c,double angle,EntFull*p){
//		ASSERT(ZERO(a*p->xend+b*p->yend+c));
		if(p->flag&ENT_ISLINE){
			double xp=a*p->xstart+b*p->ystart+c;
			if(ZERO(xp/10))return 0;
			else if(xp>0)return 1;
			else return -1;
		}else{
			float gout=p->gout;
			CenterAngle(gout,(float)angle);
			if(AEQU(angle,gout)||AEQU(angle,gout+PI)){
				double xp=a*p->xc+b*p->yc+c;
				ASSERT(!ZERO(xp));
				if(xp>0)return 1;
				else return -1;				
			}else{
				if(gout<angle)return 1;
				else return -1;
			}

		}
}
static int EntStartOnLineStatus(double a,double b,double c,double angle,EntFull*p){
//		ASSERT(ZERO(a*p->xstart+b*p->ystart+c));
		if(p->flag&ENT_ISLINE){
			double xp=a*p->xend+b*p->yend+c;
			if(ZERO(xp/10))return 0;
			else if(xp>0)return 1;
			else return -1;
		}else{
			float gin=p->gin;
			CenterAngle(gin,(float)angle);
			if(AEQU(angle,gin)||AEQU(angle,gin+PI)){
				double xp=a*p->xc+b*p->yc+c;
				ASSERT(!ZERO(xp));
				if(xp>0)return 1;
				else return -1;				
			}else{
				if(gin>angle)return 1;
				else return -1;
			}			
		}
}
static void LoopContourToNC(ELoop*ploop,float r,CArray<NcEnt,NcEnt>&ncar){
	ASSERT(ploop);
	ASSERT(ploop->loop);
	EntFull*pEnt=ploop->loop;
	NcEnt e;
	e.on_contour=FALSE;e.ms=FALSE;e.iscircle=0;e.reserved=0;
	e.type=NCG00;
	if(pEnt->flag&ENT_ISCIRCLE){
///以最右点为起点的方式， 两条指令
		e.x=pEnt->xc+pEnt->r;e.y=pEnt->yc;ncar.Add(e);///////
		e.ms=TRUE;e.on_contour=TRUE;e.iscircle=FALSE;

		e.type=pEnt->angle>0?NCARC_CCW:NCARC_CW;
		e.x=pEnt->xc-pEnt->r;e.y=pEnt->yc;///////
		ncar.Add(e);
		e.type=NCARC_CENTER;
		e.x=pEnt->xc;e.y=pEnt->yc;
		ncar.Add(e);

		e.type=pEnt->angle>0?NCARC_CCW:NCARC_CW;		
		e.x=pEnt->xc+pEnt->r;e.y=pEnt->yc;///////
		ncar.Add(e);
		e.type=NCARC_CENTER;
		e.x=pEnt->xc;e.y=pEnt->yc;
		ncar.Add(e);
///以最右点为起点的方式， 一条指令
/*		e.x=pEnt->xc+pEnt->r;e.y=pEnt->yc;ncar.Add(e);///////
		e.ms=TRUE;e.on_contour=TRUE;e.iscircle=TRUE;
		if(pEnt->angle>0)e.type=NCARC_CCW;
		else e.type=NCARC_CW;
		e.x=pEnt->xc+pEnt->r;e.y=pEnt->yc;///////
		ncar.Add(e);
		e.type=NCARC_CENTER;
		e.x=pEnt->xc;e.y=pEnt->yc;
		ncar.Add(e);*/
	/////以最底点为起点的方式，一条指令
/*		e.x=pEnt->xc;e.y=pEnt->yc-pEnt->r;ncar.Add(e);///////
		e.ms=TRUE;e.on_contour=TRUE;e.iscircle=TRUE;
		if(pEnt->angle>0)e.type=NCARC_CCW;
		else e.type=NCARC_CW;
		ncar.Add(e);
		e.type=NCARC_CENTER;
		e.x=pEnt->xc;e.y=pEnt->yc;
		ncar.Add(e);*/
	}else{
		e.x=pEnt->xstart;e.y=pEnt->ystart;
		ncar.Add(e);e.ms=TRUE;e.on_contour=TRUE;
		do{
			e.x=pEnt->xend;e.y=pEnt->yend;
			if(pEnt->flag&ENT_ISLINE){
				e.type=NCLINE;
				ncar.Add(e);
			}else{
				if(pEnt->angle>0)e.type=NCARC_CCW;
				else e.type=NCARC_CW;
				ncar.Add(e);
				e.type=NCARC_CENTER;
				e.x=pEnt->xc;e.y=pEnt->yc;
				ncar.Add(e);
			}
			pEnt=pEnt->next;
		}while(pEnt!=ploop->loop);
	}
	ploop=ploop->inner;
	while(ploop){
		LoopContourToNC(ploop,r,ncar);
		ploop=ploop->next;
	}
}
static void NcXLineLoop(double a,double b,double c,double angle,ELoop*ploop,CArray<EXPoint,EXPoint>*pxps){
	EntFull*pEnt=ploop->loop;
	EntFull*pHead=NULL,*pn=pEnt;
	int p,p1,p2;EXPoint xp1,xp2;
	do{
		if((pEnt->flag&ENT_ISLINE)&&((p=XLineLine(a,b,c,pEnt,xp1.x,xp1.y))==PTUNKNOW)){
			pEnt=pEnt->pre;
			continue;
		}
		pHead=pEnt;
		break;
	}while(pEnt!=ploop->loop);
	if(pHead==NULL)return;
	if(pEnt->flag&ENT_ISCIRCLE){
		p=XLineCircle(pEnt,a,b,c,xp1.x,xp1.y,xp2.x,xp2.y);
		if(p==2){pxps->Add(xp1);pxps->Add(xp2);}
	}else{
		do{
				if(pEnt->flag&ENT_ISLINE){
					p=XLineLine(a,b,c,pEnt,xp1.x ,xp1.y);
					if(p==PTINCURVE||p==PTONEND){
						pxps->Add(xp1);pEnt=pEnt->next;}
					else if(p==PTUNKNOW){
						pn=pEnt->next;p=-1;
						while((pn->flag&ENT_ISLINE)&&
							((p=XLineLine(a,b,c,pn,xp2.x,xp2.y))==PTUNKNOW)){
							pn=pn->next;
						}
						ASSERT(ZERO(a*pn->xstart+b*pn->ystart+c));
						p1=EntEndOnLineStatus(a,b,c,angle,pEnt->pre);
						p2=EntStartOnLineStatus(a,b,c,angle,pn);
						ASSERT(p1*p2!=0);
						if(p1*p2<0)pxps->Add(xp1);
						pEnt=pn;
					}else if(p==PTONSTART){
						p1=EntEndOnLineStatus(a,b,c,angle,pEnt->pre);
						p2=EntStartOnLineStatus(a,b,c,angle,pEnt);
						if(p1*p2>=0)pxps->Add(xp1);
						pEnt=pEnt->next;					
					}else pEnt=pEnt->next;
				}else{
					float ag1,ag2;
					p=XLineArc(a,b,c,pEnt,xp1.x,xp1.y,ag1,xp2.x,xp2.y,ag2,TRUE);
					p1=p&0xffff;
					if(p1){
						if((p1&PTONEXTRIM)&&(p1&PTINCURVE)){}
						else if((p1&0xff)==PTONSTART){
							p1=EntEndOnLineStatus(a,b,c,angle,pEnt->pre);
							p2=EntStartOnLineStatus(a,b,c,angle,pEnt);
							if(p1*p2>=0)pxps->Add(xp1);							
						}else	pxps->Add(xp1);
					}
					p2=p>>16;
					if(p2){
						if(p2==PTONSTART){
							p1=EntEndOnLineStatus(a,b,c,angle,pEnt->pre);
							p2=EntStartOnLineStatus(a,b,c,angle,pEnt);
							if(p1*p2>=0)pxps->Add(xp2);						
						}else pxps->Add(xp2);
					}
					pEnt=pEnt->next;
				}		
		}while(pEnt!=pHead);
	}
	ploop=ploop->inner;
	while(ploop){
		NcXLineLoop(a,b,c,angle,ploop,pxps);
		ploop=ploop->next;
	}
}
static void  LoopToParellelNC(ELoop*ploop,double angle,float d,float lenmin,CArray<NcEnt,NcEnt>&ncar){
	double cmin,cmax,c;
	double xs,ys,xe,ye;
	double a,b;
	BOOL forward=TRUE;
	int i,size;NcEnt e;e.iscircle=0;e.reserved=0;
	CArray<EXPoint,EXPoint>xps;
	a=-sin(angle);b=cos(angle);
	cmin=cmax=-a*ploop->xmin-b*ploop->ymin;
	c=-a*ploop->xmin-b*ploop->ymax;
	if(c<cmin)cmin=c;else cmax=c;
	c=-a*ploop->xmax-b*ploop->ymin;
	if(c<cmin)cmin=c;else if(c>cmax)cmax=c;		
	c=-a*ploop->xmax-b*ploop->ymax;
	if(c<cmin)cmin=c;else if(c>cmax)cmax=c;	
	cmax-=d/2;c=cmax;
	while(c>=cmin){
		NcXLineLoop(a,b,c,angle,ploop,&xps);
		if(forward){ax=b;bx=-a;}
		else{ax=-b;bx=a;}
		size=xps.GetSize();
		if(size>1){
			qsort(xps.GetData(),size,sizeof(EXPoint),xpCompare);
			for(i=0;i<size-1;i++){
				if(fabs(ax*(xps[i].x-xps[i+1].x)+bx*(xps[i].y-xps[i+1].y))<lenmin)
//					if(fabs(ax*(xps[i].x-xps[i+1].x)+bx*(xps[i].y-xps[i+1].y))<2*d)
				{
					xps.RemoveAt(i,2);
					size-=2;
					i--;
				}
			}
		}
		if(size>1){
			ASSERT((size&0x01)==0);
			if(forward){
				xs=xps[0].x-b*d;ys=xps[0].y+a*d;
				xe=xps[size-1].x+b*d;ye=xps[size-1].y-a*d;
			}else {
				xs=xps[0].x+b*d;ys=xps[0].y-a*d;
				xe=xps[size-1].x-b*d;ye=xps[size-1].y+a*d;
			}
			e.on_contour=FALSE;e.ms=FALSE;
			e.type=NCG00;e.x=(float)xs;e.y=(float)ys;ncar.Add(e);
			for(i=0;i<size;i++){
					if( e.ms)e.type=NCLINE;
					else e.type=NCG00;
					e.x=xps[i].x;e.y=xps[i].y;
					ncar.Add(e);
					e.ms=!e.ms;
			}
			e.type=NCG00;
			e.ms=0;e.x=(float)xe;e.y=(float)ye;ncar.Add(e);
		}
		c-=d;
		forward=!forward;
		xps.RemoveAll();
	}
}


void BuildLayerNC(ELayer*pLayer,double angle,float d,float lenmin,float r,
				  int cncOrder,int linecross){
	if(pLayer->pLoopOffsetBTree==NULL||pLayer->pLoopOffsetBTree->loop==NULL){
		pLayer->iNcEntNum=0;pLayer->pNcEnt=NULL;	
		return;
	}
	ASSERT(angle>=0&&angle<DPI);
	CArray<NcEnt,NcEnt>ncar;
	ELoop*ploop=pLayer->pLoopOffsetBTree;
	do{
		if((cncOrder&0x3)==2)
		{
			LoopToParellelNC(ploop,angle,d,lenmin,ncar);
			if(linecross)
                LoopToParellelNC(ploop,angle+HALFDPI,d,lenmin,ncar);
		}
		else if((cncOrder&0x03)==1)
            LoopContourToNC(ploop,r,ncar);
		if((cncOrder&0x0c)==0x08)
		{
			LoopToParellelNC(ploop,angle,d,lenmin,ncar);
			if(linecross)
                LoopToParellelNC(ploop,angle+HALFDPI,d,lenmin,ncar);
		}
		else if((cncOrder&0x0c)==0x04)
            LoopContourToNC(ploop,r,ncar);
		ploop=ploop->next;
	}while(ploop);
	pLayer->iNcEntNum=ncar.GetSize();
	if(pLayer->iNcEntNum>0){
		pLayer->pNcEnt=new NcEnt[pLayer->iNcEntNum];
		memcpy(pLayer->pNcEnt,ncar.GetData(),pLayer->iNcEntNum*sizeof(NcEnt));
	}
}
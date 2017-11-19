#include "stdafx.h"
#include "types.h"
#include "ent.h"
#include "math.h"
int LoopDirection(ELoop*ploop);
void ReverseLoop(ELoop*ploop);
void LoopGetExt(ELoop*ploop);

static void AngleFix(float&ag1,float &ag2){
	while(ag2<0)ag2+=TWOPI;
	while(ag2>=TWOPI)ag2-=TWOPI;
	while(ag2<ag1)ag1-=TWOPI;
	while(ag2-ag1>=TWOPI)ag1+=TWOPI;
}

static void Ent_Offset_LeftHand(EntFull*pEnt,float d){
	ASSERT(!(pEnt->flag&ENT_ISFLIP));
	EntFull*p=new EntFull;
	EntCopy(p,pEnt);
	if(p->flag&ENT_ISLINE){
		float dx,dy;
		dx=float(-d*sin(p->gout));dy=float(d*cos(p->gout));
		p->xstart+=dx;		p->xend+=dx;
		p->ystart+=dy;		p->yend+=dy;
		ParamMakeLine(p);
	}else if(p->flag&ENT_ISCIRCLE){
		if(pEnt->angle>0&&pEnt->r<=d){
			delete p;pEnt->poffset=NULL;return;
		}else{
			if(p->angle>0)p->r=p->r-d;
			else p->r=p->r+d;
			p->xend=p->xstart=p->xc+p->r;
		}
		
	}else{
		if(pEnt->angle>0&&pEnt->r<=d){
				float ag;
				float xm,ym,dd;
				delete p;
				if(pEnt->r<d){
					xm=(pEnt->xstart +pEnt->xend )/2;ym=(pEnt->ystart +pEnt->yend )/2;
					dd=SQRT(d*d-SQUARE(pEnt->xstart -xm)-SQUARE(pEnt->ystart -ym));
					ag=pEnt->gstart+pEnt->angle/2;
					pEnt->xc=float(xm-dd*cos(ag));
					pEnt->yc=float(ym-dd*sin(ag));
					pEnt->gstart=float(atan2(pEnt->ystart -pEnt->yc,pEnt->xstart -pEnt->xc));
					pEnt->gend=float(atan2(pEnt->yend -pEnt->yc,pEnt->xend -pEnt->xc));
					pEnt->angle=pEnt->gend-pEnt->gstart;
					ASSERT(pEnt->angle>-2*TWOPI);
					ASSERT(pEnt->angle<2*TWOPI);
					CenterAngle(pEnt->angle,PI);
					pEnt->gend=pEnt->gstart+pEnt->angle;
					pEnt->r=d;
					ParamMakeDir(pEnt);
				}
				pEnt->poffset=NULL;
				return ;
		}else{
			if(p->angle>0)p->r=p->r-d;
			else p->r=p->r+d;
			p->xstart=float(p->xc+p->r*cos(p->gstart));
			p->ystart=float(p->yc+p->r*sin(p->gstart));
			p->xend=float(p->xc+p->r*cos(p->gend));
			p->yend=float(p->yc+p->r*sin(p->gend));
		}
	}
	p->pre=p->next=NULL;
	p->flag&=ENT_EXT_INVALID;
	pEnt->poffset=p;
	p->poffset=pEnt;
	ParamMakeExt(p);
}
static void  Loop_Remove_Ent(ELoop*ploop,EntFull*p){
	if(ploop->loop==p)ploop->loop=p->pre;
	p->pre->next=p->next;
	p->next->pre=p->pre;
	if(p->poffset)delete p->poffset;
	delete  p;
}
static void FitAngle(float &ag,EntFull*p){
	ASSERT(!(p->flag&ENT_ISLINE));
	if(p->angle>0){
		while(ag<p->gstart)ag+=TWOPI;
		while(ag>p->gend)ag-=TWOPI;
	}else{
		while(ag>p->gstart)ag-=TWOPI;
		while(ag<p->gend)ag+=TWOPI;
	}
	ASSERT((ag-p->gstart)*(ag-p->gend)<=0);
}
static void EntTrimOffsetStart(EntFull*p,float x,float y,float& ag,float d,float&x2,float &y2){
	if(p->flag&ENT_ISLINE){
		ag=p->gout-HALFPI;
		x2=(float)(x+d*cos(ag));y2=(float)(y+d*sin(ag));
	}else{
		if(p->angle>0){
			x2=x+(x-p->xc)*d/(p->r-d);y2=y+(y-p->yc)*d/(p->r-d);
			p->gstart=ag;
		}else if(p->angle<0){
			x2=x-(x-p->xc)*d/(d+p->r);y2=y-(y-p->yc)*d/(d+p->r);
			p->gstart=ag;
			ag+=PI;
		}
		p->angle=p->gend-p->gstart;
		p->poffset->gstart=p->gstart;
		p->poffset->angle=p->angle;
		ParamMakeDir(p);
		p->poffset->gin=p->gin;p->poffset->gout=p->gout;
	}
	p->poffset->xstart=x;p->poffset->ystart=y;
	p->xstart=x2;p->ystart=y2;
	ParamMakeExt(p);
	ParamMakeExt(p->poffset);
}
static void EntTrimOffsetEnd(EntFull*p,float x,float y,float& ag,float d,float&x1,float& y1){
	if(p->flag&ENT_ISLINE){
		ag=p->gout-HALFPI;
		x1=(float)(x+d*cos(ag));y1=(float)(y+d*sin(ag));
	}else{
		if(p->angle>0){
			x1=x+(x-p->xc)*d/(p->r-d);y1=y+(y-p->yc)*d/(p->r-d);
			p->gend=ag;
		}else if(p->angle<0){
			x1=x-(x-p->xc)*d/(d+p->r);y1=y-(y-p->yc)*d/(d+p->r);
			p->gend=ag;ag+=PI;
		}
		p->angle=p->gend-p->gstart;
		p->poffset->gend=p->gend;
		p->poffset->angle=p->angle;
		ParamMakeDir(p);	
		p->poffset->gin=p->gin;p->poffset->gout=p->gout;
	}
	p->poffset->xend=x;p->poffset->yend=y;	
	p->xend=x1;p->yend=y1;
	ParamMakeExt(p);ParamMakeExt(p->poffset);
}

static BOOL Link_Point_To_Point(ELoop*ploop,EntFull*&p,float d,int& count){
	EntFull*pins=new EntFull,*p2=p->next;
	float dir;
	float xm,ym,dd;
	////求解圆弧
	if(p->pre==p2)return FALSE;
	pins->flag=0;
	pins->xstart=p->xstart;pins->ystart=p->ystart;
	pins->xend=p2->xend;pins->yend=p2->yend;
	xm=(pins->xstart +pins->xend )/2;ym=(pins->ystart +pins->yend )/2;
	dd=d*d-SQUARE(pins->xstart -xm)-SQUARE(pins->ystart -ym);
	if(dd<0)return FALSE;
	dd=(float)sqrt(dd);
	dir=(float)atan2(pins->yend-pins->ystart,pins->xend-pins->xstart);
	pins->xc=float(xm-dd*sin(dir));
	pins->yc=float(ym+dd*cos(dir));
	pins->gstart=float(atan2(pins->ystart -pins->yc,pins->xstart -pins->xc));
	pins->gend=float(atan2(pins->yend -pins->yc,pins->xend -pins->xc));
	AngleFix(pins->gstart,pins->gend);
	pins->angle=pins->gend-pins->gstart;
	ASSERT(fabs(pins->angle)<PI);
	pins->gend=pins->gstart+pins->angle;
	pins->r=d;
	ParamMakeDir(pins);
	////////////偏移
	pins->poffset=NULL;
	/////勾链
	pins->pre=p;pins->next=p2;
	p->next=pins;p2->pre=pins;
	Loop_Remove_Ent(ploop,p);
	if(p2==ploop->loop&&count>2){ploop->loop=p2->next;count--;}
	Loop_Remove_Ent(ploop,p2);
	if(count>0)count--;
	p=pins->pre;
	return TRUE;
}
static BOOL Link_Point_Tangent_Ent(ELoop*ploop,EntFull*&p,float d,int& count,float da,BOOL er=FALSE){
	float x=p->xstart,y=p->ystart,x2,y2;
	float ag,ag2,gs,ge;int xs;
	EntFull*pn=p->next,*pa;
	if(!pn->poffset)return FALSE;
	if(p->pre==pn)return FALSE;
	ge=p->gin+HALFPI;
#if 1
	if(pn->flag&ENT_ISLINE){
		gs=pn->gout+HALFPI;
		AngleFix(ge,gs);
		if((pa=EntNewArc(x,y,gs,ge-gs,d))==NULL)return FALSE;
		xs=XArcLine(pa,pn->poffset,x,y,ag);
	}else{
		gs=(float)atan2(pn->yc-y,pn->xc-x);
		if(pn->angle<0)gs+=PI;
		AngleFix(ge,gs);
		if((pa=EntNewArc(x,y,gs,ge-gs,d))==NULL)return FALSE;
		xs=XArcArc(pa,pn->poffset,x,y,ag,ag2);
	}
#else
	if(pn->flag&ENT_ISLINE){
		gs=pn->gout+HALFPI;
		da+=p->gout-p->gin;
		if(da<ATOL||da>=TWOPI)
			return FALSE;
		if((pa=EntNewArc(x,y,gs,-da,d))==NULL)return FALSE;
		xs=XArcLine(pa,pn->poffset,x,y,ag);
	}else{
		gs=(float)atan2(pn->yc-y,pn->xc-x);
		if(pn->angle<0)gs+=PI;
		da+=p->gout-p->gin;
		da-=pn->angle;
		if(da<ATOL||da>=TWOPI)
			return FALSE;		
		if((pa=EntNewArc(x,y,gs,-da,d))==NULL)return FALSE;
		xs=XArcArc(pa,pn->poffset,x,y,ag,ag2);
	}
#endif
	if(xs==NOCROSS)return FALSE;
	ASSERT(xs&0xffff0000||xs&0x00ff);
	ag-=PI;delete pa;	
	EntTrimOffsetStart(pn,x,y,ag2,d,x2,y2);
	AngleFix(ag,ag2);
	pa=EntNewArc(x,y,p->xstart,p->ystart,x2,y2,ag,ag2,d);
	if(xs&0x00ff==PTONEND){
		if(pn==ploop->loop&&count>2){ploop->loop=pn->next;count--;}
		Loop_Remove_Ent(ploop,pn);
		if(count<0)count++;
	}
	pa->pre=p->pre;pa->next=p;
	pa->pre->next=pa;p->pre=pa;
	Loop_Remove_Ent(ploop,p);
	if(count!=0)count--;
	p=pa->pre;
	return TRUE;
}
static BOOL Link_Ent_Tangent_Point(ELoop*ploop,EntFull*&p,float d,int& count,float da,BOOL er=FALSE){

	float ag,ag2,x1,y1;int xs;
	EntFull*pn=p->next,*pa;
	float x=pn->xend,y=pn->yend;
	if(!p->poffset)return FALSE;
	if(p->pre==pn)return FALSE;
	float ge,gs=pn->gout+HALFPI;
#if 1
	if(p->flag&ENT_ISLINE){
		ge=p->gout+HALFPI;
		AngleFix(ge,gs);
		if((pa=EntNewArc(x,y,gs,ge-gs,d))==NULL)return FALSE;
		xs=XLineArc(p->poffset,pa,x,y,ag2);
	}else{
		ge=(float)atan2(p->yc-y,p->xc-x);
		if(p->angle<0)ge-=PI;
		AngleFix(ge,gs);
		if((pa=EntNewArc(x,y,gs,ge-gs,d))==NULL)return FALSE;
		xs=XArcArc(p->poffset,pa,x,y,ag,ag2);
	}
#else
	if(p->flag&ENT_ISLINE){
		da+=pn->gout-pn->gin;
		if(da<ATOL||da>PI)return FALSE;
		if((pa=EntNewArc(x,y,gs,-da,d))==NULL)return FALSE;
		xs=XLineArc(p->poffset,pa,x,y,ag2);
	}else{
		ge=(float)atan2(p->yc-y,p->xc-x);
		if(p->angle<0)ge-=PI;
		da+=pn->gout-pn->gin;
		if(da<ATOL||da>PI)return FALSE;
		if((pa=EntNewArc(x,y,gs,ge-gs,d))==NULL)return FALSE;
		xs=XArcArc(p->poffset,pa,x,y,ag,ag2);
	}
#endif
	if(xs==NOCROSS)return FALSE;
	ASSERT(xs&0xffff0000||xs&0x00ff);
	delete pa;ag2-=PI;
	EntTrimOffsetEnd(p,x,y,ag,d,x1,y1);
	AngleFix(ag,ag2);
	pa=EntNewArc(x,y,x1,y1,pn->xend,pn->yend,ag,ag2,d);
    if (pa == NULL)
        return FALSE;
	pa->pre=p;pa->next=pn;
	p->next=pa;pn->pre=pa;
	if((xs>>16)==PTONSTART){
		Loop_Remove_Ent(ploop,p);
		if(count>0)count--;
	}

	if(pn==ploop->loop&&count>2){ploop->loop=pn->next;count--;}
	Loop_Remove_Ent(ploop,pn);
	count++;
	p=pa;
	return TRUE;
}

static BOOL Link_Ent_Tangent_Ent(ELoop*ploop,EntFull*&p,float d,int& count,float da,BOOL er=FALSE){
	float x,y,ag1,ag2,x1,y1,x2,y2;
	int xs1,xs2,xs;
	EntFull*pn=p->next;
	if(!p->poffset||!pn->poffset)return FALSE;
	if(PTEQU(p->poffset->xend,p->next->poffset->xstart,
			p->poffset->yend,p->next->poffset->ystart)){
		p=p->next;
		count++;
		return TRUE;
	}
	if(!er&&(p->flag&ENT_ISLINE)&&(p->next->flag&ENT_ISLINE)&&da<0.2){
		x=float(p->xend-d*sin(p->gout+da/2)/cos(da/2));
		y=float(p->yend+d*cos(p->gout+da/2)/cos(da/2));
		xs1=EntPtOnLine(p->poffset,x,y);
		xs2=EntPtOnLine(p->next->poffset,x,y);
		if(!(xs1&&xs2))return FALSE;
		xs=(xs1<<16)+xs2;
	}else {
		xs=XEntEnt(p->poffset,p->next->poffset,x,y,ag1,ag2,FALSE);
		if(xs==NOCROSS)return FALSE;
		xs1=xs>>16;xs2=xs&0x000000ff;
	}
	EntTrimOffsetEnd(p,x,y,ag1,d,x1,y1);
	EntTrimOffsetStart(pn,x,y,ag2,d,x2,y2);
	AngleFix(ag1,ag2);
	EntFull *pins=EntNewArc(x,y,x1,y1,x2,y2,ag1,ag2,d);
	pins->pre=p;pins->next=pn;
	p->next=pins;pn->pre=pins;
	if(xs==STARTONEND){
		Loop_Remove_Ent(ploop,p);
		Loop_Remove_Ent(ploop,pn);
		if(count>0)count--;
		p=pins->pre;
	}else if(xs1==PTONSTART){
		Loop_Remove_Ent(ploop,p);
		if(count!=0)count--;
		p=pins->pre;
	}else if(xs2==PTONEND){
		if(pn==ploop->loop&&count>2){ploop->loop=pn->next;count--;}
		Loop_Remove_Ent(ploop,pn);
		count++;
		p=pins;
	}else{
		if(count>=0)count++;
		count++;
		p=pins->next;
	}
	return TRUE;
}
static BOOL Link_Point_To_Ent(ELoop*ploop,EntFull*&p,float d,int& count,float da){
	if(Link_Point_Tangent_Ent(ploop,p,d,count,da)
	||Link_Point_To_Point(ploop,p,d,count))return TRUE;
	else return FALSE;
}
static BOOL Link_Ent_To_Point(ELoop*ploop,EntFull*&p,float d,int& count,float da){
	if(Link_Ent_Tangent_Point(ploop,p,d,count,da)||
		Link_Point_To_Point(ploop,p,d,count))return TRUE;
	else return FALSE;
}

static BOOL Link_Ent_To_Ent(ELoop*ploop,EntFull*&p,float d,int& count,float da,BOOL er){
	if(	Link_Ent_Tangent_Ent(ploop,p,d,count,da,er)||
		Link_Point_Tangent_Ent(ploop,p,d,count,da,er)||
		Link_Ent_Tangent_Point(ploop,p,d,count,da,er)||
		Link_Point_To_Point(ploop,p,d,count))return TRUE;
	else return FALSE;
}

ELoop* BuildLayerLoopOffset(ELoop*&ploop,float d ,BOOL exterior){
	int ldir,count=0, ic=0;
	float da;BOOL LinkStatus;
	if(ploop==NULL)return NULL;
	EntFull*p;
	////////按顺序来偏移每个线条实体
	p=ploop->loop;
	do{
		Ent_Offset_LeftHand(p,d);
		p=p->next;
	}while(p!=ploop->loop);
	///////生成偏移后的环的二层树
	if(p->flag&ENT_ISCIRCLE){
		if(p->poffset==NULL)goto removethis;
		else{
			ELoop*plp=new ELoop;p=p->poffset;
			plp->xmax=p->xc+p->r;plp->xmin=p->xc-p->r;
			plp->ymin=p->yc-p->r;plp->ymax=p->yc+p->r;
			plp->loop=p;
			p->next=p->pre=p;
			if(ploop->next)plp->next=BuildLayerLoopOffset(ploop->next,d,exterior);
			else plp->next=NULL;
			if(ploop->inner)plp->inner=BuildLayerLoopOffset(ploop->inner,d,!exterior);
			else plp->inner=NULL;
			return plp;	
		}
	}
	/////检查角落
	do{
		LinkStatus=TRUE;
		da=EntDirChange(p,p->next,0);
		if(da<=ATOL){p=p->next;count++;continue;}
		if(p->poffset==NULL){
			if(p->next->poffset==NULL) LinkStatus=Link_Point_To_Point(ploop,p,d,count);
			else {
				float dap=EntDirChange(p->pre,p,0);
				if(dap>=-ATOL){
					/////////////////@#!$%@%#^%$#&^$#^&%$&^5
					EntFull*ppre=p->pre;
					da+=dap;da+=p->gout-p->gin;
					Loop_Remove_Ent(ploop,p);
					p=ppre;
					if(ppre->pre==ppre){LinkStatus=FALSE;break;}
					if(count>0)count--;
					LinkStatus=Link_Ent_To_Ent(ploop,p,d,count,da,TRUE);
				}else
					LinkStatus=Link_Point_To_Ent(ploop,p,d,count,da);
			}
		}else{
			if(p->next->poffset==NULL){
				float dan=EntDirChange(p->next,p->next->next,0);
				if(dan>=-ATOL){
					/////////////////@#!$%@%#^%$#&^$#^&%$&^5
					da+=dan;da+=p->next->gout-p->next->gin;
					Loop_Remove_Ent(ploop,p->next);
					if(p->pre==p){LinkStatus=FALSE;break;}
					if(count<0)count++;
					LinkStatus=Link_Ent_To_Ent(ploop,p,d,count,da,TRUE);
				}else{
					/////////////////@#!$%@%#^%$#&^$#^&%$&^5
					LinkStatus=Link_Ent_To_Point(ploop,p,d,count,da);
				}
			}else
				LinkStatus=Link_Ent_To_Ent(ploop,p,d,count,da,FALSE);

		}
		ic++;
//		if(ic==25)ASSERT(FALSE);
//		ASSERT(LinkStatus);
	}while(LinkStatus&&(p!=ploop->loop||count<=2));
	
	if(LinkStatus){
		LoopGetExt(ploop);
		ldir=LoopDirection(ploop);
	}
	if(LinkStatus&&((ldir==1&&exterior)||(ldir==-1&&!exterior))){
		ELoop*plp=new ELoop;EntFull*pf=NULL;
		plp->xmax=plp->xmin=plp->ymin=plp->ymax=0;
#define GENARATELOOP	1
#if GENARATELOOP
		p=ploop->loop;
		do{
			da=EntDirChange(p,p->next,0);
//			ASSERT(da<ATOL);
			if(p->poffset||da<=-ATOL){
				if(p->poffset){
					plp->loop=pf=p->poffset;
					if(da<=-ATOL){
						pf->next=EntNewArc(p->xend,p->yend,p->gout+HALFPI,da,d);
						pf->next->pre=pf;pf=pf->next;
					}
					p=p->next;
				}else{
					plp->loop=pf=EntNewArc(p->xend,p->yend,p->gout+HALFPI,da,d);
					p=p->next;
				}
				break;
			}
			p=p->next;
		}while(p!=ploop->loop);
		if(pf==NULL) goto removethis;
		do{
			da=EntDirChange(p,p->next,0);
//			ASSERT(da<ATOL);
			if(p->poffset){
				pf->next=p->poffset;
				pf->next->pre=pf;pf=pf->next;
				if(da<=-ATOL){
					pf->next=EntNewArc(p->xend,p->yend,p->gout+HALFPI,da,d);
					pf->next->pre=pf;pf=pf->next;
				}
			}else if(da<=-ATOL){
					pf->next=EntNewArc(p->xend,p->yend,p->gout+HALFPI,da,d);
					pf->next->pre=pf;pf=pf->next;
			}
			p=p->next;
		}while(p!=ploop->loop);
		pf->next=plp->loop;plp->loop->pre=pf;
#else
		plp->loop=NULL;
#endif
		if(ploop->next)plp->next=BuildLayerLoopOffset(ploop->next,d,exterior);
		else plp->next=NULL;
		if(ploop->inner)plp->inner=BuildLayerLoopOffset(ploop->inner,d,!exterior);
		else plp->inner=NULL;
		return plp;
	}
removethis:
	ELoop*plnext=ploop->next;
	ploop->next=NULL;
	ReleaseLoop(ploop);
	ploop=plnext;
	if(ploop)return BuildLayerLoopOffset(ploop,d,exterior);
	else return NULL;
	
}

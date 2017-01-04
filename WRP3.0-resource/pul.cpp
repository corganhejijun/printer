#include "stdafx.h"
#include <afxtempl.h>
#include "rp.h"
#include "types.h"
#include "math.h"
#include "funcs.h"
class PulFile{
	int ix,iy,iz;
	void ArcCCW1(float x,float y,float cx,float cy);
	void ArcCCW2(float x,float y,float cx,float cy);
	void ArcCCW3(float x,float y,float cx,float cy);
	void ArcCCW4(float x,float y,float cx,float cy);
	void ArcCW1(float x,float y,float cx,float cy);
	void ArcCW2(float x,float y,float cx,float cy);
	void ArcCW3(float x,float y,float cx,float cy);
	void ArcCW4(float x,float y,float cx,float cy);
	int  toint(float f){return f>0?(int(2*f+1))/2:-(int(1-2*f))/2;}
public:
	CString puls;
	PulFile(){ix=iy=iz=0;}
	~PulFile(){};
	void zmove(float z);
	void G00(float x,float y);
	void Line(float x,float y);
	void ArcCCW(float x,float y,float cx,float cy);
	void ArcCW(float x,float y,float cx,float cy);
	void gzon(){puls+="GO";};
	void gzoff(){puls+="GC";};
	void End(){puls+="ED";};
};
void PulFile::zmove(float z)
{
	int ze=toint(z);
	while(z>iz)
	{
		puls+="Z+";
		iz++;
	}
}
void PulFile::G00(float x,float y)
{
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	if(xe>ix)
	{	
		do{
			puls+="X+";
			ix++;
		}while(xe>ix);
	}else if(xe<ix)
	{	
		do{
			puls+="X-";
			ix--;
		}while(xe<ix);

	}
	if(ye>iy)
	{
		do{
			puls+="Y+";
			iy++;
		}while(ye>iy);
	}else if(y-iy<-0.5)
	{
		do{
			puls+="Y-";
			iy--;
		}while(ye<iy);
	}


}
void PulFile::Line(float x,float y)
{
	int dx,dy,e;
	int xe,ye;
	xe=toint(x);ye=int(y);
	dy=ye-iy;dx=xe-ix;
	e=dy*(xe-ix)-dx*(ye-iy);
	if(dx>0)
	{
		///X要向正向移动
		if(dy>0)
		{
			//Y向上.每一象限
			if(dy>=dx)
			{
					while(iy<ye)
					{
						puls+="Y+";
						iy++;
						e-=dx;
						if(e<0)
						{
							puls+="X+";
							ix++;
							e+=dy;
						}
					}
					ASSERT(ix==xe);
			}
			else
			{
					while(ix<xe){
						puls+="X+";
						ix++;
						e+=dy;
						if(e>0)
						{
							puls+="Y+";
							iy++;
							e-=dx;
						}

					}
					ASSERT(iy==ye);

			}

		}else if(dy<0)
		{

			//Y向下.//第四象限 
			//Y向上.每一象限
			if(-dy>=dx)
			{
					while(iy>ye)
					{
						puls+="Y-";
						iy--;
						e+=dx;
						if(e>0)
						{
							puls+="X+";
							ix++;
							e+=dy;
						}
					}
					ASSERT(ix==xe);
			}
			else
			{
					while(ix<xe){
						puls+="X+";
						ix++;
						e+=dy;
						if(e<0)
						{
							puls+="Y-";
							iy--;
							e+=dx;
						}

					}
					ASSERT(iy==ye);
			}

		}else
		{
			//Y不动
			while(xe>ix)
			{
				puls+="X+";
				ix++;
			}

		}

	}else if(dx<0)
	{
		///X向负向移动
		if(dy>0)
		{
			//Y向上.第二象限
			if(dy>=-dx)
			{
					while(iy<ye)
					{
						puls+="Y+";
						iy++;
						e-=dx;
						if(e>0)
						{
							puls+="X-";
							ix--;
							e-=dy;
						}
					}
					ASSERT(ix==xe);
			}
			else
			{
					while(ix>xe){
						puls+="X-";
						ix--;
						e-=dy;
						if(e<0)
						{
							puls+="Y+";
							iy++;
							e-=dx;
						}
					}
					ASSERT(iy==ye);

			}

		}else if(dy<0)
		{

			//Y向下.第三象限
			if(-dy>=-dx)
			{
					while(iy>ye)
					{
						puls+="Y-";
						iy--;
						e+=dx;
						if(e<0)
						{
							puls+="X-";
							ix--;
							e-=dy;
						}
					}
					ASSERT(ix==xe);
			}
			else
			{
					while(ix>xe){
						puls+="X-";
						ix--;
						e-=dy;
						if(e>0)
						{
							puls+="Y-";
							iy--;
							e+=dx;
						}
					}
					ASSERT(iy==ye);

			}

		}else
		{
			//Y不动
			while(xe<ix)
			{
				puls+="X-";
				ix--;
			}

		}



	}else
	{
		///X无需移动
		if(dy>0)
		{
			//Y向上
			while(ye>iy)
			{
				puls+="Y+";
				iy++;
			}

		}else if(dy<0)
		{

			//Y向下
			while(ye<iy)
			{
				puls+="Y-";
				iy--;
			}

		}


	}

}
void PulFile::ArcCCW(float x,float y,float cx,float cy)
{
	float xx,yy;
	float gstart,gend;
	int d=2;
	float g=-PI;
	float r=float(sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy)));
	gstart=(float)atan2(iy-cy,ix-cx);
	gend=(float)atan2(y-cy,x-cx);
	if(gend<=gstart)gend+=TWOPI;
	while(g<gstart){g+=HALFPI;d=(d+1)%4;}
	while(gend>gstart)
	{
		if(gend<g)
		{
			g=gend;
			xx=x;yy=y;
		}else
		{
			if(d==0){xx=cx+r;yy=cy;}
			else if(d==1){xx=cx;yy=cy+r;}
			else if(d==2){xx=cx-r;yy=cy;}
			else if(d==3){xx=cx;yy=cy-r;}
		}
		///gstart->g
		if(d==0)ArcCCW4(xx,yy,cx,cy);
		else if(d==1)ArcCCW1(xx,yy,cx,cy);
		else if(d==2)ArcCCW2(xx,yy,cx,cy);
		else if(d==3)ArcCCW3(xx,yy,cx,cy);
		
		if(gend==g)break;
		gstart=g;
		g+=HALFPI;
		d=(d+1)%4;
	}


}
void PulFile::ArcCCW1(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix>xe||iy<ye)
	{
		if(e>0)
		{
			ix--;
			e-=1+2*(ix-cx);
			puls+="X-";

		}else
		{
			e+=1+2*(iy-cy);
			iy++;
			puls+="Y+";
		}
	}
}
void PulFile::ArcCCW2(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix>xe||iy>ye)
	{
		if(e<=0)
		{
			ix--;
			e-=1+2*(ix-cx);
			puls+="X-";

		}else
		{
			iy--;
			e-=1+2*(iy-cy);
			puls+="Y-";
		}
	}
}
void PulFile::ArcCCW3(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix<xe||iy>ye)
	{
		if(e>0)
		{
			e+=1+2*(ix-cx);
			ix++;
			puls+="X+";

		}else
		{
			iy--;
			e-=1+2*(iy-cy);
			puls+="Y-";
		}
	}
}
void PulFile::ArcCCW4(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix<xe||iy<ye)
	{
		if(e<=0)
		{
			e+=1+2*(ix-cx);
			ix++;
			puls+="X+";

		}else
		{
			e+=1+2*(iy-cy);
			iy++;
			puls+="Y+";
		}
	}
}
void PulFile::ArcCW(float x,float y,float cx,float cy)
{

	float xx,yy;
	float gstart, gend;
	int d=0;
	float g=TWOPI;
	float r=float(sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy)));
	gstart=(float)atan2(iy-cy,ix-cx);
	gend=(float)atan2(y-cy,x-cx);
	if(gend>=gstart)gend-=TWOPI;
	while(g>gstart){g-=HALFPI;d=(d+3)%4;}
	while(gend<gstart)
	{
		if(gend>g)
		{
			g=gend;
			xx=x;yy=y;
		}else
		{
			if(d==0){xx=cx+r;yy=cy;}
			else if(d==1){xx=cx;yy=cy+r;}
			else if(d==2){xx=cx-r;yy=cy;}
			else if(d==3){xx=cx;yy=cy-r;}
		}
		///gstart->g
		if(d==0)ArcCW1(xx,yy,cx,cy);
		else if(d==1)ArcCW2(xx,yy,cx,cy);
		else if(d==2)ArcCW3(xx,yy,cx,cy);
		else if(d==3)ArcCW4(xx,yy,cx,cy);
		
		if(gend==g)break;
		gstart=g;
		g-=HALFPI;
		d=(d+3)%4;
	}


}
void PulFile::ArcCW1(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix<xe||iy>ye)
	{
		if(e<=0)
		{
			e+=1+2*(ix-cx);
			ix++;
			puls+="X+";

		}else
		{
			iy--;
			e-=1+2*(iy-cy);
			puls+="Y-";
		}
	}

}
void PulFile::ArcCW2(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix<xe||iy<ye)
	{
		if(e>0)
		{
			e+=1+2*(ix-cx);
			ix++;
			puls+="X+";

		}else
		{
			e+=1+2*(iy-cy);
			iy++;
			puls+="Y+";
		}
	}
}
void PulFile::ArcCW3(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix>xe||iy<ye)
	{
		if(e<=0)
		{
			ix--;
			e-=1+2*(ix-cx);
			puls+="X-";

		}else
		{
			e+=1+2*(iy-cy);
			iy++;
			puls+="Y+";
		}
	}
}
void PulFile::ArcCW4(float x,float y,float cx,float cy)
{
	float e;
	int xe,ye;
	xe=toint(x);
	ye=toint(y);
	e=(ix-cx)*(ix-cx)+(iy-cy)*(iy-cy)-(x-cx)*(x-cx)-(y-cy)*(y-cy);
	while(ix>xe||iy>ye)
	{
		if(e>0)
		{
			ix--;
			e-=1+2*(ix-cx);
			puls+="X-";

		}else
		{
			iy--;
			e-=1+2*(iy-cy);
			puls+="Y-";
		}
	}

}
void NcEntsToPul(ESpace*pEntSpace,CString&puls)
{

	int   ix=0,iy=0,iz=0;
	int i,j;float xc,yc;
	ELayer*pLayer=pEntSpace->pLayer;
	NcEnt *p;
	BOOL gzon=FALSE;
	PulFile pulfile;
	for(i=0;i<pEntSpace->LayerNum;i++,pLayer++)
	{
		p=pLayer->pNcEnt;
		///Z方向移动
		pulfile.zmove(pLayer->z);
		for(j=0;j<pLayer->iNcEntNum;j++,p++)
		{
			if(p->ms&&!gzon){pulfile.gzon();gzon=TRUE;}
			else if(!p->ms&&gzon){pulfile.gzoff();gzon=FALSE;}
			if(p->type==NCG00)
			{
				pulfile.G00(p->x,p->y);
			}else if(p->type==NCLINE)
			{
				pulfile.Line(p->x,p->y);

			}else if(p->type==NCARC_CCW)
			{
				ASSERT(!p->iscircle);
				xc=(p+1)->x;yc=(p+1)->y;
				pulfile.ArcCCW(p->x,p->y,xc,yc);
				p++;j++;
			}else if(p->type==NCARC_CW)
			{
				ASSERT(!p->iscircle);
				xc=(p+1)->x;yc=(p+1)->y;
				pulfile.ArcCW(p->x,p->y,xc,yc);
				p++;j++;

			}else
			{
				ASSERT(FALSE);
			}
		}
		if(gzon){pulfile.gzoff();gzon=FALSE;}
	}
	pulfile.End();
	puls=pulfile.puls;
}

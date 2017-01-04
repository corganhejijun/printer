#include "stdafx.h"
#include "gl\gl.h"
#include "gl\glu.h"
#include "types.h"
#include "math.h"
#include <afxtempl.h>
#include "glfuncs.h"
//////////////////////////
/////绘制线条
///MAX-16  MIN-128
#define DELTA_ANGLE_16 0.39269908169872415480783042290994//16
#define DELTA_ANGLE_32 0.19634954084936207740391521145497//32
#define DELTA_ANGLE_64 0.098174770424681038701957605727484//64
#define DELTA_ANGLE_128 0.049087385212340519350978802863742//128
#define DELTA_ANGLE_256 0.024543692606170259675489401431871//256
#define DELTA_ANGLE_MAX DELTA_ANGLE_16
#define DELTA_ANGLE_MIN	DELTA_ANGLE_128

static double glDeltaAngleForArcSplit(float r){
	////////////////////////////1
//	return DELTA_ANGLE_32;
	/////////////////////////////2
	if(r<5)return DELTA_ANGLE_32;
	else if(r>40)return DELTA_ANGLE_256;
	return DELTA_ANGLE_32*5/r;
	////////////////////////3
	if(r<5)return DELTA_ANGLE_16;
	else if(r>320)return DELTA_ANGLE_128;
	return DELTA_ANGLE_16/sqrt(r/5);	
	/////////////////////////4
//	if(r<5)return DELTA_ANGLE_16;
//	else if(r>2560) return DELTA_ANGLE_128;
//	return DELTA_ANGLE_16/pow(r/5,0.33333333333333333333333333);
}
void glDrawEntLine(EntFull *pEnt)
{
	if(!pEnt)return ;
//		glColor3f(rand()%256/255.f,rand()%256/255.f,rand()%256/255.f);
		if(pEnt->flag&ENT_ISLINE)
		{
				glBegin(GL_LINES);
					glVertex2f(pEnt->xstart,pEnt->ystart);	
					glVertex2f(pEnt->xend,pEnt->yend);
				glEnd();
		}
		else
		{
			float x,y;
/*da*/		double a,da=glDeltaAngleForArcSplit(pEnt->r);
			glBegin(GL_LINE_STRIP);
			glVertex2f(pEnt->xstart ,pEnt->ystart );
			if(pEnt->angle>0){
				for(a=pEnt->gstart+da;a<pEnt->gend;a+=da){
					x=float(pEnt->xc+pEnt->r*cos(a));
					y=float(pEnt->yc+pEnt->r*sin(a));
					glVertex2f(x,y);
				}		
			}else{
				for(a=pEnt->gstart-da;a>pEnt->gend;a-=da){
					x=float(pEnt->xc+pEnt->r*cos(a));
					y=float(pEnt->yc+pEnt->r*sin(a));
					glVertex2f(x,y);
				}	
			}
			glVertex2f(pEnt->xend,pEnt->yend);
			glEnd();

		}

}
void glDrawEntLine(EntRaw *pEnt,float z)
{
	if(!pEnt)return ;
		if(pEnt->bulge==0)
		{
			glBegin(GL_LINES);
				glVertex3f(pEnt->xstart,pEnt->ystart,z);	
				glVertex3f(pEnt->xend,pEnt->yend,z);
			glEnd();
		}
		else
		{
			float x,y,gend=pEnt->sa+pEnt->angle; 
/*da*/		double a,da=glDeltaAngleForArcSplit(pEnt->r);
			glBegin(GL_LINE_STRIP);
			if(pEnt->angle>0){
				for(a=pEnt->sa;a<gend;a+=da){
					x=float(pEnt->xc+pEnt->r*cos(a));
					y=float(pEnt->yc+pEnt->r*sin(a));
					glVertex3f(x,y,z);
				}
			}else{
				for(a=pEnt->sa;a>gend;a-=da){
					x=float(pEnt->xc+pEnt->r*cos(a));
					y=float(pEnt->yc+pEnt->r*sin(a));
					glVertex3f(x,y,z);
				}
			}
			x=float(pEnt->xc+pEnt->r*cos(gend));
			y=float(pEnt->yc+pEnt->r*sin(gend));
			glVertex3f(x,y,z);
			glEnd();

		}
}

//////////////绘制半圆
static void glDrawPartDisk(float xc,float yc,float d,double sa,float z,double ag){
	float x,y;double a,ea=sa+ag;
	VERIFY(ag>0);
	glBegin(GL_POLYGON);
		for(a=sa;a<ea;a+=DELTA_ANGLE_MAX){
			x=float(xc+d*cos(a));
			y=float(yc+d*sin(a));
			glVertex3f(x,y,z);
		}
		x=float(xc+d*cos(ea));
		y=float(yc+d*sin(ea));
		glVertex3f(x,y,z);
	glEnd();
}

////////////////画圆柱面

static void glDrawCylinder(float xc,float yc,float d,double sa,double ag,float z,float zh,BOOL exterior=TRUE){

	double a,da,ea=ag+sa,vcos,vsin;
	d=float(fabs(d));
	da=glDeltaAngleForArcSplit(d);
//////////柱面
#define CYLINDER_STRIP 1
#if CYLINDER_STRIP
	float x,y;
	glBegin(GL_QUAD_STRIP);
	if(exterior){
		if(ag>0){
			for(a=sa;a<ea;a+=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(vcos,vsin,0);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
			}
		}else{
			for(a=sa;a>ea;a-=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(vcos,vsin,0);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
			}
		}
		vcos=cos(ea);vsin=sin(ea);
		x=float(xc+d*vcos);y=float(yc+d*vsin);
		glNormal3d(vcos,vsin,0);
		glVertex3f(x,y,zh);glVertex3f(x,y,z);
	}else{
		if(ag>0){
			for(a=sa;a<ea;a+=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(-vcos,-vsin,0);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
			}
		}else{
			for(a=sa;a>ea;a-=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(-vcos,-vsin,0);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
			}
		}	
		vcos=cos(ea);vsin=sin(ea);
		x=float(xc+d*vcos);y=float(yc+d*vsin);
		glNormal3d(-vcos,-vsin,0);
		glVertex3f(x,y,zh);glVertex3f(x,y,z);
	}
	glEnd();
#else
	float x,y;
	x=float(xc+d*cos(sa));y=float(yc+d*sin(sa));
	if(exterior){
		glBegin(GL_POLYGON);
			glVertex3f(x,y,zh);
			glVertex3f(x,y,z);glEdgeFlag(GL_FALSE);
		if(ag>0){
			for(a=sa+da;a<ea;a+=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(vcos,vsin,0);
					glVertex3f(x,y,z);
					glVertex3f(x,y,zh);
				glEnd();
				glBegin(GL_POLYGON);
					glVertex3f(x,y,zh);
					glVertex3f(x,y,z);					
			}
		}else{
			for(a=sa-da;a>ea;a-=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(vcos,vsin,0);
					glVertex3f(x,y,z);
					glVertex3f(x,y,zh);
				glEnd();
				glBegin(GL_POLYGON);
					glVertex3f(x,y,zh);
					glVertex3f(x,y,z);						
			}
		}
		vcos=cos(ea);vsin=sin(ea);
		x=float(xc+d*vcos);y=float(yc+d*vsin);
					glEdgeFlag(GL_TRUE);
					glVertex3f(x,y,z);
					glVertex3f(x,y,zh);
				glEnd();		
	}else{
		glBegin(GL_POLYGON);
			glVertex3f(x,y,z);glVertex3f(x,y,zh);
			glEdgeFlag(GL_FALSE);
		if(ag>0){

			for(a=sa+da;a<ea;a+=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(-vcos,-vsin,0);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
					
				glEnd();
				glBegin(GL_POLYGON);
					glVertex3f(x,y,z);glVertex3f(x,y,zh);			
			}
		}else{
			for(a=sa-da;a>ea;a-=da){
					vcos=cos(a);vsin=sin(a);
					x=float(xc+d*vcos);y=float(yc+d*vsin);
					glNormal3d(-vcos,-vsin,0);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
					
				glEnd();
				glBegin(GL_POLYGON);
					glVertex3f(x,y,z);glVertex3f(x,y,zh);					
			}
		}
		vcos=cos(ea);vsin=sin(ea);
		x=float(xc+d*vcos);y=float(yc+d*vsin);
					glEdgeFlag(GL_TRUE);
					glVertex3f(x,y,zh);glVertex3f(x,y,z);
				glEnd();
	}

#endif
//	glNormal3f(0.0f,0.0f,1.0f);
}
///////绘制线条的面
void glDrawEntFace(EntFull *pEnt,float d,float z,BOOL upper,BOOL prems)
{
		if(!pEnt)return ;
		double ag;float x,y;
		if(upper)		glNormal3f(0.0f,0.0f,1.0f);
		else glNormal3f(0.0f,0.0f,-1.0f);
		if(pEnt->flag&ENT_ISLINE)
		{
			ag=pEnt->gout-HALFDPI;
			if(prems==FALSE)glDrawPartDisk(pEnt->xstart,pEnt->ystart,d,ag+DPI,z);
			glBegin(GL_POLYGON);			
				x=float(pEnt->xstart+d*cos(ag));y=float(pEnt->ystart+d*sin(ag));
				glVertex3f(x,y,z);
				x=float(pEnt->xend+d*cos(ag));y=float(pEnt->yend+d*sin(ag));
				glVertex3f(x,y,z);
				x=float(pEnt->xend-d*cos(ag));y=float(pEnt->yend-d*sin(ag));
				glVertex3f(x,y,z);
				x=float(pEnt->xstart-d*cos(ag));y=float(pEnt->ystart-d*sin(ag));
				glVertex3f(x,y,z);
			glEnd();
			glDrawPartDisk(pEnt->xend,pEnt->yend,d,ag,z);
		}
		else
		{
/*da*/		double a,da=glDeltaAngleForArcSplit(pEnt->r);
			if(prems==FALSE){
				if(pEnt->angle>0)glDrawPartDisk(pEnt->xstart,pEnt->ystart,d,pEnt->gstart-DPI,z);
				else glDrawPartDisk(pEnt->xstart,pEnt->ystart,d,pEnt->gstart,z);
			}
			glBegin(GL_QUAD_STRIP);
			if(pEnt->angle>0){
				for(a=pEnt->gstart;a<pEnt->gend;a+=da){
					x=float(pEnt->xc+(pEnt->r+d)*cos(a));
					y=float(pEnt->yc+(pEnt->r+d)*sin(a));
					glVertex3f(x,y,z);
					x=float(pEnt->xc+(pEnt->r-d)*cos(a));
					y=float(pEnt->yc+(pEnt->r-d)*sin(a));
					glVertex3f(x,y,z);
				}
			}else{
				for(a=pEnt->gstart;a>pEnt->gend;a-=da){
					x=float(pEnt->xc+(pEnt->r+d)*cos(a));
					y=float(pEnt->yc+(pEnt->r+d)*sin(a));
					glVertex3f(x,y,z);
					x=float(pEnt->xc+(pEnt->r-d)*cos(a));
					y=float(pEnt->yc+(pEnt->r-d)*sin(a));
					glVertex3f(x,y,z);
				}
			}
			a=pEnt->gend;
			x=float(pEnt->xc+(pEnt->r+d)*cos(a));
			y=float(pEnt->yc+(pEnt->r+d)*sin(a));
			glVertex3f(x,y,z);
			x=float(pEnt->xc+(pEnt->r-d)*cos(a));
			y=float(pEnt->yc+(pEnt->r-d)*sin(a));
			glVertex3f(x,y,z);			
			glEnd();
			if(pEnt->angle>0)glDrawPartDisk(pEnt->xend,pEnt->yend,d,pEnt->gend,z);
			else glDrawPartDisk(pEnt->xend,pEnt->yend,d,pEnt->gend+DPI,z);
		}
}
//////////////绘制线条的体
void glDrawNcEntSolid(EntFull *pEnt,float d,float z,float zh,BOOL prems)
{
		if(!pEnt)return ;
		double ag;
		glDrawEntFace(pEnt,d,z,FALSE,prems);
		if(pEnt->flag&ENT_ISLINE)
		{
			float dx,dy;
			ag=pEnt->gout-HALFDPI;
			dx=(float)(d*cos(ag));dy=(float)(d*sin(ag));
			if(prems==FALSE)glDrawCylinder(pEnt->xstart,pEnt->ystart,d,ag+DPI,DPI,z,zh);
			glBegin(GL_POLYGON);
				glNormal3d(cos(ag),sin(ag),0);
				glVertex3f(pEnt->xstart+dx,pEnt->ystart+dy,z);
				glVertex3f(pEnt->xend  +dx,pEnt->yend  +dy,z);
				glVertex3f(pEnt->xend  +dx,pEnt->yend  +dy,zh);				
				glVertex3f(pEnt->xstart+dx,pEnt->ystart+dy,zh);
			glEnd();
			glBegin(GL_POLYGON);	
				glNormal3d(-cos(ag),-sin(ag),0);			
				glVertex3f(pEnt->xstart-dx,pEnt->ystart-dy,zh);
				glVertex3f(pEnt->xend  -dx,pEnt->yend  -dy,zh);
				glVertex3f(pEnt->xend  -dx,pEnt->yend  -dy,z);
				glVertex3f(pEnt->xstart-dx,pEnt->ystart-dy,z);	
			glEnd();
			glDrawCylinder(pEnt->xend,pEnt->yend,d,ag,DPI,z,zh);

		}
		else
		{
			if(pEnt->angle>0)
			{
				if(prems==FALSE)glDrawCylinder(pEnt->xstart,pEnt->ystart,d,pEnt->gstart-DPI,DPI,z,zh);
				glDrawCylinder(pEnt->xc,pEnt->yc,pEnt->r+d,pEnt->gstart,pEnt->angle,z,zh);
				glDrawCylinder(pEnt->xc,pEnt->yc,pEnt->r-d,pEnt->gend,-pEnt->angle,z,zh,FALSE);
				glDrawCylinder(pEnt->xend,pEnt->yend,d,pEnt->gend,DPI,z,zh);
			}
			else{
				if(prems==FALSE)glDrawCylinder(pEnt->xstart,pEnt->ystart,d,pEnt->gstart,DPI,z,zh);
				glDrawCylinder(pEnt->xc,pEnt->yc,pEnt->r-d,pEnt->gstart,pEnt->angle,z,zh,FALSE);
				glDrawCylinder(pEnt->xc,pEnt->yc,pEnt->r+d,pEnt->gend,-pEnt->angle,z,zh);	
				glDrawCylinder(pEnt->xend,pEnt->yend,d,pEnt->gend+DPI,DPI,z,zh);
			}
		}
	glDrawEntFace(pEnt,d,zh,TRUE,prems);
}

/////绘制环
void glDrawLoop(ELoop *ploop,int drawtype,float d,BOOL randomcolor)
{
	if(ploop==NULL||ploop->loop==NULL)return;
	if(randomcolor)
//		glColor3b(rand()%256,rand()%256,rand()%256);
		glColor3f(rand()%256/255.0f,rand()%256/255.0f,rand()%256/255.0f);
	EntFull*pEnt;
	pEnt=ploop->loop;
	do{
		if(drawtype==GL_DRAW_LINE)	glDrawEntLine(pEnt);
		else if(drawtype==GL_DRAW_FACE)			glDrawEntFace(pEnt,d);
//		else ASSERT(FALSE);
		pEnt=pEnt->next;
	}while(pEnt!=ploop->loop);
	if(ploop->next)glDrawLoop(ploop->next,drawtype,d,randomcolor);
	if(ploop->inner)glDrawLoop(ploop->inner,drawtype,d,randomcolor);

}
////////////绘制环上的所有结点
static void glDrawLoopVex(ELoop*ploop,GLUtesselator *tess,
			CTypedPtrArray<CArray<float*,float*>,float*>&vexar){
	double dv[3];float *fv;dv[2]=0;
	EntFull*pEnt=ploop->loop;
	do{
		fv=new float[2];vexar.Add(fv);
		dv[0]=fv[0]=pEnt->xstart;dv[1]=fv[1]=pEnt->ystart;
		gluTessVertex(tess, dv, fv); 
		if(!(pEnt->flag&ENT_ISLINE)){
			double a,da=glDeltaAngleForArcSplit(pEnt->r);
			if(pEnt->angle>0){
				for(a=pEnt->gstart+da;a<pEnt->gend;a+=da){
					fv=new float[2];vexar.Add(fv);
					dv[0]=pEnt->xc+pEnt->r*cos(a);fv[0]=(float)dv[0];
					dv[1]=pEnt->yc+pEnt->r*sin(a);fv[1]=(float)dv[1];
					gluTessVertex(tess, dv, fv); 				
				}
			}else{
				for(a=pEnt->gstart-da;a>pEnt->gend;a-=da){
					fv=new float[2];vexar.Add(fv);
					dv[0]=pEnt->xc+pEnt->r*cos(a);fv[0]=(float)dv[0];
					dv[1]=pEnt->yc+pEnt->r*sin(a);fv[1]=(float)dv[1];
					gluTessVertex(tess, dv, fv); 				
				}
			}
		}
		pEnt=pEnt->next;
	}while(pEnt!=ploop->loop);
}
///////////绘制层面
void glTessRegionFace(ELoop*ploop,GLUtesselator *tess,POLYGONS_DATA* pdata){

	gluTessBeginPolygon(tess, pdata); 
		gluTessBeginContour(tess); 
//		gluNextContour(tess, GLU_ETERIOR/*GLU_CW*/);
		glDrawLoopVex(ploop,tess,pdata->pfar);
		gluTessEndContour(tess);
		ploop=ploop->inner;
		while(ploop){
//			gluNextContour(tess, GLU_INTERIOR/*GLU_CW*/);
			gluTessBeginContour(tess); 		
			glDrawLoopVex(ploop,tess,pdata->pfar);
			gluTessEndContour(tess);
			ploop=ploop->next;
		}
	ASSERT(pdata->vNum==0);
	gluTessEndPolygon(tess); 
	int size=pdata->pfar.GetSize();
	for(int i=0;i<size;i++)delete []pdata->pfar[i];
	pdata->pfar.RemoveAll();
}
///////////绘制环的柱面
void glDrawLoopCylinder(ELoop*ploop,float z,float zh){
	EntFull*pEnt=ploop->loop;
	do{
		if(pEnt->flag&ENT_ISLINE){
			glBegin(GL_POLYGON);
				glNormal3d(sin(pEnt->gout),-cos(pEnt->gout),0);
				glVertex3f(pEnt->xstart,pEnt->ystart,z);
				glVertex3f(pEnt->xend,pEnt->yend,z);
				glVertex3f(pEnt->xend,pEnt->yend,zh);				
				glVertex3f(pEnt->xstart,pEnt->ystart,zh);
			glEnd();
		}else{
			glDrawCylinder(pEnt->xc,pEnt->yc,pEnt->r,pEnt->gstart,
				pEnt->angle,z,zh,pEnt->angle>0);
		}
		pEnt=pEnt->next;
	}while(pEnt!=ploop->loop);
	if(ploop->inner)glDrawLoopCylinder(ploop->inner,z,zh);
	if(ploop->next)glDrawLoopCylinder(ploop->next,z,zh);


}
#if _DEBUG
static void CALLBACK myTessError(GLenum errno){
/*	#define GLU_TESS_MISSING_BEGIN_POLYGON  GLU_TESS_ERROR1 100151 ox18738
	#define GLU_TESS_MISSING_BEGIN_CONTOUR  GLU_TESS_ERROR2 100152
	#define GLU_TESS_MISSING_END_POLYGON    GLU_TESS_ERROR3 100153
	#define GLU_TESS_MISSING_END_CONTOUR    GLU_TESS_ERROR4 100154
	#define GLU_TESS_COORD_TOO_LARGE        GLU_TESS_ERROR5 100155
	#define GLU_TESS_NEED_COMBINE_CALLBACK  GLU_TESS_ERROR6	100156			*/
	CString es;
	es.Format("GLU_TESS_ERROR:%d",_errno);
	AfxMessageBox(es);
}
#endif

static void CALLBACK myBegin(GLenum type,POLYGONS_DATA* pdata){
	pdata->gbtype=type;
	ASSERT(pdata->vNum==0);
}
static void CALLBACK myEnd(POLYGONS_DATA* pdata){
	if(pdata->vNum>0){
		GL_BEGIN gb;
		gb.type=pdata->gbtype;
		gb.vNum=pdata->vNum;
		gb.pVertex=new float[gb.vNum*2];
		memcpy(gb.pVertex,pdata->var.GetData(),sizeof(float)*2*gb.vNum);
		pdata->pgar.Add(gb);
		pdata->var.RemoveAll();
		pdata->vNum=0;
	}

}
static void CALLBACK myVertex(GLfloat * vertex_data,POLYGONS_DATA* pdata){
	pdata->vNum++;
	pdata->var.Add(vertex_data[0]);
	pdata->var.Add(vertex_data[1]);
	
}

static void CALLBACK myCombine(GLdouble coords[3], GLdouble *vertex_data[4],
							GLfloat weight[4], GLfloat **dataOut,POLYGONS_DATA* pdata )
{
    GLfloat *vertex;
    vertex = (GLfloat *) malloc(2 * sizeof(GLfloat));
	pdata->pfar.Add(vertex);
    vertex[0] =(float) coords[0];
    vertex[1] =(float) coords[1];
    *dataOut = vertex;
}

void glpreTessellate(GLUtesselator*tess){
	gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (GLCALLBACK)myBegin);  
	gluTessCallback(tess, GLU_TESS_VERTEX_DATA,(GLCALLBACK)myVertex);
	gluTessCallback(tess, GLU_TESS_END_DATA, (GLCALLBACK)myEnd);
	gluTessCallback(tess, GLU_TESS_COMBINE_DATA , (GLCALLBACK)myCombine);
#if _DEBUG
	gluTessCallback(tess, GLU_ERROR, (GLCALLBACK)myTessError); 
#endif
	gluTessNormal(tess,0.0,0.0,1.0);
}
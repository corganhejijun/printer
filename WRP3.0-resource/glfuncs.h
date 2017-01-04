/////////////////////////用于多边形分割的结构

typedef CArray<GL_BEGIN,GL_BEGIN>POLYGONS;
typedef struct _POLYGONS_DATA{
	POLYGONS	pgar;
	CArray<float,float>var;
	CTypedPtrArray<CArray<float*,float*>,float*>pfar;
	int gbtype;
	int vNum;
}POLYGONS_DATA;

//////////////////////////
/////绘制线条
void glDrawEntLine(EntFull *pEnt);
void glDrawEntLine(EntRaw *pEnt,float z=0.0f);
//////////////绘制半圆
void glDrawPartDisk(float xc,float yc,float d,double sa,float z=0,double ag=DPI);
///////绘制线条的面
void glDrawEntFace(EntFull *pEnt,float d,float z=0,BOOL upper=TRUE,BOOL prems=FALSE);
//////////////绘制线条的体
void glDrawNcEntSolid(EntFull *pEnt,float d,float z,float zh,BOOL prems=FALSE);
/////绘制环
void glDrawLoop(ELoop *ploop,int drawtype,float d=0,BOOL randomcolor=TRUE);

void glTessRegionFace(ELoop*ploop,GLUtesselator *tess,POLYGONS_DATA*pdata);
///////////绘制环的柱面
void glDrawLoopCylinder(ELoop*ploop,float z,float zh);
void glpreTessellate(GLUtesselator*tess);
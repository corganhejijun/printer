/////////////////////////���ڶ���ηָ�Ľṹ

typedef CArray<GL_BEGIN,GL_BEGIN>POLYGONS;
typedef struct _POLYGONS_DATA{
	POLYGONS	pgar;
	CArray<float,float>var;
	CTypedPtrArray<CArray<float*,float*>,float*>pfar;
	int gbtype;
	int vNum;
}POLYGONS_DATA;

//////////////////////////
/////��������
void glDrawEntLine(EntFull *pEnt);
void glDrawEntLine(EntRaw *pEnt,float z=0.0f);
//////////////���ư�Բ
void glDrawPartDisk(float xc,float yc,float d,double sa,float z=0,double ag=DPI);
///////������������
void glDrawEntFace(EntFull *pEnt,float d,float z=0,BOOL upper=TRUE,BOOL prems=FALSE);
//////////////������������
void glDrawNcEntSolid(EntFull *pEnt,float d,float z,float zh,BOOL prems=FALSE);
/////���ƻ�
void glDrawLoop(ELoop *ploop,int drawtype,float d=0,BOOL randomcolor=TRUE);

void glTessRegionFace(ELoop*ploop,GLUtesselator *tess,POLYGONS_DATA*pdata);
///////////���ƻ�������
void glDrawLoopCylinder(ELoop*ploop,float z,float zh);
void glpreTessellate(GLUtesselator*tess);
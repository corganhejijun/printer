#if !defined(MYTYPES_H_INCLUDED_)
#define MYTYPES_H_INCLUDED_
#define HALFPI ((float)1.5707963267948966192313216916398)
#define HALFDPI 1.5707963267948966192313216916398
#define PI ((float)3.1415926535897932384626433832795)
#define TWOPI ((float)6.283185307179586476925286766559)
#define DPI 3.1415926535897932384626433832795


#define  LINEINTP 0
//////////
//////////////////////////误差容许范围
#define TOL .00001
#define ANGLEERROR 100
#define ZERO(X) (fabs(X)<TOL)
#define EQU1(Z,Y) ZERO((Z-Y)/(fabs(Z)+fabs(Y)+10))
#define EQU2(Z,Y) ZERO(Z-Y)
#define EQU(Z,Y) (fabs(Z-Y)<0.002)
#define ATOL .00001
#define AZERO(X) (fabs(X)<ATOL)
#define AEQU(Z,Y) AZERO(Z-Y)
#define PTEQU(X1,X2,Y1,Y2) (EQU(X1,X2)&&EQU(Y1,Y2))
#define SQUARE(X) ((X)*(X))
#define SQRT(X) ((float)sqrt(X))
//////////////ENT ENT INTERSECT MODE
#define STARTONEND		0x00020008
#define STARTINCURVE	0x00020001
#define CROSSONEND		0x00010008
#define CROSSINCURVE	0x00010001

#define STARTONSTART	0x00020002/////
#define ENDONEND		0x00080008/////
#define ENDONSTART		0x00080002////
#define ENDINCURVE		0x00080001////
#define CROSSONSTART	0x00010002/////

#define STARTONEXTEND	0x00020000//////
#define ENDONEXTEND		0x00080000////
#define EXTENDINCURVE	0x00000001//////
#define EXTENDONSTART	0x00000002//////
#define EXTENDONEND		0x00000008//////
#define EXTENDONEXTEND	0x00000000///////
#define NOCROSS			0x00000000///////

//////PT ENT RELATION
#define PTNOTONCURVE	0x0
#define PTINCURVE		0x1
#define PTONSTART		0x2
#define PTONEND			0x8
#define PTCROSSUNKNOW	0x10
#define PTONEXTRIM		0x100
#define PTUNKNOW		0xffff
////////////LOOP RELATION
#define LOOP_UNKNOWN	0
#define LOOP_CONTAIN	1
#define LOOP_CONTAINED	2
#define LOOP_BEOUT		4
#define LOOP_INTERSECT	8
//////////////ENT PROPERTIES
#define ENT_ISLINE			0x0001
#define ENT_ISNULL			0x0002
#define ENT_ISFLIP			0x0004
#define ENT_ABCVALID		0x0008
#define ENT_EXTVALID		0x0010
#define ENT_DIRVALID		0x0020
//#define ENT_BOTTOM_END		0x0040
//#define ENT_BOTTOM_START	0x0080
//#define ENT_BOTTOM_MID		0x0100
#define ENT_DIR_INVALID		0xffdf
#define ENT_EXT_INVALID		0xffef
#define ENT_NC_G00			0x0040
#define ENT_ISCIRCLE		0x0080
#define ENT_NC_GZON			0x0100
#define ENT_NC_CONTOUR		0x0200

////////////FILE MODE
///////////DRAW MODE
#define GL_DRAW_LINE	0
#define GL_DRAW_FACE	1
#define GL_DRAW_SOLID	2
/////////////CNC
#define NCG00			1
#define	NCLINE			2
#define NCARC_CW		4
#define NCARC_CCW		8
#define NCARC_CENTER	16
#define NC_POSSETXY		32
#define NC_POSSETZ		64///0x40
#define NC_CONTOUR		0x100

#define CNC_FIRST_LINE  0x03
#define CNC_SECOND_LINE 0x0c
#define CNC_LINE_CROSS  0x10

typedef void (__stdcall *GLCALLBACK)(void);

typedef struct _NcEnt{
	unsigned ms:1;
	unsigned on_contour:1;
	unsigned iscircle:1;
	unsigned reserved:13;
	short type;
	float x;
	float y;
}NcEnt;

typedef union _EntRaw{
	struct{
	float bulge;
	float xstart,ystart;
	float xend,yend;

	};
	struct{
	float angle;
	float xc,yc;
	float r,sa;
	};
}EntRaw;
typedef struct _EntFull{
	int flag;
	float xstart,ystart;
	float xend,yend;
	float gin,gout;
	float xmin,ymin;
	float xmax,ymax;
	_EntFull*poffset;
	union{
	struct{float angle,xc,yc,r,gstart,gend;};
	struct{double a,b,c;};
	};
	_EntFull*next,*pre;
}EntFull;

typedef struct _ELoop{
	EntFull* loop;
	float xmin,ymin;
	float xmax,ymax;
	_ELoop* inner;
	_ELoop* next;
}ELoop;
typedef struct _GL_BEGIN{
	int type;
	int vNum;
	float  *pVertex;
}GL_BEGIN;

typedef struct _ELayer{
	int			iEntNum;////////////空间中实体数组中本层的实体数目
	int			iNcEntNum;//////////本层的NC指令数组
	float		z;//////////////////本层的起始Z坐标
	ELoop*		pLoopOriginalBTree;//本层的未处理实体的二叉环树
	ELoop*		pLoopOriginalTree;//本层的未处理实体的环树
	ELoop*		pLoopExtremeTree;///经处理过的极限实体的环树
	ELoop*		pLoopOffsetBTree;////由偏移实体环树生成的二叉树
	ELoop*		pLoopOffsetTree;////处理生成的偏移实体的环树
	EntRaw*		pEntRaw;////////////本层的原实体数组起始地址
	NcEnt*		pNcEnt;/////////////本层的NC指令的数组起始地址
	GL_BEGIN*	pPolygonsOriginal;
	GL_BEGIN*	pPolygonsExtreme;
	int			iPolygonsOriginal;
	int			iPolygonsExtreme;
}ELayer;

typedef struct _ELayerInfo{
	int iLoopCount;
	int iNcCount;
	float xmin,xmax;
	float ymin,ymax;
}ELayerInfo;

typedef struct _ESpace{
	int flag;
	int		EntNum;//数组中实体的数目或NC指令的数目
	int		LayerNum;//数组中层的数目
	float	xmin,ymin;
	float	xmax,ymax;
	float	zmin,zmax;
	float*	pz;
	EntRaw*	pEntRaw;////所有实体得的数组
	NcEnt *	pNcEnts;//NC指令的数组
	ELayer* pLayer;//存储所有的层
}ESpace;
 

typedef struct _EXPoint{
	float x;
	float y;
}EXPoint;

#define EMU_RPD_DUIN	0x1
#define EMU_RPD_PAUZ	0x2
#define EMU_RPD_STOP	0x3
#define EMU_PRD_NONE	0x0
#define EMULATE_DUIN	0x01
#define EMULATE_PAUZ	0x02
#define EMULATE_STOP	0x03
#define PRODUCE_DUIN	0x11
#define	PRODUCE_PAUZ	0x12
#define PRODUCE_STOP	0x13

#define PROD_PAUZ_NONE			0x1f
#define PROD_PAUZ_O1_TO_O		0x01
#define PROD_PAUZ_IPOL_ENT		0x02
#define PROD_PAUZ_O_TO_O1		0x04
#define PROD_PAUZ_IPOL_Z		0x08
#define PROD_PAUZ_WAIT_GP		0x10


typedef struct _EMULATE_STATUS{
	CMutex* pMutex;
	short pzpos;
	short emode;
	int ilayer;
	int icnc;
}EMULATE_STATUS;

void ReleaseLoop(ELoop* ploop,BOOL DeleteEnt=TRUE);
#endif
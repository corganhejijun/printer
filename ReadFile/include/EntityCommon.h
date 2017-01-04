#ifndef __ENTITY_COMMON_H__
#define __ENTITY_COMMON_H__
#include <stdio.h>

#define FALSE   0
#define TRUE    1
typedef int BOOL;

#define NULL 0
#define ERROR_MSG(msg) printf_s(msg)

#define SQUARE(X) ((X)*(X))

//////////////////////////误差容许范围
#define TOL (.00001)
#define ATOL (.00001)
#define ANGLEERROR 100
#define ZERO(X) (fabs((X)) < TOL)
#define AZERO(X) (fabs(X) < ATOL)
#define EQU(Z, Y) (fabs((double)((Z) - (Y))) < 0.002)
#define AEQU(Z,Y) AZERO(Z-Y)
#define PTEQU(X1,X2,Y1,Y2) (EQU(X1, X2) && EQU(Y1, Y2))

//////////////ENT PROPERTIES
#define ENT_ISLINE			0x0001
#define ENT_ISNULL			0x0002
#define ENT_ISFLIP			0x0004
#define ENT_ABCVALID		0x0008
#define ENT_EXTVALID		0x0010
#define ENT_DIRVALID		0x0020

#define ENT_ISCIRCLE		0x0080

typedef struct _NcEnt {
    unsigned ms : 1;
    unsigned on_contour : 1;
    unsigned iscircle : 1;
    unsigned reserved : 13;
    short type;
    float x;
    float y;
}NcEnt;

typedef union _EntRaw {
    struct {
        float bulge;
        float xstart, ystart;
        float xend, yend;

    };
    struct {
        float angle;
        float xc, yc;
        float r, sa;
    };
}EntRaw;

typedef struct _GL_BEGIN {
    int type;
    int vNum;
    float  *pVertex;
}GL_BEGIN;

typedef struct _EntFull {
    int flag;
    float xstart, ystart;
    float xend, yend;
    float gin, gout;
    float xmin, ymin;
    float xmax, ymax;
    _EntFull*poffset;
    union {
        struct { float angle, xc, yc, r, gstart, gend; };
        struct { double a, b, c; };
    };
    _EntFull*next, *pre;
}EntFull;

typedef struct _ELoop {
    EntFull* loop;
    float xmin, ymin;
    float xmax, ymax;
    _ELoop* inner;
    _ELoop* next;
}ELoop;

typedef struct _ELayer {
    int         iEntNum;
    int         iNcEntNum;
    float       z;
    ELoop*      pLoopOriginalBTree;
    ELoop*      pLoopOriginalTree;
    ELoop*      pLoopExtremeTree;///经处理过的极限实体的环树
    ELoop*      pLoopOffsetBTree;
    ELoop*      pLoopOffsetTree;////处理生成的偏移实体的环树
    EntRaw*     pEntRaw;////////////本层的原实体数组起始地址
    NcEnt*      pNcEnt;/////////////本层的NC指令的数组起始地址
    GL_BEGIN*   pPolygonsOriginal;
    GL_BEGIN*   pPolygonsExtreme;
    int         iPolygonsOriginal;
    int         iPolygonsExtreme;
}ELayer;

typedef struct _ESpace {
    int flag;
    int		EntNum;
    int		LayerNum;
    float	xmin, ymin;
    float	xmax, ymax;
    float	zmin, zmax;
    float*	pz;
    EntRaw*	pEntRaw;
    NcEnt *	pNcEnts;
    ELayer* pLayer;
}ESpace;

typedef struct _EXPoint {
    float x;
    float y;
}EXPoint;

#endif

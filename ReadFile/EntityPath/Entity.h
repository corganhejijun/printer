#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "EntityCommon.h"

//////PT ENT RELATION
#define PTNOTONCURVE	0x0
#define PTINCURVE		0x1
#define PTONSTART		0x2
#define PTONEND			0x8
#define PTCROSSUNKNOW	0x10
#define PTONEXTRIM		0x100
#define PTUNKNOW		0xffff
#define ENT_EXT_INVALID		0xffef

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

/////////////CNC
#define NCG00		    1
#define ENT_NC_G00		0x0040
#define	NCLINE			2
#define NCARC_CW		4
#define NCARC_CCW		8	

class Entity {
public:
    void ParamMakeExt(EntFull* pEnt);
    void EntGetFull(EntFull* pEntfull, EntRaw* pEnt);
    int EntPtOnLine(EntFull* pEnt, float x, float y);
    int EntPtOnArc(EntFull* parc, float& ag);
    int EntHaveBottom(EntFull* pEnt, float ymin, BOOL flip);
    void FlipEnt(EntFull* pEnt);
    float EntDirChange(EntFull*pin, EntFull*pout, float d);
    void ParamMakeDir(EntFull*pEnt);
    void CenterAngle(float& ag, float center);
    void Ent_Offset_LeftHand(EntFull* pEnt, float d);
    void EntCopy(EntFull* dest, EntFull* src);
    void ParamMakeLine(EntFull* pEnt);
    int XEntEnt(EntFull* pEnt1, EntFull* pEnt2, float& x, float& y, float& ag1, float& ag2, BOOL extcheck);
    int XLineLine(EntFull* pEnt1, EntFull* pEnt2, float& x, float& y);
    int XLineLine(double a1, double b1, double c1, EntFull* pEnt, float& x, float& y);
    int XLineLine(double a1, double b1, double c1, double a2, double b2, double c2, float& x, float& y);
    int XLineArc(EntFull*pEnt, EntFull*pArc, float& x, float& y, float& ag);
    int XLineArc(double a, double b, double c, EntFull*pArc, float& x1, float& y1, float& ag1, float& x2,
        float& y2, float& ag2, BOOL arcisnext);
    int XLineCircle(EntFull*pArc, double a, double b, double c, float& x1, float& y1, float& x2, float&y2);
    int XArcLine(EntFull* pArc, EntFull* pEnt, float& x, float& y, float& ag);
    int XArcArc(EntFull* pArc1, EntFull* pArc2, float& x, float& y, float& ag1, float& ag2);
    void EntTrimOffsetEnd(EntFull* p, float x, float y, float& ag, float d, float& x1, float& y1);
    void EntTrimOffsetStart(EntFull* p, float x, float y, float& ag, float d, float& x2, float& y2);
    EntFull* EntNewArc(float xc, float yc, float xstart, float ystart, float xend, float yend, float gstart, float gend, float r);
    EntFull* EntNewArc(float xc, float yc, float gstart, float ag, float r);
    EntFull* EntBreakEnd(EntFull* pEnt, float x, float y, float ag, int p);
    EntFull* EntBreakStart(EntFull* pEnt, float x, float y, float ag, int p);
    BOOL NcToEntFull(NcEnt* p, float& Px, float& Py, EntFull& e);
};

#endif
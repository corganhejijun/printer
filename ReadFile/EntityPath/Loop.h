#ifndef __LOOP_H__
#define __LOOP_H__

#include "EntityCommon.h"

#include <vector>
using namespace std;

#include "Entity.h"

////////////LOOP RELATION
#define LOOP_UNKNOWN	0
#define LOOP_CONTAIN	1
#define LOOP_CONTAINED	2
#define LOOP_BEOUT		4
#define LOOP_INTERSECT	8

/////////////CNC
#define NCG00			1
#define	NCLINE			2
#define NCARC_CW		4
#define NCARC_CCW		8
#define NCARC_CENTER	16
#define NC_POSSETXY		32
#define NC_POSSETZ		64///0x40
#define NC_CONTOUR		0x10

class Loop {
    static double ax, bx;
    static bool xpCompare(const EXPoint xp1, const EXPoint xp2);
public:
    Entity m_entity;
    void LoopGetExt(ELoop*ploop);
    ELoop* EGetLoop(EntFull* &pLoop, EntFull* &pRear);
    ELoop* BuildLoopBTree(vector<ELoop*> &ELoopArray);
    void LoopArrangeDirection(ELoop*ploop, BOOL exterior);
    int LoopRelationSimple(ELoop*pLoop1, ELoop*pLoop2);
    int LoopRelationSimple2(ELoop *pLoop1, ELoop * pLoop2);
    int PtInLoop(float x, float y, ELoop *pLoop);
    float PtLineAngle(float x, float y, EntFull *pEnt);
    int LoopDirection(ELoop*ploop);
    void ReverseLoop(ELoop*ploop);
    void ReOrganizeEnts(EntFull*pEnt);
    ELoop* LoopTreeCopyNoEnt(ELoop* pLoopTree);
    void BuildLoopTreeFromBTree(ELoop* pLoopBtree);
    ELoop* LoopTreeCopy(ELoop* pLoopTree, float scale);
    ELoop* BuildLayerLoopOffset(ELoop*& ploop, float d, BOOL exterior);
    BOOL Link_Point_To_Point(ELoop* ploop, EntFull*& p, float d, int& count);
    void AngleFix(float& ag1, float& ag2);
    void Loop_Remove_Ent(ELoop* ploop, EntFull* p);
    BOOL Link_Ent_To_Ent(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er);
    BOOL Link_Ent_Tangent_Ent(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er = FALSE);
    BOOL Link_Point_Tangent_Ent(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er = FALSE);
    BOOL Link_Ent_Tangent_Point(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er = FALSE);
    void ReleaseLoop(ELoop* ploop, BOOL DeleteEnt = TRUE);
    void LoopSfxCheck(ELoop*& ploop, BOOL exterior);
    ELoop* LoopFromArc(EntFull*pArc);
    void LoopEntNullRemove(ELoop* ploop);
    ELoop* LoopSelfX(ELoop* ploop, EntFull*& pEnt1, EntFull* pEnt2);
    void LoopTreeGetExt(ELoop* ploop);
    void LoopToParellelNC(ELoop* ploop, double angle, float d, float lenmin, vector<NcEnt>& ncar);
    void NcXLineLoop(double a, double b, double c, double angle, ELoop* ploop, vector<EXPoint>* pxps);
    int EntOnLineStatus(double a, double b, double c, double angle, EntFull* p, bool end);
    void LoopContourToNC(ELoop*ploop, float r, vector<NcEnt>& ncar);
};

#endif
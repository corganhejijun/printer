#ifndef __POLY_LINE_H__
#define __POLY_LINE_H__

#include "EntityCommon.h"
#include "Entity.h"

class EPolyLine
{
    long bCheckSelfXOnLoopBuild;
public:
    void Reverse();
    int Append(EPolyLine *pEpl);
    EPolyLine(EntFull*pEnt);
    virtual ~EPolyLine();
    float xstart, ystart;
    float xend, yend;
    EntFull *pLoop;
    EntFull *pRear;
};
#endif

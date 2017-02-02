#ifndef __DRAW_ENTITY_H__
#define __DRAW_ENTITY_H__

#include "EntityCommon.h"

#define _USE_MATH_DEFINES
#include <math.h>

/////ªÊ÷∆œﬂÃı
///MAX-16  MIN-128
#define DELTA_ANGLE_16 0.39269908169872415480783042290994//16
#define DELTA_ANGLE_32 0.19634954084936207740391521145497//32
#define DELTA_ANGLE_64 0.098174770424681038701957605727484//64
#define DELTA_ANGLE_128 0.049087385212340519350978802863742//128
#define DELTA_ANGLE_256 0.024543692606170259675489401431871//256
#define DELTA_ANGLE_MAX DELTA_ANGLE_16
#define DELTA_ANGLE_MIN	DELTA_ANGLE_128

class DrawEntity {
public:
    void glDrawNcEntSolid(EntFull *pEnt, float d, float z, float zh, BOOL prems);
    void glDrawEntFace(EntFull *pEnt, float d, float z = 0, BOOL upper = TRUE, BOOL prems = FALSE);
    void glDrawEntLine(EntFull *pEnt);
    void glDrawPartDisk(float xc, float yc, float d, double sa, float z, double ag = M_PI);
    void glDrawCylinder(float xc, float yc, float d, double sa, double ag, float z, float zh, BOOL exterior = TRUE);
    double glDeltaAngleForArcSplit(float r);
};

#endif
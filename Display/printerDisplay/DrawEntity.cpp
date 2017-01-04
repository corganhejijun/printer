#include "DrawEntity.h"

#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

//////////////绘制线条的体
void DrawEntity::glDrawNcEntSolid(EntFull *pEnt, float d, float z, float zh, BOOL prems) {
    if (!pEnt)
        return;
    double ag;
    glDrawEntFace(pEnt, d, z, FALSE, prems);
    if (pEnt->flag&ENT_ISLINE) {
        float dx, dy;
        ag = pEnt->gout - M_PI_2;
        dx = (float)(d*cos(ag)); dy = (float)(d*sin(ag));
        if (prems == FALSE)
            glDrawCylinder(pEnt->xstart, pEnt->ystart, d, ag + M_PI, M_PI, z, zh);
        glBegin(GL_POLYGON);
        glNormal3d(cos(ag), sin(ag), 0);
        glVertex3f(pEnt->xstart + dx, pEnt->ystart + dy, z);
        glVertex3f(pEnt->xend + dx, pEnt->yend + dy, z);
        glVertex3f(pEnt->xend + dx, pEnt->yend + dy, zh);
        glVertex3f(pEnt->xstart + dx, pEnt->ystart + dy, zh);
        glEnd();
        glBegin(GL_POLYGON);
        glNormal3d(-cos(ag), -sin(ag), 0);
        glVertex3f(pEnt->xstart - dx, pEnt->ystart - dy, zh);
        glVertex3f(pEnt->xend - dx, pEnt->yend - dy, zh);
        glVertex3f(pEnt->xend - dx, pEnt->yend - dy, z);
        glVertex3f(pEnt->xstart - dx, pEnt->ystart - dy, z);
        glEnd();
        glDrawCylinder(pEnt->xend, pEnt->yend, d, ag, M_PI, z, zh);
    } else {
        if (pEnt->angle>0) {
            if (prems == FALSE)
                glDrawCylinder(pEnt->xstart, pEnt->ystart, d, pEnt->gstart - M_PI, M_PI, z, zh);
            glDrawCylinder(pEnt->xc, pEnt->yc, pEnt->r + d, pEnt->gstart, pEnt->angle, z, zh);
            glDrawCylinder(pEnt->xc, pEnt->yc, pEnt->r - d, pEnt->gend, -pEnt->angle, z, zh, FALSE);
            glDrawCylinder(pEnt->xend, pEnt->yend, d, pEnt->gend, M_PI, z, zh);
        } else {
            if (prems == FALSE)
                glDrawCylinder(pEnt->xstart, pEnt->ystart, d, pEnt->gstart, M_PI, z, zh);
            glDrawCylinder(pEnt->xc, pEnt->yc, pEnt->r - d, pEnt->gstart, pEnt->angle, z, zh, FALSE);
            glDrawCylinder(pEnt->xc, pEnt->yc, pEnt->r + d, pEnt->gend, -pEnt->angle, z, zh);
            glDrawCylinder(pEnt->xend, pEnt->yend, d, pEnt->gend + M_PI, M_PI, z, zh);
        }
    }
    glDrawEntFace(pEnt, d, zh, TRUE, prems);
}

void DrawEntity::glDrawCylinder(float xc, float yc, float d, double sa, double ag, float z, float zh, BOOL exterior) {
    double a, da, ea = ag + sa, vcos, vsin;
    d = float(fabs(d));
    da = glDeltaAngleForArcSplit(d);
    //////////柱面
    float x, y;
    glBegin(GL_QUAD_STRIP);
    if (exterior) {
        if (ag > 0) {
            for (a = sa; a < ea; a += da) {
                vcos = cos(a); vsin = sin(a);
                x = float(xc + d*vcos); y = float(yc + d*vsin);
                glNormal3d(vcos, vsin, 0);
                glVertex3f(x, y, zh); glVertex3f(x, y, z);
            }
        } else {
            for (a = sa; a > ea; a -= da) {
                vcos = cos(a); vsin = sin(a);
                x = float(xc + d*vcos); y = float(yc + d*vsin);
                glNormal3d(vcos, vsin, 0);
                glVertex3f(x, y, zh); glVertex3f(x, y, z);
            }
        }
        vcos = cos(ea); vsin = sin(ea);
        x = float(xc + d*vcos); y = float(yc + d*vsin);
        glNormal3d(vcos, vsin, 0);
        glVertex3f(x, y, zh); glVertex3f(x, y, z);
    } else {
        if (ag > 0) {
            for (a = sa; a < ea; a += da) {
                vcos = cos(a); vsin = sin(a);
                x = float(xc + d*vcos); y = float(yc + d*vsin);
                glNormal3d(-vcos, -vsin, 0);
                glVertex3f(x, y, zh); glVertex3f(x, y, z);
            }
        } else {
            for (a = sa; a > ea; a -= da) {
                vcos = cos(a); vsin = sin(a);
                x = float(xc + d*vcos); y = float(yc + d*vsin);
                glNormal3d(-vcos, -vsin, 0);
                glVertex3f(x, y, zh); glVertex3f(x, y, z);
            }
        }
        vcos = cos(ea); vsin = sin(ea);
        x = float(xc + d*vcos); y = float(yc + d*vsin);
        glNormal3d(-vcos, -vsin, 0);
        glVertex3f(x, y, zh); glVertex3f(x, y, z);
    }
    glEnd();
}

///////绘制线条的面
void DrawEntity::glDrawEntFace(EntFull *pEnt, float d, float z, BOOL upper, BOOL prems) {
    if (!pEnt)
        return;
    double ag; 
    float x, y;
    if (upper)
        glNormal3f(0.0f, 0.0f, 1.0f);
    else
        glNormal3f(0.0f, 0.0f, -1.0f);
    if (pEnt->flag&ENT_ISLINE)
    {
        ag = pEnt->gout - M_PI_2;
        if (prems == FALSE)
            glDrawPartDisk(pEnt->xstart, pEnt->ystart, d, ag + M_PI, z);
        glBegin(GL_POLYGON);
        x = float(pEnt->xstart + d*cos(ag)); y = float(pEnt->ystart + d*sin(ag));
        glVertex3f(x, y, z);
        x = float(pEnt->xend + d*cos(ag)); y = float(pEnt->yend + d*sin(ag));
        glVertex3f(x, y, z);
        x = float(pEnt->xend - d*cos(ag)); y = float(pEnt->yend - d*sin(ag));
        glVertex3f(x, y, z);
        x = float(pEnt->xstart - d*cos(ag)); y = float(pEnt->ystart - d*sin(ag));
        glVertex3f(x, y, z);
        glEnd();
        glDrawPartDisk(pEnt->xend, pEnt->yend, d, ag, z);
    } else {
        /*da*/		double a, da = glDeltaAngleForArcSplit(pEnt->r);
        if (prems == FALSE) {
            if (pEnt->angle>0)
                glDrawPartDisk(pEnt->xstart, pEnt->ystart, d, pEnt->gstart - M_PI, z);
            else
                glDrawPartDisk(pEnt->xstart, pEnt->ystart, d, pEnt->gstart, z);
        }
        glBegin(GL_QUAD_STRIP);
        if (pEnt->angle>0) {
            for (a = pEnt->gstart; a<pEnt->gend; a += da) {
                x = float(pEnt->xc + (pEnt->r + d)*cos(a));
                y = float(pEnt->yc + (pEnt->r + d)*sin(a));
                glVertex3f(x, y, z);
                x = float(pEnt->xc + (pEnt->r - d)*cos(a));
                y = float(pEnt->yc + (pEnt->r - d)*sin(a));
                glVertex3f(x, y, z);
            }
        } else {
            for (a = pEnt->gstart; a>pEnt->gend; a -= da) {
                x = float(pEnt->xc + (pEnt->r + d)*cos(a));
                y = float(pEnt->yc + (pEnt->r + d)*sin(a));
                glVertex3f(x, y, z);
                x = float(pEnt->xc + (pEnt->r - d)*cos(a));
                y = float(pEnt->yc + (pEnt->r - d)*sin(a));
                glVertex3f(x, y, z);
            }
        }
        a = pEnt->gend;
        x = float(pEnt->xc + (pEnt->r + d)*cos(a));
        y = float(pEnt->yc + (pEnt->r + d)*sin(a));
        glVertex3f(x, y, z);
        x = float(pEnt->xc + (pEnt->r - d)*cos(a));
        y = float(pEnt->yc + (pEnt->r - d)*sin(a));
        glVertex3f(x, y, z);
        glEnd();
        if (pEnt->angle>0)
            glDrawPartDisk(pEnt->xend, pEnt->yend, d, pEnt->gend, z);
        else
            glDrawPartDisk(pEnt->xend, pEnt->yend, d, pEnt->gend + M_PI, z);
    }
}

//////////////绘制半圆
void DrawEntity::glDrawPartDisk(float xc, float yc, float d, double sa, float z, double ag) {
    float x, y; 
    double a, ea = sa + ag;
    if (!(ag > 0)) {
        ERROR_MSG("glDrawPartDisk error\n");
        return;
    }
    glBegin(GL_POLYGON);
    for (a = sa; a<ea; a += DELTA_ANGLE_MAX) {
        x = float(xc + d*cos(a));
        y = float(yc + d*sin(a));
        glVertex3f(x, y, z);
    }
    x = float(xc + d*cos(ea));
    y = float(yc + d*sin(ea));
    glVertex3f(x, y, z);
    glEnd();
}

double DrawEntity::glDeltaAngleForArcSplit(float r) {
    ////////////////////////////1
    //	return DELTA_ANGLE_32;
    /////////////////////////////2
    if (r<5)
        return DELTA_ANGLE_32;
    else if (r>40)
        return DELTA_ANGLE_256;
    return 
        DELTA_ANGLE_32 * 5 / r;
    ////////////////////////3
    if (r<5)
        return DELTA_ANGLE_16;
    else if (r>320)
        return DELTA_ANGLE_128;
    return
        DELTA_ANGLE_16 / sqrt(r / 5);
    /////////////////////////4
    //	if(r<5)return DELTA_ANGLE_16;
    //	else if(r>2560) return DELTA_ANGLE_128;
    //	return DELTA_ANGLE_16/pow(r/5,0.33333333333333333333333333);
}

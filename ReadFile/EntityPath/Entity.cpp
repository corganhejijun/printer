#include "Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>


void Entity::ParamMakeExt(EntFull*pEnt) {
    if (pEnt->flag & ENT_ISLINE) {
        if (pEnt->xstart < pEnt->xend) {
            pEnt->xmin = pEnt->xstart; pEnt->xmax = pEnt->xend; 
        }
        else {
            pEnt->xmin = pEnt->xend; pEnt->xmax = pEnt->xstart; 
        }
        if (pEnt->ystart < pEnt->yend) {
            pEnt->ymin = pEnt->ystart; pEnt->ymax = pEnt->yend; 
        }
        else {
            pEnt->ymin = pEnt->yend; pEnt->ymax = pEnt->ystart; 
        }
    }
    else {
        if (pEnt->flag & ENT_ISCIRCLE) {
            pEnt->xmax = pEnt->xc + pEnt->r; pEnt->xmin = pEnt->xc - pEnt->r;
            pEnt->ymax = pEnt->yc + pEnt->r; pEnt->ymin = pEnt->yc - pEnt->r;
        }
        else {
            if (pEnt->gstart*pEnt->gend <= 0) 
                pEnt->xmax = pEnt->xc + pEnt->r;
            else 
                pEnt->xmax = pEnt->xstart>pEnt->xend ? pEnt->xstart : pEnt->xend;

            if ((pEnt->gstart - M_PI)*(pEnt->gend - M_PI) <= 0 || (pEnt->gstart + M_PI)*(pEnt->gend + M_PI) <= 0)
                pEnt->xmin = pEnt->xc - pEnt->r;
            else 
                pEnt->xmin = pEnt->xstart>pEnt->xend ? pEnt->xend : pEnt->xstart;

            if ((pEnt->gstart - M_PI_2)*(pEnt->gend - M_PI_2) <= 0 || pEnt->gstart <= -1.5f * M_PI) 
                pEnt->ymax = pEnt->yc + pEnt->r;
            else 
                pEnt->ymax = pEnt->ystart>pEnt->yend ? pEnt->ystart : pEnt->yend;

            if ((pEnt->gstart - 1.5f * M_PI)*(pEnt->gend - 1.5f * M_PI) <= 0 || (pEnt->gstart + M_PI_2)*(pEnt->gend + M_PI_2) <= 0) 
                pEnt->ymin = pEnt->yc - pEnt->r;
            else 
                pEnt->ymin = pEnt->ystart>pEnt->yend ? pEnt->yend : pEnt->ystart;
        }
    }
    pEnt->flag |= ENT_EXTVALID;
}

void Entity::EntGetFull(EntFull*pEntfull, EntRaw*pEnt) {
    if(!(pEntfull && pEnt))
        return;
    if (pEnt->bulge == 0) {
        pEntfull->xstart = pEnt->xstart;
        pEntfull->ystart = pEnt->ystart;
        pEntfull->xend = pEnt->xend;
        pEntfull->yend = pEnt->yend;
        pEntfull->flag = ENT_ISLINE;
    } else {
        pEntfull->angle = pEnt->angle;
        pEntfull->r = pEnt->r;
        pEntfull->xc = pEnt->xc; pEntfull->yc = pEnt->yc;
        if (EQU(pEnt->angle, M_PI * 2)) {
            pEntfull->flag = ENT_ISCIRCLE;
            pEntfull->gstart = 0; pEntfull->gend = (float)(M_PI * 2);
            pEntfull->xstart = pEntfull->xend = pEntfull->xc + pEntfull->r;
            pEntfull->ystart = pEntfull->yend = pEntfull->yc;
        }
        else
        {
            pEntfull->flag = 0;
            pEntfull->gstart = pEnt->sa;
            pEntfull->gend = pEntfull->gstart + pEntfull->angle;
            pEntfull->xstart = float(pEntfull->xc + pEntfull->r*cos(pEntfull->gstart));
            pEntfull->ystart = float(pEntfull->yc + pEntfull->r*sin(pEntfull->gstart));
            pEntfull->xend = float(pEntfull->xc + pEntfull->r*cos(pEntfull->gend));
            pEntfull->yend = float(pEntfull->yc + pEntfull->r*sin(pEntfull->gend));
        }
    }
    pEntfull->poffset = NULL;
    pEntfull->next = pEntfull->pre = NULL;
}

int Entity::EntPtOnArc(EntFull*parc, float& ag) {
    if (parc->angle>0) {
        while (ag<parc->gstart - ATOL)
            ag += (float)(M_PI*2);
        while (ag>parc->gend + ATOL)
            ag -= (float)(M_PI*2);
    }
    else {
        while (ag>parc->gstart + ATOL)
            ag -= (float)(M_PI*2);
        while (ag<parc->gend - ATOL)
            ag += (float)(M_PI*2);
    }
    float da1 = ag - parc->gstart, da2 = ag - parc->gend;
    float cross = da1*da2;
    if (cross < -ATOL)
        return PTINCURVE;
    else if (cross > ATOL)
        return PTNOTONCURVE;
    if (fabs(da1) <= fabs(da2))
        return PTONSTART;
    else
        return PTONEND;
}

//////判断最低点的类型1 端点， 2 极值点
//////找到最低点
int Entity::EntHaveBottom(EntFull*pEnt, float ymin, BOOL flip) {
    if (pEnt->flag&ENT_ISLINE) {
        if ((!flip) && EQU(pEnt->yend, ymin))
        {
            if (EQU(pEnt->ystart, ymin))
                return 2;
            return 1;
        }
        else if (flip&&EQU(pEnt->ystart, ymin))
        {
            if (EQU(pEnt->yend, ymin))
                return 2;
            return 1;
        }
        else
            return 0;
    }
    else {
        if ((!flip) && EQU(pEnt->yend, ymin))
            return 1;
        else if (flip&&EQU(pEnt->ystart, ymin))
            return 1;
        else if (EQU(pEnt->yc - pEnt->r, ymin) && ((pEnt->gstart - M_PI*1.5f)*(pEnt->gend - M_PI*1.5f) < 0
            || (pEnt->gstart + M_PI_2)*(pEnt->gend + M_PI_2) < 0))
            return 2;
        else
            return 0;
    }
}

void Entity::FlipEnt(EntFull*pEnt) {
    float v;
    v = pEnt->xstart; pEnt->xstart = pEnt->xend; pEnt->xend = v;
    v = pEnt->ystart; pEnt->ystart = pEnt->yend; pEnt->yend = v;
    if (!(pEnt->flag & ENT_ISLINE)) {
        v = pEnt->gstart; pEnt->gstart = pEnt->gend; pEnt->gend = v;
        pEnt->angle = -pEnt->angle;
    }
    pEnt->flag ^= ENT_ISFLIP;
}

float Entity::EntDirChange(EntFull*pin, EntFull*pout, float d) {
    float da, x, y;
    if ((pin->flag & ENT_ISFLIP) || (pout->flag&ENT_ISFLIP)) {
        ERROR_MSG("EntDirChange error\n");
        return 0;
    }
    if (!(pin->flag & ENT_DIRVALID)) 
        ParamMakeDir(pin);
    if (!(pout->flag & ENT_DIRVALID))
        ParamMakeDir(pout);
    da = pout->gin - pin->gout;
    CenterAngle(da, 0);
    ////////如果转向为PI时，要判断是＋pi　还是　－pi
    if (AEQU(fabs(da), M_PI)) {
        x = pin->xend; y = pin->yend;
        if (d != 0) {
            x += float(d*sin(pin->gout));
            y -= float(d*cos(pin->gout));
        }
        if (pin->flag&ENT_ISLINE) {
            if (pout->flag&ENT_ISLINE)
                da = (float)M_PI;
            else {
                if (pout->angle > 0)
                    da = (float)(-M_PI);
                else
                    da = (float)M_PI;
            }
        }
        else if (pout->flag&ENT_ISLINE) {
            if (pin->angle > 0)
                da = (float)(-M_PI);
            else
                da = (float)M_PI;
        }
        else {
            double dy1, dy2;
            dy1 = (pin->yc - y)*cos(pin->gout) - (pin->xc - x)*sin(pin->gout);
            dy2 = (pout->yc - y)*cos(pin->gout) - (pout->xc - x)*sin(pin->gout);
            if (dy1*dy2 == 0) {
                ERROR_MSG("EntDirChange 3\n");
                return 0;
            }
            if ((dy1 > 0 && (dy2<0 || dy2>dy1)) || (dy1 < 0 && dy2<0 && dy2>dy1))
                da = (float)(-M_PI);
            else 
                da = (float)M_PI;
        }
    }
    return da;
}

void Entity::ParamMakeDir(EntFull*pEnt) {
    if (pEnt->flag & ENT_ISLINE) {
        pEnt->gout = (float)atan2(pEnt->yend - pEnt->ystart, pEnt->xend - pEnt->xstart);
        pEnt->gin = pEnt->gout;
    }
    else {
        if (pEnt->angle > 0) {
            pEnt->gin = pEnt->gstart + (float)M_PI_2;
            pEnt->gout = pEnt->gend + (float)M_PI_2;
        }
        else {
            pEnt->gin = pEnt->gstart - (float)M_PI_2;
            pEnt->gout = pEnt->gend - (float)M_PI_2;
        }
    }
    pEnt->flag |= ENT_DIRVALID;
}

void Entity::CenterAngle(float& ag, float center) {
    while (ag - center < -M_PI)
        ag += (float)(M_PI*2);
    while (ag - center >= M_PI)
        ag -= (float)(M_PI*2);
}

void Entity::Ent_Offset_LeftHand(EntFull* pEnt, float d) {
    if (pEnt->flag & ENT_ISFLIP) {
        ERROR_MSG("Ent_Offset_LeftHand error\n");
        return;
    }
    EntFull*p = new EntFull;
    memset(p, 0, sizeof(EntFull));
    EntCopy(p, pEnt);
    if (p->flag & ENT_ISLINE) {
        float dx, dy;
        dx = float(-d*sin(p->gout)); dy = float(d*cos(p->gout));
        p->xstart += dx;		p->xend += dx;
        p->ystart += dy;		p->yend += dy;
        ParamMakeLine(p);
    } else if (p->flag & ENT_ISCIRCLE) {
        if (pEnt->angle>0 && pEnt->r <= d) {
            delete p; pEnt->poffset = NULL; return;
        } else {
            if (p->angle>0)
                p->r = p->r - d;
            else
                p->r = p->r + d;
            p->xend = p->xstart = p->xc + p->r;
        }
    } else {
        if (pEnt->angle>0 && pEnt->r <= d) {
            float ag, xm, ym, dd;
            delete p;
            if (pEnt->r<d) {
                xm = (pEnt->xstart + pEnt->xend) / 2; ym = (pEnt->ystart + pEnt->yend) / 2;
                dd = (float)sqrt(d*d - SQUARE(pEnt->xstart - xm) - SQUARE(pEnt->ystart - ym));
                ag = pEnt->gstart + pEnt->angle / 2;
                pEnt->xc = float(xm - dd*cos(ag));
                pEnt->yc = float(ym - dd*sin(ag));
                pEnt->gstart = float(atan2(pEnt->ystart - pEnt->yc, pEnt->xstart - pEnt->xc));
                pEnt->gend = float(atan2(pEnt->yend - pEnt->yc, pEnt->xend - pEnt->xc));
                pEnt->angle = pEnt->gend - pEnt->gstart;
                if (!(pEnt->angle > -2 * M_PI * 2) && !(pEnt->angle < 2 * M_PI * 2)) {
                    ERROR_MSG("Ent_Offset_LeftHand error 2\n");
                    return;
                }
                CenterAngle(pEnt->angle, (float)M_PI);
                pEnt->gend = pEnt->gstart + pEnt->angle;
                pEnt->r = d;
                ParamMakeDir(pEnt);
            }
            pEnt->poffset = NULL;
            return;
        } else {
            if (p->angle>0)p->r = p->r - d;
            else p->r = p->r + d;
            p->xstart = float(p->xc + p->r*cos(p->gstart));
            p->ystart = float(p->yc + p->r*sin(p->gstart));
            p->xend = float(p->xc + p->r*cos(p->gend));
            p->yend = float(p->yc + p->r*sin(p->gend));
        }
    }
    p->pre = p->next = NULL;
    p->flag &= ENT_EXT_INVALID;
    pEnt->poffset = p;
    p->poffset = pEnt;
    ParamMakeExt(p);
}

void Entity::EntCopy(EntFull* dest, EntFull* src) {
    dest->flag = src->flag;
    dest->xstart = src->xstart;
    dest->ystart = src->ystart;
    dest->xend = src->xend;
    dest->yend = src->yend;
    dest->xmin = src->xmin;
    dest->ymin = src->ymin;
    dest->xmax = src->xmax;
    dest->ymax = src->ymax;
    dest->gin = src->gin;
    dest->gout = src->gout;
    dest->a = src->a; dest->b = src->b; dest->c = src->c;
    dest->next = NULL; dest->pre = NULL; dest->poffset = NULL;
}

void Entity::ParamMakeLine(EntFull* pEnt) {
    if (!(pEnt->flag & ENT_ISLINE)) {
        ERROR_MSG("ParamMakerLine error\n");
        return;
    }
    pEnt->a = pEnt->yend - pEnt->ystart;
    pEnt->b = pEnt->xstart - pEnt->xend;
    double sq = sqrt(pEnt->a*pEnt->a + pEnt->b*pEnt->b);
    pEnt->a /= sq; pEnt->b /= sq;
    pEnt->c = -(pEnt->a*pEnt->xstart + pEnt->b*pEnt->ystart);
    pEnt->flag |= ENT_ABCVALID;
}

int Entity::EntPtOnLine(EntFull *pEnt, float x, float y) {
    float de = (float)sqrt(SQUARE(pEnt->xstart - pEnt->xend) + SQUARE(pEnt->ystart - pEnt->yend));
    float d = (float)((x - pEnt->xstart)*(pEnt->xend - pEnt->xstart) + (y - pEnt->ystart)*(pEnt->yend - pEnt->ystart)) / de;
    if (ZERO(d / 10) && fabs(d)<fabs(d - de))
        return PTONSTART;
    else if (EQU(d, de))
        return PTONEND;
    else if (d>0 && d<de)
        return PTINCURVE;
    else
        return PTNOTONCURVE;
}

int Entity::XEntEnt(EntFull* pEnt1, EntFull* pEnt2, float& x, float& y, float& ag1, float& ag2, BOOL extcheck) {
    if (!(pEnt1->flag & ENT_EXTVALID) && !(pEnt2->flag & ENT_EXTVALID)) {
        ERROR_MSG("XEntEnt error\n");
        return 0;
    }
    if (extcheck && (pEnt1->xmax<pEnt2->xmin || pEnt1->xmin>pEnt2->xmax || pEnt1->ymin>pEnt2->ymax || pEnt1->ymax<pEnt2->ymin))
        return 0;
    else if (pEnt1->flag&ENT_ISLINE) {
        if (pEnt2->flag&ENT_ISLINE)
            return XLineLine(pEnt1, pEnt2, x, y);
        else
            return XLineArc(pEnt1, pEnt2, x, y, ag2);
    }
    else {
        if (pEnt2->flag&ENT_ISLINE)
            return XArcLine(pEnt1, pEnt2, x, y, ag1);
        else
            return XArcArc(pEnt1, pEnt2, x, y, ag1, ag2);
    }
}

int Entity::XArcArc(EntFull* pArc1, EntFull* pArc2, float& x, float& y, float& ag1, float& ag2) {
    int p2, p11, p12, p21, p22;
    float ag12, ag22;
    float x2, y2;
    if (PTEQU(pArc1->xc, pArc2->xc, pArc1->yc, pArc2->yc)) {
        if (EQU(pArc1->r, pArc2->r)) {
            return NOCROSS;
        }
        else 
            return NOCROSS;
    }
    else {
        double a, b, c;
        a = 2 * (pArc2->xc - pArc1->xc);
        b = 2 * (pArc2->yc - pArc1->yc);
        c = SQUARE(pArc1->xc) - SQUARE(pArc2->xc) + SQUARE(pArc1->yc) - SQUARE(pArc2->yc) + SQUARE(pArc2->r) - SQUARE(pArc1->r);
        p2 = XLineArc(a, b, c, pArc2, x, y, ag2, x2, y2, ag22, TRUE);
        if (p2 == 0)
            return NOCROSS;
        p21 = p2 & 0x0000ff;
        p22 = p2 >> 16;
        ag1 = (float)atan2(y - pArc1->yc, x - pArc1->xc);
        ag12 = (float)atan2(y2 - pArc1->yc, x2 - pArc1->xc);
        p11 = EntPtOnArc(pArc1, ag1); 
        if (!p11)
            CenterAngle(ag1, pArc1->gend);
        p12 = EntPtOnArc(pArc1, ag12); 
        if (!p12)
            CenterAngle(ag12, pArc1->gend);
    }
    if (!((p11&&p21) || (p12&&p22)))
        return NOCROSS;
    else if ((!p11 || !p21) && (p12&&p22)) {
        /////如果点1不在其中一条线上，而点2在两条线上
        ag1 = ag12; ag2 = ag22; x = x2; y = y2;
        return (p12 << 16) + p22;
    }
    else if ((!p12 || !p22) && (p11&&p21)) {
        /////如果点2不在其中一条线上，而点1在两条线上
        return (p11 << 16) + p21;
    }
    else if (p11 && p21 && p12 && p22) {
        if (fabs(ag1 - pArc1->gend)>fabs(ag12 - pArc1->gend) || p11 == PTONEND || p21 == PTONSTART) {
            ag1 = ag12; ag2 = ag22; x = x2; y = y2;
            return (p12 << 16) + p22;
        }
        else
            return (p11 << 16) + p21;
    }
    else {
        ERROR_MSG("XArcArc error\n");
        return 0;
    }
    return NOCROSS;


}

int Entity::XArcLine(EntFull* pArc, EntFull* pEnt, float& x, float& y, float& ag) {
    int p1, p11, p12, p21, p22;
    float x2, y2, ag2;
    if (!(pEnt->flag & ENT_ABCVALID)) {
        ERROR_MSG("XArcLine error\n");
        return 0;
    }
    p1 = XLineArc(pEnt->a, pEnt->b, pEnt->c, pArc, x, y, ag, x2, y2, ag2, FALSE);
    if (p1 == 0)
        return NOCROSS;
    p11 = p1 & 0x000000ff;
    p12 = p1 >> 16;
    p21 = EntPtOnLine(pEnt, x, y);
    p22 = EntPtOnLine(pEnt, x2, y2);
    if (!((p11&&p21) || (p12&&p22)))
        return NOCROSS;
    else if ((!p11 || !p21) && (p12&&p22)) {
        /////如果点1不在其中一条线上，而点2在两条线上
        ag = ag2; x = x2; y = y2;
        return (p12 << 16) + p22;
    }
    else if ((!p12 || !p22) && (p11&&p21)) {
        /////如果点2不在其中一条线上，而点1在两条线上，或者
        /////如果点2不在两条线上，但是点1在其中一条线上，取交点为点1
        return (p11 << 16) + p21;
    }
    else if (p11&&p21&&p12&&p22) {
        if (fabs(ag - pArc->gend)>fabs(ag2 - pArc->gend) || p11 == PTONEND || p21 == PTONSTART) {
            ag = ag2; x = x2; y = y2;
            return (p12 << 16) + p22;
        }
        else
            return (p11 << 16) + p21;
    }
    else {
        ERROR_MSG("XArcLine error 2\n");
        return 0;
    }
    return NOCROSS;
}

int Entity::XLineArc(EntFull*pEnt, EntFull*pArc, float& x, float& y, float& ag) {
    int p2, p11, p12, p21, p22;
    float x2, y2, ag2;
    if (!(pEnt->flag & ENT_ABCVALID)) {
        ERROR_MSG("XLineArc error\n");
        return 0;
    }
    p2 = XLineArc(pEnt->a, pEnt->b, pEnt->c, pArc, x, y, ag, x2, y2, ag2, TRUE);
    if (p2 == 0)return NOCROSS;
    p21 = p2 & 0x0000ff;
    p22 = p2 >> 16;
    p12 = EntPtOnLine(pEnt, x2, y2);
    p11 = EntPtOnLine(pEnt, x, y);
    if (!((p11&&p21) || (p12&&p22)))
        return NOCROSS;
    else if ((!p11 || !p21) && (p12&&p22)) {
        /////如果点1不在其中一条线上，而点2在两条线上
        ag = ag2; x = x2; y = y2;
        return (p12 << 16) + p22;
    }
    else if ((!p12 || !p22) && (p11&&p21)) {
        /////如果点2不在其中一条线上，而点1在两条线上
        return (p11 << 16) + p21;
    }
    else if (p11&&p21&&p12&&p22) {
        /////如果点1,2都在两条线上
        if (fabs(ag - pArc->gstart)>fabs(ag2 - pArc->gstart) || p11 == PTONEND || p21 == PTONSTART) {
            ag = ag2; x = x2; y = y2;
            return (p12 << 16) + p22;
        }
        else
            return (p11 << 16) + p21;
    }
    else {
        ERROR_MSG("XLineArc error 2\n");
        return 0;
    }
    return NOCROSS;
}

int Entity::XLineArc(double a, double b, double c, EntFull*pArc, float& x1, float& y1,
    float& ag1, float& x2, float& y2, float& ag2, BOOL arcisnext) {
    int isn, rtn;
    isn = XLineCircle(pArc, a, b, c, x1, y1, x2, y2);
    if (isn == 0)
        return NOCROSS;
    else {
        ag1 = (float)atan2(y1 - pArc->yc, x1 - pArc->xc);
        rtn = EntPtOnArc(pArc, ag1);
        if (!rtn) {
            if (arcisnext)
                CenterAngle(ag1, pArc->gstart);
            else
                CenterAngle(ag1, pArc->gend);
        }
        if (isn == 1) {
            if (rtn)rtn |= PTONEXTRIM;
        }
        else if (isn == 2) {
            ag2 = (float)atan2(y2 - pArc->yc, x2 - pArc->xc);
            isn = EntPtOnArc(pArc, ag2);
            if (!isn) {
                if (arcisnext)
                    CenterAngle(ag1, pArc->gstart);
                else 
                    CenterAngle(ag1, pArc->gend);
            }
            else
                rtn += isn << 16;
        }
        return rtn;
    }
}

//返回直线与圆弧的交点数和交点的值
int Entity::XLineCircle(EntFull*pArc, double a, double b, double c, float& x1, float& y1, float& x2, float&y2) {
    double aa, bb, cc, dd;
    if (a != 0 && ZERO(1000 * b / a)) {
        x1 = x2 = float(-c / a);
        dd = pArc->r*pArc->r - (x1 - pArc->xc)*(x1 - pArc->xc);
        if (dd>0) {
            dd = sqrt(dd);
            y1 = float(pArc->yc + dd);
            y2 = float(pArc->yc - dd);
            return 2;
        }
        else if (dd>-0.00001) {
            y1 = y2 = pArc->yc;
            return 1;
        }
        else
            return 0;
    }
    else if (b != 0 && ZERO(1000 * a / b)) {
        y1 = y2 = float(-c / b);
        dd = pArc->r*pArc->r - (y1 - pArc->yc)*(y1 - pArc->yc);
        if (dd>0) {
            dd = sqrt(dd);
            x1 = float(pArc->xc + dd);
            x2 = float(pArc->xc - dd);
            return 2;
        }
        else if (dd>-0.00001) {
            x1 = x2 = pArc->xc;
            return 1;
        }
        else
            return 0;
    }
    else {
        if (fabs(a)<fabs(b)) {
            ///解方程组aa x^2+ bb x +cc=0
            ////////ax+by+c=0
            aa = a*a + b*b;
            bb = 2 * a*c + 2 * a*b*pArc->yc - 2 * b*b*pArc->xc;
            cc = SQUARE(b*pArc->xc) + c*c + SQUARE(b*pArc->yc) + 2 * b*c*pArc->yc - SQUARE(b*pArc->r);
            dd = bb*bb - 4 * aa*cc;
            if (dd>0) {
                dd = sqrt(dd);
                x1 = float((-bb + dd) / (2 * aa));
                x2 = float((-bb - dd) / (2 * aa));
                y1 = float(-(a*x1 + c) / b); y2 = float(-(a*x2 + c) / b);
                return 2;
            }
            else if (dd == 0) {
                x1 = x2 = float(-bb / aa / 2);
                y1 = y2 = float(-(a*x1 + c) / b);
                return 1;
            }
            else 
                return 0;
        }
        else {
            ///解方程组aa y^2+ bb y +cc=0
            ////////ax+by+c=0
            aa = a*a + b*b;
            bb = 2 * b*c + 2 * a*b*pArc->xc - 2 * a*a*pArc->yc;
            cc = SQUARE(a*pArc->yc) + c*c + SQUARE(a*pArc->xc) + 2 * a*c*pArc->xc - SQUARE(a*pArc->r);
            dd = bb*bb - 4 * aa*cc;
            if (dd>0) {
                dd = sqrt(dd);
                y1 = float((-bb + dd) / (2 * aa));
                y2 = float((-bb - dd) / (2 * aa));
                x1 = float(-(b*y1 + c) / a); x2 = float(-(b*y2 + c) / a);
                return 2;
            }
            else if (dd == 0) {
                y1 = y2 = float(-bb / aa / 2);
                x1 = x2 = float(-(b*y1 + c) / a);
                return 1;
            }
            else 
                return 0;
        }
    }
}

int Entity::XLineLine(EntFull* pEnt1, EntFull* pEnt2, float& x, float& y) {
    int p1, p2;
    if (!(pEnt1->flag & ENT_ABCVALID)) {
        ERROR_MSG("XLineLine error\n");
        return 0;
    }
    p2 = XLineLine(pEnt1->a, pEnt1->b, pEnt1->c, pEnt2, x, y);
    if (p2 == PTUNKNOW) {////直线重合
        return NOCROSS;
    }
    else if (p2)
        p1 = EntPtOnLine(pEnt1, x, y);
    if (!(p1&&p2))
        return NOCROSS;
    else
        return (p1 << 16) + p2;
}

int Entity::XLineLine(double a1, double b1, double c1, EntFull* pEnt, float& x, float& y) {
    if (!(pEnt->flag & ENT_ABCVALID)) {
        ERROR_MSG("XLineLine error 2\n");
        return 0;
    }
    int p = XLineLine(a1, b1, c1, pEnt->a, pEnt->b, pEnt->c, x, y);
    if (p == PTCROSSUNKNOW)
        return EntPtOnLine(pEnt, x, y);
    else
        return p;
}

/////获得两个线条实体的交点
/////只求离第一个线条实体终点更接近的一个
int Entity::XLineLine(double a1, double b1, double c1, double a2, double b2, double c2, float& x, float& y) {
    if (fabs(a1*b2 - a2*b1)<0.000001) {
        float dist;
        if (fabs(a1)<fabs(b1))dist = float(c2*b1 / b2 - c1);
        else dist = float(c2*a1 / a2 - c1);
        if (ZERO(dist))	return PTUNKNOW;
        else return PTNOTONCURVE;
    }
    else if (fabs(a1) >= fabs(a2)) {
        double dy;
        dy = (c2 - c1*(a2 / a1)) / (b1*(a2 / a1) - b2);
        y = (float)dy;
        x = (float)(-(c1 + b1*dy) / a1);
    }
    else {
        double dy;
        dy = (c1 - c2*(a1 / a2)) / (b2*(a1 / a2) - b1);
        y = (float)dy;
        x = (float)(-(c2 + b2*dy) / a2);
    }
    return PTCROSSUNKNOW;
}

void Entity::EntTrimOffsetEnd(EntFull* p, float x, float y, float& ag, float d, float& x1, float& y1) {
    if (p->flag&ENT_ISLINE) {
        ag = p->gout - (float)M_PI_2;
        x1 = (float)(x + d*cos(ag)); y1 = (float)(y + d*sin(ag));
    }
    else {
        if (p->angle>0) {
            x1 = x + (x - p->xc)*d / (p->r - d); y1 = y + (y - p->yc)*d / (p->r - d);
            p->gend = ag;
        }
        else if (p->angle<0) {
            x1 = x - (x - p->xc)*d / (d + p->r); y1 = y - (y - p->yc)*d / (d + p->r);
            p->gend = ag; ag += (float)M_PI;
        }
        p->angle = p->gend - p->gstart;
        p->poffset->gend = p->gend;
        p->poffset->angle = p->angle;
        ParamMakeDir(p);
        p->poffset->gin = p->gin; p->poffset->gout = p->gout;
    }
    p->poffset->xend = x; p->poffset->yend = y;
    p->xend = x1; p->yend = y1;
    ParamMakeExt(p); ParamMakeExt(p->poffset);
}

void Entity::EntTrimOffsetStart(EntFull* p, float x, float y, float& ag, float d, float& x2, float& y2) {
    if (p->flag & ENT_ISLINE) {
        ag = p->gout - (float)M_PI_2;
        x2 = (float)(x + d*cos(ag)); y2 = (float)(y + d*sin(ag));
    }
    else {
        if (p->angle>0) {
            x2 = x + (x - p->xc)*d / (p->r - d); y2 = y + (y - p->yc)*d / (p->r - d);
            p->gstart = ag;
        }
        else if (p->angle<0) {
            x2 = x - (x - p->xc)*d / (d + p->r); y2 = y - (y - p->yc)*d / (d + p->r);
            p->gstart = ag;
            ag += (float)M_PI;
        }
        p->angle = p->gend - p->gstart;
        p->poffset->gstart = p->gstart;
        p->poffset->angle = p->angle;
        ParamMakeDir(p);
        p->poffset->gin = p->gin; p->poffset->gout = p->gout;
    }
    p->poffset->xstart = x; p->poffset->ystart = y;
    p->xstart = x2; p->ystart = y2;
    ParamMakeExt(p);
    ParamMakeExt(p->poffset);
}

EntFull* Entity::EntNewArc(float xc, float yc, float xstart, float ystart, float xend, float yend, float gstart, float gend, float r) {
    if (AEQU(gstart, gend) || ZERO(r))return NULL;
    EntFull*p = new EntFull;
    p->flag = 0;
    p->r = r;
    p->gstart = gstart; p->gend = gend;
    p->angle = gend - gstart;
    p->xc = xc; p->yc = yc;
    p->xstart = xstart; p->ystart = ystart;
    p->xend = xend;	p->yend = yend;
    p->poffset = NULL;
    ParamMakeExt(p);
    ParamMakeDir(p);
    return p;
}

EntFull* Entity::EntNewArc(float xc, float yc, float gstart, float ag, float r) {
    if (ZERO(ag) || ZERO(r))
        return NULL;
    EntFull*p = new EntFull;
    p->flag = 0;
    p->angle = ag; p->r = r;
    p->gstart = gstart; p->gend = gstart + ag;
    p->xc = xc; p->yc = yc;
    p->xstart = float(p->xc + p->r*cos(p->gstart));
    p->ystart = float(p->yc + p->r*sin(p->gstart));
    p->xend = float(p->xc + p->r*cos(p->gend));
    p->yend = float(p->yc + p->r*sin(p->gend));
    ParamMakeExt(p); ParamMakeDir(p);
    p->poffset = NULL;
    return p;
}

EntFull* Entity::EntBreakStart(EntFull* pEnt, float x, float y, float ag, int p) {
    if (p)
        return NULL;
    EntFull*pe = new EntFull;
    EntCopy(pe, pEnt);
    if (p == CROSSONEND || p == STARTONEND) {
        pEnt->flag |= ENT_ISNULL;
    }
    else {
        pe->xend = x; pe->yend = y;
        pEnt->xstart = x; pEnt->ystart = y;
        if (!(pe->flag&ENT_ISLINE)) {
            pe->gend = ag;
            pEnt->gstart = ag;
            ParamMakeDir(pe);
            ParamMakeDir(pEnt);
        }
        ParamMakeExt(pe);
        ParamMakeExt(pEnt);
    }
    return pe;
}

EntFull* Entity::EntBreakEnd(EntFull* pEnt, float x, float y, float ag, int p) {
    if (p)
        return NULL;
    EntFull*pe = new EntFull;
    EntCopy(pe, pEnt);
    if (p == STARTONEND || p == STARTINCURVE) {
        pEnt->flag |= ENT_ISNULL;
    }
    else {
        pe->xstart = x; pe->ystart = y;
        pEnt->xend = x; pEnt->yend = y;
        if (!(pe->flag&ENT_ISLINE)) {
            pEnt->gend = ag;
            pe->gstart = ag;
            ParamMakeDir(pe);
            ParamMakeDir(pEnt);
        }
        ParamMakeExt(pe);
        ParamMakeExt(pEnt);
    }
    return pe;
}

BOOL Entity::NcToEntFull(NcEnt* p, float& Px, float& Py, EntFull& e) {
    float Lx = p->x, Ly = p->y;
    if (p->type == NCG00) {
        e.flag = ENT_NC_G00 | ENT_ISLINE;
        e.xstart = Px; e.ystart = Py; e.xend = Lx; e.yend = Ly;
    } else if (p->type == NCLINE) {
        e.flag = ENT_ISLINE;
        e.xstart = Px; e.ystart = Py; e.xend = Lx; e.yend = Ly;
    } else if (p->type == NCARC_CW) {
        e.flag = 0; e.xc = (p + 1)->x; e.yc = (p + 1)->y;
        e.xstart = Px; e.ystart = Py; e.xend = Lx; e.yend = Ly;
        if (p->iscircle) {
            e.angle = (float)(-M_PI*2);
            e.gstart = (float)(M_PI*2);
            e.gend = 0;
            e.r = e.xstart - e.xc;
        } else {
            e.gstart = (float)atan2(Py - e.yc, Px - e.xc);
            e.gend = (float)atan2(Ly - e.yc, Lx - e.xc);
            if (e.gend >= e.gstart)e.gend -= (float)(M_PI * 2);
            e.angle = e.gend - e.gstart;
            e.r = (float)sqrt(SQUARE(Lx - e.xc) + SQUARE(Ly - e.yc));
        }
    } else if (p->type == NCARC_CCW) {
        e.flag = 0; e.xc = (p + 1)->x; e.yc = (p + 1)->y;
        e.xstart = Px; e.ystart = Py; e.xend = Lx; e.yend = Ly;
        if (p->iscircle) {
            e.angle = (float)(M_PI * 2);
            e.gstart = 0;
            e.gend = (float)(M_PI * 2);
            e.r = e.xstart - e.xc;
        } else {
            e.gstart = (float)atan2(Py - e.yc, Px - e.xc);
            e.gend = (float)atan2(Ly - e.yc, Lx - e.xc);
            if (e.gend <= e.gstart)e.gend += (float)(M_PI * 2);
            e.angle = e.gend - e.gstart;
            e.r = (float)sqrt(SQUARE(Lx - e.xc) + SQUARE(Ly - e.yc));
        }
    } else 
        return FALSE;
    ParamMakeDir(&e);
    Px = Lx; Py = Ly;
    return TRUE;
}

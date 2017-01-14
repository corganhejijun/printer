#include "Loop.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "PolyLine.h"

using namespace std;
ELoop* Loop::EGetLoop(EntFull* &pLoop, EntFull* &pRear) {
    ELoop*pEL = new ELoop;
    pLoop->pre = pRear;
    pRear->next = pLoop;
    pEL->loop = pLoop;
    pEL->inner = pEL->next = NULL;
    LoopGetExt(pEL);
    pLoop = NULL;
    return pEL;
}

/////得到环的范围
void Loop::LoopGetExt(ELoop*ploop) {
    if (!ploop || !ploop->loop)
        return;
    EntFull*pEnt = ploop->loop;
    EntFull* pb = pEnt;
    if (!(pEnt->flag & ENT_EXTVALID))
        m_entity.ParamMakeExt(pEnt);
    ploop->xmin = pEnt->xmin; ploop->ymin = pEnt->ymin;
    ploop->xmax = pEnt->xmax; ploop->ymax = pEnt->ymax;
    if (pEnt->flag & ENT_ISCIRCLE) {
    }
    else {
        while (pEnt->next != ploop->loop) {
            pEnt = pEnt->next;
            if (!(pEnt->flag & ENT_EXTVALID))
                m_entity.ParamMakeExt(pEnt);
            if (pEnt->xmax>ploop->xmax)
                ploop->xmax = pEnt->xmax;
            if (pEnt->ymax>ploop->ymax)
                ploop->ymax = pEnt->ymax;
            if (pEnt->xmin<ploop->xmin)
                ploop->xmin = pEnt->xmin;
            if (pEnt->ymin<ploop->ymin) { 
                ploop->ymin = pEnt->ymin; pb = pEnt; 
            }
        }
        ploop->loop = pb;
    }
}

/////////生成包含环内外关系的二叉树
ELoop* Loop::BuildLoopBTree(vector<ELoop*> &ELoopArray) {
    vector<ELoop*> inner, outer;
    int rela, Imax = 0, size = (int)ELoopArray.size();
    float dxmax = 0, dymax = 0, dx, dy;
    ELoop*ploop;
    if (size == 1)
        return ELoopArray[0];
    else if (size == 0)
        return NULL;
    for (int i = 0; i<size; i++) {
        ploop = ELoopArray[i];
        dx = ploop->xmax - ploop->xmin;
        dy = ploop->ymax - ploop->ymin;
        if (dx>dxmax || (dx == dxmax&&dy>dymax) || dx == dxmax 
            && dy == dymax && LoopRelationSimple(ELoopArray[Imax], ploop) == LOOP_CONTAINED)
        {
            dxmax = dx; dymax = dy; Imax = i;
        }

    }
    ploop = ELoopArray[Imax];
    ELoopArray.erase(ELoopArray.begin() + Imax);
    size--;
    for (int i = 0; i<size; i++) {
        rela = LoopRelationSimple(ploop, ELoopArray[i]);
        if (rela == LOOP_BEOUT)
            outer.push_back(ELoopArray[i]);
        else if (rela == LOOP_CONTAIN)
            inner.push_back(ELoopArray[i]);
        else 
            ERROR_MSG("区域内外关系错误");
    }
    ploop->inner = BuildLoopBTree(inner);
    ploop->next = BuildLoopBTree(outer);
    inner.clear();
    outer.clear();
    return ploop;
}

///返回值：0  pLoop2	在		pLoop1外
////////// 1  pLoop1	包含	pLoop2
////////// 2  pLoop2	包含	pLoop1 
////////// 4  无法用此办法判断
int Loop::LoopRelationSimple(ELoop*pLoop1, ELoop*pLoop2) {
    int rela;
    if (pLoop1->xmax>pLoop2->xmax&&
        pLoop1->xmin<pLoop2->xmin&&
        pLoop1->ymax>pLoop2->ymax&&
        pLoop1->ymin<pLoop2->ymin)
    {
        rela = LoopRelationSimple2(pLoop1, pLoop2);
        if(rela != LOOP_CONTAINED)
            return LOOP_UNKNOWN;
        return rela;
    }
    else if (pLoop2->xmax>pLoop1->xmax&&
        pLoop2->xmin<pLoop1->xmin&&
        pLoop2->ymax>pLoop1->ymax&&
        pLoop2->ymin<pLoop1->ymin)
    {
        rela = LoopRelationSimple2(pLoop1, pLoop2);
        if(rela != LOOP_CONTAIN)
            return LOOP_UNKNOWN;
        return rela;
    }
    else 
        return LOOP_BEOUT;

}

int Loop::LoopRelationSimple2(ELoop *pLoop1, ELoop * pLoop2) {
    int inloop, intmp;
    EntFull*pEnt = pLoop1->loop;
    do {
        inloop = PtInLoop(pEnt->xstart, pEnt->ystart, pLoop2);
        if (inloop == 0)
            break;
        else if (inloop == 1)
            return LOOP_CONTAINED;
        pEnt = pEnt->next;
    } while (inloop == 2 && pEnt != pLoop1->loop);
    pEnt = pLoop2->loop;
    intmp = inloop;
    do {
        inloop = PtInLoop(pEnt->xstart, pEnt->ystart, pLoop1);
        if (inloop == 1 && intmp == 0)
            return LOOP_CONTAIN;
        else if (inloop == 0)
            return LOOP_BEOUT;
        pEnt = pEnt->next;
    } while (inloop == 2 && pEnt != pLoop2->loop);
    ERROR_MSG("无法判断环的内外关系!");
    return LOOP_UNKNOWN;
}

//////////看点(X，Y)是否在环pLoop内
//////////是	返回	1，
////////不是	返回	0，
//无法判断则	返回	2
int Loop::PtInLoop(float x, float y, ELoop *pLoop) {
    EntFull *pEnt = pLoop->loop;
    float angle = 0, da;
    if (x<pLoop->xmin || x>pLoop->xmax || y<pLoop->ymin || y>pLoop->ymax)
        return 0;
    do {
        da = PtLineAngle(x, y, pEnt);
        if (da == ANGLEERROR)
            return 2;
        if (pEnt->flag&ENT_ISFLIP)
            angle -= da;
        else 
            angle += da;
        pEnt = pEnt->next;
    } while (pEnt&&pEnt != pLoop->loop);
    if (fabs(angle) < 0.5)
        return 0;
    else if (fabs(fabs(angle) - M_PI*2) < 0.5)
        return 1;
    else
        return 2;
}

/////////返回线条 pEnt相对于点(X,Y)的张角
////////若(X,Y)在 pEnt上则返回 ANGLEERROR
float Loop::PtLineAngle(float x, float y, EntFull *pEnt) {
    float ag;
    float dx1, dy1, dx2, dy2, d1, d2;
    if (pEnt->flag & ENT_ISLINE) {
        dx1 = pEnt->xstart - x;	dy1 = pEnt->ystart - y;
        dx2 = pEnt->xend - x;	dy2 = pEnt->yend - y;
        if (ZERO(dy2*dx1 - dy1*dx2) && m_entity.EntPtOnLine(pEnt, x, y))
            return ANGLEERROR;
        d1 = sqrt(dx1*dx1 + dy1*dy1);
        d2 = sqrt(dx2*dx2 + dy2*dy2);
        ag = (dx1*dx2 + dy1*dy2) / d1 / d2;
        //////
        if (ag<-1)
            ag = -1;
        else if (ag>1)
            ag = 1;
        ///////
        ag = float(acos(ag));
        if ((dy2*dx1 - dy1*dx2)<0)
            ag = -ag;
    }
    else {
        if (pEnt->flag&ENT_ISCIRCLE) {
            float dxc, dyc, dc;
            dxc = float(pEnt->xc - x);
            dyc = float(pEnt->yc - y);
            dc = sqrt(dxc*dxc + dyc*dyc);
            if (EQU(dc, pEnt->r))
                return ANGLEERROR;
            else if (dc<pEnt->r)
                return pEnt->angle;
            else
                return 0;
        }
        else {
            float dxc, dyc, dc, ax;
            float a, b, c;///连线段的直线方程ax+by+c=0;
            dxc = float(pEnt->xc - x);
            dyc = float(pEnt->yc - y);
            dc = sqrt(dxc*dxc + dyc*dyc);
            ax = float(atan2(-dyc, -dxc));
            if (EQU(dc, pEnt->r) && m_entity.EntPtOnArc(pEnt, ax))
                return ANGLEERROR;
            dx1 = pEnt->xstart - x; dy1 = pEnt->ystart - y;
            dx2 = pEnt->xend - x; dy2 = pEnt->yend - y;
            d1 = sqrt(dx1*dx1 + dy1*dy1);
            d2 = sqrt(dx2*dx2 + dy2*dy2);
            ag = (dx1*dx2 + dy1*dy2) / d1 / d2;
            //////
            if (ag<-1)
                ag = -1;
            else if (ag>1)
                ag = 1;
            ///////
            ag = float(acos(ag));
            //////点和弧在边线段的同侧 且 点在圆内
            a = dy2 - dy1; b = dx1 - dx2; c = dx2*dy1 - dx1*dy2;
            dxc = float(pEnt->xc + pEnt->r*cos((pEnt->gstart + pEnt->gend) / 2) - x);
            dyc = float(pEnt->yc + pEnt->r*sin((pEnt->gstart + pEnt->gend) / 2) - y);
            if (((dc - pEnt->r) / pEnt->r<-TOL) && ((a*dxc + b*dyc + c) / c > TOL))
                ag -= (float)(2 * M_PI);
            if ((dy2*dx1 - dy1*dx2) < 0)
                ag = -ag;
        }
    }
    return ag;
}

void Loop::LoopArrangeDirection(ELoop*ploop, BOOL exterior) {
    //////判断环的绕向
    int loopdir = LoopDirection(ploop);
    ///////使外环逆时针，内外顺时针
    if ((loopdir == -1 && exterior) || (loopdir == 1 && !exterior))
        ReverseLoop(ploop);
    ReOrganizeEnts(ploop->loop);
    if (ploop->next)LoopArrangeDirection(ploop->next, exterior);
    if (ploop->inner)LoopArrangeDirection(ploop->inner, !exterior);
}

//////使环首末倒置，反向
void Loop::ReverseLoop(ELoop*ploop) {
    EntFull *p = ploop->loop, *t;
    do {
        p->flag ^= ENT_ISFLIP;
        t = p->next;
        p->next = p->pre;
        p->pre = t; p = t;
    } while (p != ploop->loop);
}

void Loop::ReOrganizeEnts(EntFull*pEnt) {
    EntFull*p = pEnt;
    if (!p) {
        ERROR_MSG("ReOrganizeEnts error\n");
        return;
    }
    do {
        if (p->flag & ENT_ISFLIP)
            m_entity.FlipEnt(p);
        m_entity.ParamMakeDir(p);
        p = p->next;
    } while (p != pEnt);
}

//////一条环的绕向判断
//////返回1  如果此环是逆时针
//////返回-1 如果此环是顺时针
/////返回 0  如果无法判断
int Loop::LoopDirection(ELoop*ploop) {
    int botype = 0, by;
    float da;
    if (!ploop) {
        ERROR_MSG("LoopDirection error\n");
        return 0;
    }
    EntFull*p = ploop->loop->pre, *pEnt = NULL;
    EntFull*pEnd = ploop->loop->next;
    if (pEnd != p)
        pEnd = pEnd->next;
    if (p->flag&ENT_ISCIRCLE) {
        if (p->flag&ENT_ISFLIP) {
            if (p->angle>0)
                return -1;
            else
                return 1;
        }
        else {
            if (p->angle>0)
                return 1;
            else
                return -1;
        }
    }
    do {
        by = m_entity.EntHaveBottom(p, ploop->ymin, p->flag&ENT_ISFLIP);
        if (by<botype&&by == 0)
            break;
        if (by>botype) {
            pEnt = p; botype = by; 
        } else if (by == 2 && pEnt->flag&ENT_ISLINE &&!(p->flag&ENT_ISLINE)) {
            pEnt = p; botype = by;
        } else if (by == 2 && (pEnt->flag&ENT_ISLINE) && (p->flag&ENT_ISLINE)) {
            by = 1; pEnt = p; botype = by;
            break;
        }
        p = p->next;
    } while (p != pEnd);
    if (!botype) {
        ERROR_MSG("LoopDirection error 2\n");
        return 0;
    }
    if (botype == 0)
        return 0;
    if (pEnt->flag & ENT_ISFLIP)
        m_entity.FlipEnt(pEnt);
    if (pEnt->next->flag & ENT_ISFLIP)
        m_entity.FlipEnt(pEnt->next);
    da = m_entity.EntDirChange(pEnt, pEnt->next, 0);
    if (botype == 2 || (botype == 1 && AZERO(da))) {
        if (pEnt->flag&ENT_ISLINE) {
            if (pEnt->xstart<pEnt->xend)
                return 1;
            else 
                return -1;
        }
        else {
            if (pEnt->angle <0)
                return -1;
            else
                return 1;
        }
    }
    else if (botype == 1) {
        if (da>0)
            return 1;
        else if (da<0)
            return -1;
    }
    ERROR_MSG("LoopDirection error 3\n");
    return 0;
}

ELoop* Loop::LoopTreeCopyNoEnt(ELoop* pLoopTree) {
    if (pLoopTree == NULL)
        return NULL;
    ELoop*pLoopTreeCopy = new ELoop;
    pLoopTreeCopy->loop = pLoopTree->loop;
    pLoopTreeCopy->xmax = pLoopTree->xmax;
    pLoopTreeCopy->ymax = pLoopTree->ymax;
    pLoopTreeCopy->xmin = pLoopTree->xmin;
    pLoopTreeCopy->ymin = pLoopTree->ymin;
    pLoopTreeCopy->next = LoopTreeCopyNoEnt(pLoopTree->next);
    pLoopTreeCopy->inner = LoopTreeCopyNoEnt(pLoopTree->inner);
    return pLoopTreeCopy;
}

//////////生成包含区域内外关系的二层树
void Loop::BuildLoopTreeFromBTree(ELoop* pLoopBtree) {
    if (pLoopBtree == NULL)return;
    ELoop*ploop = pLoopBtree;
    ELoop*pRear = pLoopBtree;
    while (pRear->next)
        pRear = pRear->next;
    while (ploop) {
        ELoop *pl = ploop->inner;
        while (pl)
        {
            pRear->next = pl->inner;
            pl->inner = NULL;
            while (pRear->next)
                pRear = pRear->next;
            pl = pl->next;
        }
        ploop = ploop->next;
    }
}

ELoop* Loop::LoopTreeCopy(ELoop* pLoopTree, float scale) {
    if (pLoopTree == NULL)
        return NULL;
    EntFull *pEnt = pLoopTree->loop, *p = new EntFull, *pre = NULL;
    memset(p, 0, sizeof(EntFull));
    ELoop*pLoopTreeCopy = new ELoop;
    memset(pLoopTreeCopy, 0, sizeof(pLoopTreeCopy));
    pLoopTreeCopy->loop = p;
    pLoopTreeCopy->xmax = pLoopTree->xmax*scale;
    pLoopTreeCopy->ymax = pLoopTree->ymax*scale;
    pLoopTreeCopy->xmin = pLoopTree->xmin*scale;
    pLoopTreeCopy->ymin = pLoopTree->ymin*scale;
    do {
        p->flag = pEnt->flag;
        p->xstart = pEnt->xstart*scale; p->ystart = pEnt->ystart*scale;
        p->xend = pEnt->xend*scale; p->yend = pEnt->yend*scale;
        p->xmin = pEnt->xmin*scale; p->ymin = pEnt->ymin*scale;
        p->xmax = pEnt->xmax*scale; p->ymax = pEnt->ymax*scale;
        p->gin = pEnt->gin; p->gout = pEnt->gout;
        if (p->flag & ENT_ISLINE) {
            p->a = pEnt->a; p->b = pEnt->b; p->c = pEnt->c*scale;
        } else {
            p->xc = pEnt->xc*scale; p->yc = pEnt->yc*scale; p->r = pEnt->r*scale;
            p->angle = pEnt->angle; p->gstart = pEnt->gstart; p->gend = pEnt->gend;
        }
        p->pre = pre; pre = p; 
        p->next = new EntFull;
        memset(p->next, 0, sizeof(EntFull));
        p = p->next;
        pEnt = pEnt->next;
    } while (pEnt != pLoopTree->loop);
    delete p; pre->next = pLoopTreeCopy->loop;
    pLoopTreeCopy->loop->pre = pre;
    pLoopTreeCopy->next = LoopTreeCopy(pLoopTree->next, scale);
    pLoopTreeCopy->inner = LoopTreeCopy(pLoopTree->inner, scale);
    return pLoopTreeCopy;
}

ELoop* Loop::BuildLayerLoopOffset(ELoop*& ploop, float d, BOOL exterior) {
    int ldir, count = 0, ic = 0;
    float da; 
    BOOL LinkStatus;
    if (ploop == NULL)
        return NULL;
    EntFull* p = ploop->loop;
    ////////按顺序来偏移每个线条实体
    do {
        m_entity.Ent_Offset_LeftHand(p, d);
        p = p->next;
    } while (p != ploop->loop);
    ///////生成偏移后的环的二层树
    if (p->flag & ENT_ISCIRCLE) {
        if (p->poffset == NULL)
            goto removethis;
        else {
            ELoop*plp = new ELoop; p = p->poffset;
            plp->xmax = p->xc + p->r; plp->xmin = p->xc - p->r;
            plp->ymin = p->yc - p->r; plp->ymax = p->yc + p->r;
            plp->loop = p;
            p->next = p->pre = p;
            if (ploop->next)
                plp->next = BuildLayerLoopOffset(ploop->next, d, exterior);
            else
                plp->next = NULL;
            if (ploop->inner)
                plp->inner = BuildLayerLoopOffset(ploop->inner, d, !exterior);
            else
                plp->inner = NULL;
            return plp;
        }
    }
    /////检查角落
    do {
        LinkStatus = TRUE;
        da = m_entity.EntDirChange(p, p->next, 0);
        if (da <= ATOL) {
            p = p->next; count++; 
            continue; 
        }
        if (p->poffset == NULL) {
            if (p->next->poffset == NULL)
                LinkStatus = Link_Point_To_Point(ploop, p, d, count);
            else {
                float dap = m_entity.EntDirChange(p->pre, p, 0);
                if (dap >= -ATOL) {
                    EntFull*ppre = p->pre;
                    da += dap; da += p->gout - p->gin;
                    Loop_Remove_Ent(ploop, p);
                    p = ppre;
                    if (ppre->pre == ppre) {
                        LinkStatus = FALSE; 
                        break; 
                    }
                    if (count > 0)
                        count--;
                    LinkStatus = Link_Ent_To_Ent(ploop, p, d, count, da, TRUE);
                } else
                    LinkStatus = (Link_Point_Tangent_Ent(ploop, p, d, count, da) || Link_Point_To_Point(ploop, p, d, count));
            }
        } else {
            if (p->next->poffset == NULL) {
                float dan = m_entity.EntDirChange(p->next, p->next->next, 0);
                if (dan >= -ATOL) {
                    da += dan; da += p->next->gout - p->next->gin;
                    Loop_Remove_Ent(ploop, p->next);
                    if (p->pre == p) {
                        LinkStatus = FALSE; 
                        break; 
                    }
                    if (count < 0)
                        count++;
                    LinkStatus = Link_Ent_To_Ent(ploop, p, d, count, da, TRUE);
                } else {
                    LinkStatus = (Link_Ent_Tangent_Point(ploop, p, d, count, da) || Link_Point_To_Point(ploop, p, d, count));
                }
            } else
                LinkStatus = Link_Ent_To_Ent(ploop, p, d, count, da, FALSE);

        }
        ic++;
    } while (LinkStatus && (p != ploop->loop || count <= 2));

    if (LinkStatus) {
        LoopGetExt(ploop);
        ldir = LoopDirection(ploop);
    }
    if (LinkStatus && ((ldir == 1 && exterior) || (ldir == -1 && !exterior))) {
        ELoop*plp = new ELoop;
        EntFull*pf = NULL;
        plp->xmax = plp->xmin = plp->ymin = plp->ymax = 0;
        p = ploop->loop;
        do {
            da = m_entity.EntDirChange(p, p->next, 0);
            if (p->poffset || da <= -ATOL) {
                if (p->poffset) {
                    plp->loop = pf = p->poffset;
                    if (da <= -ATOL) {
                        pf->next = m_entity.EntNewArc(p->xend, p->yend, p->gout + (float)M_PI_2, da, d);
                        pf->next->pre = pf; pf = pf->next;
                    }
                    p = p->next;
                } else {
                    plp->loop = pf = m_entity.EntNewArc(p->xend, p->yend, p->gout + (float)M_PI_2, da, d);
                    p = p->next;
                }
                break;
            }
            p = p->next;
        } while (p != ploop->loop);
        if (pf == NULL)
            goto removethis;
        do {
            da = m_entity.EntDirChange(p, p->next, 0);
            if (p->poffset) {
                pf->next = p->poffset;
                pf->next->pre = pf; pf = pf->next;
            }
            if (da <= -ATOL) {
                pf->next = m_entity.EntNewArc(p->xend, p->yend, p->gout + (float)M_PI_2, da, d);
                pf->next->pre = pf; pf = pf->next;
            }
            p = p->next;
        } while (p != ploop->loop);
        pf->next = plp->loop; plp->loop->pre = pf;
        if (ploop->next)
            plp->next = BuildLayerLoopOffset(ploop->next, d, exterior);
        else 
            plp->next = NULL;
        if (ploop->inner)
            plp->inner = BuildLayerLoopOffset(ploop->inner, d, !exterior);
        else 
            plp->inner = NULL;
        return plp;
    }
removethis:
    ELoop*plnext = ploop->next;
    ploop->next = NULL;
    ReleaseLoop(ploop);
    ploop = plnext;
    if (ploop)
        return BuildLayerLoopOffset(ploop, d, exterior);
    else
        return NULL;
}

//////////从内存中删除一条环
void Loop::ReleaseLoop(ELoop* ploop, BOOL DeleteEnt) {
    if (!ploop)return;
    if (ploop->next)ReleaseLoop(ploop->next, DeleteEnt);
    if (ploop->inner)ReleaseLoop(ploop->inner, DeleteEnt);
    if (!ploop->loop)return;
    if (DeleteEnt) {
        EntFull*pf = ploop->loop, *p;
        pf->pre->next = NULL;
        while (pf) { p = pf->next; delete pf; pf = p; }
    }
    delete ploop;
}

void Loop::AngleFix(float& ag1, float& ag2) {
    while (ag2 < 0)
        ag2 += (float)(M_PI*2);
    while (ag2 >= M_PI*2)
        ag2 -= (float)(M_PI*2);
    while (ag2 < ag1)
        ag1 -= (float)(M_PI*2);
    while (ag2 - ag1 >= M_PI*2)
        ag1 += (float)(M_PI*2);
}

void Loop::Loop_Remove_Ent(ELoop* ploop, EntFull* p) {
    if (ploop->loop == p)
        ploop->loop = p->pre;
    p->pre->next = p->next;
    p->next->pre = p->pre;
    if (p->poffset)
        delete p->poffset;
    delete  p;
}

BOOL Loop::Link_Point_To_Point(ELoop* ploop, EntFull*& p, float d, int& count) {
    EntFull*pins = new EntFull, *p2 = p->next;
    float dir;
    float xm, ym, dd;
    ////求解圆弧
    if (p->pre == p2)
        return FALSE;
    pins->flag = 0;
    pins->xstart = p->xstart; pins->ystart = p->ystart;
    pins->xend = p2->xend; pins->yend = p2->yend;
    xm = (pins->xstart + pins->xend) / 2; ym = (pins->ystart + pins->yend) / 2;
    dd = d*d - SQUARE(pins->xstart - xm) - SQUARE(pins->ystart - ym);
    if (dd < 0)
        return FALSE;
    dd = (float)sqrt(dd);
    dir = (float)atan2(pins->yend - pins->ystart, pins->xend - pins->xstart);
    pins->xc = float(xm - dd*sin(dir));
    pins->yc = float(ym + dd*cos(dir));
    pins->gstart = float(atan2(pins->ystart - pins->yc, pins->xstart - pins->xc));
    pins->gend = float(atan2(pins->yend - pins->yc, pins->xend - pins->xc));
    AngleFix(pins->gstart, pins->gend);
    pins->angle = pins->gend - pins->gstart;
    if (fabs(pins->angle) < M_PI)
        return FALSE;
    pins->gend = pins->gstart + pins->angle;
    pins->r = d;
    m_entity.ParamMakeDir(pins);
    ////////////偏移
    pins->poffset = NULL;
    /////勾链
    pins->pre = p; pins->next = p2;
    p->next = pins; p2->pre = pins;
    Loop_Remove_Ent(ploop, p);
    if (p2 == ploop->loop&&count>2) {
        ploop->loop = p2->next; count--; 
    }
    Loop_Remove_Ent(ploop, p2);
    if (count>0)
        count--;
    p = pins->pre;
    return TRUE;
}

BOOL Loop::Link_Ent_Tangent_Ent(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er) {
    float x, y, ag1, ag2, x1, y1, x2, y2;
    int xs1, xs2, xs;
    EntFull*pn = p->next;
    if (!p->poffset || !pn->poffset)return FALSE;
    if (PTEQU(p->poffset->xend, p->next->poffset->xstart, p->poffset->yend, p->next->poffset->ystart)) {
        p = p->next;
        count++;
        return TRUE;
    }
    if (!er && (p->flag & ENT_ISLINE) && (p->next->flag & ENT_ISLINE) && da < 0.2) {
        x = float(p->xend - d*sin(p->gout + da / 2) / cos(da / 2));
        y = float(p->yend + d*cos(p->gout + da / 2) / cos(da / 2));
        xs1 = m_entity.EntPtOnLine(p->poffset, x, y);
        xs2 = m_entity.EntPtOnLine(p->next->poffset, x, y);
        if (!(xs1 && xs2))
            return FALSE;
        xs = (xs1 << 16) + xs2;
    }
    else {
        xs = m_entity.XEntEnt(p->poffset, p->next->poffset, x, y, ag1, ag2, FALSE);
        if (xs == NOCROSS)
            return FALSE;
        xs1 = xs >> 16; xs2 = xs & 0x000000ff;
    }
    m_entity.EntTrimOffsetEnd(p, x, y, ag1, d, x1, y1);
    m_entity.EntTrimOffsetStart(pn, x, y, ag2, d, x2, y2);
    AngleFix(ag1, ag2);
    EntFull *pins = m_entity.EntNewArc(x, y, x1, y1, x2, y2, ag1, ag2, d);
    pins->pre = p; pins->next = pn;
    p->next = pins; pn->pre = pins;
    if (xs == STARTONEND) {
        Loop_Remove_Ent(ploop, p);
        Loop_Remove_Ent(ploop, pn);
        if (count>0)count--;
        p = pins->pre;
    }
    else if (xs1 == PTONSTART) {
        Loop_Remove_Ent(ploop, p);
        if (count != 0)count--;
        p = pins->pre;
    }
    else if (xs2 == PTONEND) {
        if (pn == ploop->loop&&count>2) { ploop->loop = pn->next; count--; }
        Loop_Remove_Ent(ploop, pn);
        count++;
        p = pins;
    }
    else {
        if (count >= 0)count++;
        count++;
        p = pins->next;
    }
    return TRUE;
}

BOOL Loop::Link_Point_Tangent_Ent(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er) {
    float x = p->xstart, y = p->ystart, x2, y2;
    float ag, ag2, gs, ge;
    int xs;
    EntFull*pn = p->next, *pa;
    if (!pn->poffset)
        return FALSE;
    if (p->pre == pn)
        return FALSE;
    ge = p->gin + (float)M_PI_2;
    if (pn->flag & ENT_ISLINE) {
        gs = pn->gout + (float)M_PI_2;
        AngleFix(ge, gs);
        if ((pa = m_entity.EntNewArc(x, y, gs, ge - gs, d)) == NULL)
            return FALSE;
        xs = m_entity.XArcLine(pa, pn->poffset, x, y, ag);
    }
    else {
        gs = (float)atan2(pn->yc - y, pn->xc - x);
        if (pn->angle<0)
            gs += (float)M_PI;
        AngleFix(ge, gs);
        if ((pa = m_entity.EntNewArc(x, y, gs, ge - gs, d)) == NULL)
            return FALSE;
        xs = m_entity.XArcArc(pa, pn->poffset, x, y, ag, ag2);
    }
    if (xs == NOCROSS)
        return FALSE;
    if (!(xs & 0xffff0000 || xs & 0x00ff)) {
        ERROR_MSG("Link_Point_Tangent_Ent error\n");
        return FALSE;
    }
    ag -= (float)M_PI; 
    delete pa;
    m_entity.EntTrimOffsetStart(pn, x, y, ag2, d, x2, y2);
    AngleFix(ag, ag2);
    pa = m_entity.EntNewArc(x, y, p->xstart, p->ystart, x2, y2, ag, ag2, d);
    if ((xs & 0x00ff) == PTONEND) {
        if (pn == ploop->loop&&count>2) { 
            ploop->loop = pn->next; count--; 
        }
        Loop_Remove_Ent(ploop, pn);
        if (count<0)
            count++;
    }
    pa->pre = p->pre; pa->next = p;
    pa->pre->next = pa; p->pre = pa;
    Loop_Remove_Ent(ploop, p);
    if (count != 0)
        count--;
    p = pa->pre;
    return TRUE;
}

BOOL Loop::Link_Ent_Tangent_Point(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er) {
    float ag, ag2, x1, y1; 
    int xs;
    EntFull*pn = p->next, *pa;
    float x = pn->xend, y = pn->yend;
    if (!p->poffset)
        return FALSE;
    if (p->pre == pn)
        return FALSE;
    float ge, gs = pn->gout + (float)M_PI_2;
    if (p->flag & ENT_ISLINE) {
        ge = p->gout + (float)M_PI_2;
        AngleFix(ge, gs);
        if ((pa = m_entity.EntNewArc(x, y, gs, ge - gs, d)) == NULL)
            return FALSE;
        xs = m_entity.XLineArc(p->poffset, pa, x, y, ag2);
    }
    else {
        ge = (float)atan2(p->yc - y, p->xc - x);
        if (p->angle < 0)
            ge -= (float)M_PI;
        AngleFix(ge, gs);
        if ((pa = m_entity.EntNewArc(x, y, gs, ge - gs, d)) == NULL)
            return FALSE;
        xs = m_entity.XArcArc(p->poffset, pa, x, y, ag, ag2);
    }
    if (xs == NOCROSS)
        return FALSE;
    if (!(xs & 0xffff0000 || xs & 0x00ff)) {
        ERROR_MSG("Link_Ent_Tangent_Point error\n");
        return FALSE;
    }
    delete pa; 
    ag2 -= (float)M_PI;
    m_entity.EntTrimOffsetEnd(p, x, y, ag, d, x1, y1);
    AngleFix(ag, ag2);
    pa = m_entity.EntNewArc(x, y, x1, y1, pn->xend, pn->yend, ag, ag2, d);
    pa->pre = p; pa->next = pn;
    p->next = pa; pn->pre = pa;
    if ((xs >> 16) == PTONSTART) {
        Loop_Remove_Ent(ploop, p);
        if (count > 0)
            count--;
    }

    if (pn == ploop->loop&&count > 2) {
        ploop->loop = pn->next; count--;
    }
    Loop_Remove_Ent(ploop, pn);
    count++;
    p = pa;
    return TRUE;
}

BOOL Loop::Link_Ent_To_Ent(ELoop* ploop, EntFull*& p, float d, int& count, float da, BOOL er) {
    if (Link_Ent_Tangent_Ent(ploop, p, d, count, da, er) ||
        Link_Point_Tangent_Ent(ploop, p, d, count, da, er) ||
        Link_Ent_Tangent_Point(ploop, p, d, count, da, er) ||
        Link_Point_To_Point(ploop, p, d, count))
            return TRUE;
    else
        return FALSE;
}

void Loop::LoopSfxCheck(ELoop*& ploop, BOOL exterior) {
    vector<ELoop*> LoopArray;
    EntFull*p = ploop->loop, *q, *ph = NULL, *pr = NULL, *pl;
    ELoop*plp;
    ELoop*inner = ploop->inner, *next = ploop->next;
    int i = 1, j = 1, k = 0;
    ploop->inner = ploop->next = NULL;
    if (p->flag&ENT_ISCIRCLE) {
        if (next)
            LoopSfxCheck(next, exterior);
        ploop->next = next;
        if (inner)
            LoopSfxCheck(inner, !exterior);
        ploop->inner = inner;
        return;
    }
    //////检查有没有超过2PI的圆
    do {
        plp = LoopFromArc(p);
        if (plp) {
            LoopArray.push_back(plp);
            if (p->angle == 0)p->flag |= ENT_ISNULL;
        }
        p = p->next;
    } while (p != ploop->loop);
    LoopEntNullRemove(ploop);//删除空的实体
    /////找一个有效的头 //////开始检查环的自交情况 //////生成一系列不自交的环
    p = ploop->loop; q = p->next;
    pl = ploop->loop;
    while (q->next != p) {
        plp = LoopSelfX(ploop, p, q);
        if (plp) {
            LoopArray.push_back(plp);
            p = ploop->loop; q = p->next; i = j = 1;
        } else if (j == i) {
            p = p->next;
            q = q->next;
            if (p == ploop->loop) {
                i++; j++; q = q->next;
            }
        } else if (j<i) {
            if (k<j) {
                p = p->next; q = q->next; k++;
            } else if (k == j) {
                p = ph = ph->pre;
                j++; q = pr; k = 1;
            }
        }
    }
    LoopArray.push_back(ploop);
    ////自交检查结束 ////开始删除多余的反向环
    int ldir, ndir;
    if (exterior)
        ndir = -1;
    else
        ndir = 1;
    for (i = 0; i < (int)LoopArray.size(); i++) {
        LoopGetExt(LoopArray[i]);
        ldir = LoopDirection(LoopArray[i]);
        if (ldir == ndir) {
            ReleaseLoop(LoopArray[i]);
            LoopArray.erase(LoopArray.begin() + i);
            i--;
        }
    }
    if (LoopArray.size()>0) {
        for (i = 0; i < (int)LoopArray.size() - 1; i++)LoopArray[i]->next = LoopArray[i + 1];
        LoopArray[i]->next = NULL;
        ploop = LoopArray[0];
        if (next)
            LoopSfxCheck(next, exterior);
        LoopArray[i]->next = next;
        if (inner)
            LoopSfxCheck(inner, !exterior);
        ploop->inner = inner;
    }
    else 
        ploop = NULL;
}

//////解析环的自相交
ELoop* Loop::LoopSelfX(ELoop* ploop, EntFull*& pEnt1, EntFull* pEnt2) {
    float x, y, ag1, ag2; 
    int p;
    if (!pEnt1 || !pEnt2)
        return NULL;
    p = m_entity.XEntEnt(pEnt1, pEnt2, x, y, ag1, ag2, TRUE);
    if (!(p == CROSSONEND || p == CROSSINCURVE || p == STARTONEND || p == STARTINCURVE))
        p = NOCROSS;
    if (p == NOCROSS)
        return NULL;
    else {
        ELoop*plp = new ELoop;
        plp->xmax = plp->xmin = plp->ymax = plp->ymin = 0;
        plp->inner = plp->next = NULL;
        EntFull*ple1, *ple2, *pt;
        ple1 = m_entity.EntBreakEnd(pEnt1, x, y, ag1, p);
        ple2 = m_entity.EntBreakStart(pEnt2, x, y, ag2, p);

        if (pEnt1->next == pEnt2) {
            ple1->next = ple2;
            ple2->pre = ple1;
        }
        else {
            ple1->next = pEnt1->next;
            ple2->pre = pEnt2->pre;
            ple1->next->pre = ple1;
            ple2->pre->next = ple2;
        }
        ple1->pre = ple2; ple2->next = ple1;
        plp->loop = ple1; pt = ple1->next;
        do {
            if (pt == ploop->loop) {
                ploop->loop = pEnt2;
                break;
            }
            pt = pt->next;
        } while (pt != plp->loop);
        if (pEnt1->flag & ENT_ISNULL) {
            if (pEnt1 == ploop->loop)	ploop->loop = pEnt2;
            pEnt1 = pEnt1->pre;
            delete pEnt1->next;

        }
        if (pEnt2->flag & ENT_ISNULL) {
            if (pEnt2 == ploop->loop) ploop->loop = pEnt2->next;
            pEnt2 = pEnt2->next;
            delete pEnt2->pre;
        }
        pEnt1->next = pEnt2;
        pEnt2->pre = pEnt1;
        return plp;
    }
}

///////删除　环中的空实体
void Loop::LoopEntNullRemove(ELoop* ploop) {
    EntFull *pEnt, *pdel;
    while (ploop->loop->flag & ENT_ISNULL) {
        pEnt = ploop->loop;
        ploop->loop = pEnt->next;
        if (!ploop->loop) {
            ERROR_MSG("LoopEntNullRemove error\n");
            return;
        }
        ploop->loop->pre = pEnt->pre;
        pEnt->pre->next = ploop->loop;
        delete pEnt;
    }
    pEnt = ploop->loop;
    do {
        if (pEnt->flag & ENT_ISNULL) {
            pdel = pEnt;
            pEnt->next->pre = pEnt->pre;
            pEnt->pre->next = pEnt->next;
            pEnt = pEnt->next;
            delete pdel;
        }
        else 
            pEnt = pEnt->next;
    } while (pEnt != ploop->loop);
}

//////看有没有超过2pi的圆弧
ELoop* Loop::LoopFromArc(EntFull*pArc) {
    if (pArc) {
        return NULL;
    }
    if (pArc->flag & ENT_ISLINE)
        return NULL;
    if (fabs(pArc->angle) >= M_PI*2) {
        float ag;
        if (pArc->angle>0) { 
            pArc->angle -= (float)(M_PI*2); ag = (float)M_PI;
        }
        else {
            pArc->angle += (float)(M_PI*2); ag = -(float)M_PI; 
        }
        ELoop*ploop = new ELoop;
        EntFull*p = new EntFull;
        ploop->loop = p;
        ///////////////////////////
        m_entity.EntCopy(p, pArc);
        p->gstart = 0; p->gend = p->angle = ag;
        m_entity.ParamMakeDir(p); 
        m_entity.ParamMakeExt(p);
        /////////////////////////////
        p->next = p->pre = new EntFull; p = p->next;
        m_entity.EntCopy(p, ploop->loop);
        p->angle = ag;
        if (ag < 0) {
            p->gstart = (float)M_PI; p->gend = 0;
        }
        else { 
            p->gstart = (float)M_PI; p->gend = (float)(M_PI*2); 
        }
        m_entity.ParamMakeDir(p); 
        m_entity.ParamMakeExt(p);
        p->next = p->pre = ploop->loop;
        ploop->next = ploop->inner = NULL;
        ploop->xmax = p->xc + p->r; ploop->xmin = p->xc - p->r;
        ploop->ymax = p->yc + p->r; ploop->ymin = p->yc - p->r;
        return ploop;
    }
    else 
        return NULL;
}

void Loop::LoopTreeGetExt(ELoop* ploop) {
    if (ploop == NULL)return;
    LoopGetExt(ploop);
    if (ploop->next)LoopTreeGetExt(ploop->next);
    if (ploop->inner)LoopTreeGetExt(ploop->inner);
}

double Loop::ax = 0.0;
double Loop::bx = 0.0;

bool Loop::xpCompare(const EXPoint xp1, const EXPoint xp2) {
    double dx = ax*(xp1.x - xp2.x) + bx*(xp1.y - xp2.y);
    if (ZERO(dx) || dx < 0)
        return false;
    return true;
}

void Loop::LoopToParellelNC(ELoop* ploop, double angle, float d, float lenmin, vector<NcEnt>& ncar) {
    double cmin, cmax, c;
    double xs, ys, xe, ye;
    double a, b;
    BOOL forward = TRUE;
    size_t size; 
    NcEnt e; e.iscircle = 0; e.reserved = 0;
    vector<EXPoint> xps;
    a = -sin(angle); b = cos(angle);
    cmin = cmax = -a*ploop->xmin - b*ploop->ymin;
    c = -a*ploop->xmin - b*ploop->ymax;
    if (c < cmin)
        cmin = c; 
    else 
        cmax = c;
    c = -a*ploop->xmax - b*ploop->ymin;
    if (c < cmin)
        cmin = c; 
    else if (c > cmax)
        cmax = c;
    c = -a*ploop->xmax - b*ploop->ymax;
    if (c < cmin)
        cmin = c; 
    else if (c > cmax)
        cmax = c;
    cmax -= d / 2; 
    c = cmax;
    while (c >= cmin) {
        NcXLineLoop(a, b, c, angle, ploop, &xps);
        if (forward) { 
            ax = b; bx = -a; 
        } else { 
            ax = -b; bx = a; 
        }
        size = xps.size();
        if (size > 1) {
            sort(xps.begin(), xps.end(), Loop::xpCompare);
            for (int i = 0; i < (int)(size - 1); i++) {
                if (fabs(ax*(xps[i].x - xps[i + 1].x) + bx*(xps[i].y - xps[i + 1].y))<lenmin) {
                    xps.erase(xps.begin() + i, xps.begin() + i + 2);
                    size -= 2;
                    i--;
                }
            }
        }
        if (size>1) {
            if ((size & 0x01) != 0) {
                ERROR_MSG("LoopToParellelNC error\n");
                return;
            }
            if (forward) {
                xs = xps[0].x - b*d; ys = xps[0].y + a*d;
                xe = xps[size - 1].x + b*d; ye = xps[size - 1].y - a*d;
            } else {
                xs = xps[0].x + b*d; ys = xps[0].y - a*d;
                xe = xps[size - 1].x - b*d; ye = xps[size - 1].y + a*d;
            }
            e.on_contour = FALSE; e.ms = FALSE;
            e.type = NCG00; e.x = (float)xs; e.y = (float)ys; ncar.push_back(e);
            for (int i = 0; i < (int)size; i++) {
                if (e.ms)
                    e.type = NCLINE;
                else
                    e.type = NCG00;
                e.x = xps[i].x; e.y = xps[i].y;
                ncar.push_back(e);
                e.ms = !e.ms;
            }
            e.type = NCG00;
            e.ms = 0; e.x = (float)xe; e.y = (float)ye; ncar.push_back(e);
        }
        c -= d;
        forward = !forward;
        xps.clear();
    }
}

void Loop::NcXLineLoop(double a, double b, double c, double angle, ELoop* ploop, vector<EXPoint> *pxps) {
    EntFull*pEnt = ploop->loop;
    EntFull*pHead = NULL, *pn = pEnt;
    int p, p1, p2; EXPoint xp1, xp2;
    do {
        if ((pEnt->flag&ENT_ISLINE) && ((p = m_entity.XLineLine(a, b, c, pEnt, xp1.x, xp1.y)) == PTUNKNOW)) {
            pEnt = pEnt->pre;
            continue;
        }
        pHead = pEnt;
        break;
    } while (pEnt != ploop->loop);
    if (pHead == NULL)
        return;
    if (pEnt->flag&ENT_ISCIRCLE) {
        p = m_entity.XLineCircle(pEnt, a, b, c, xp1.x, xp1.y, xp2.x, xp2.y);
        if (p == 2) { 
            pxps->push_back(xp1); pxps->push_back(xp2); 
        }
    }
    else {
        do {
            if (pEnt->flag&ENT_ISLINE) {
                p = m_entity.XLineLine(a, b, c, pEnt, xp1.x, xp1.y);
                if (p == PTINCURVE || p == PTONEND) {
                    pxps->push_back(xp1); pEnt = pEnt->next;
                }
                else if (p == PTUNKNOW) {
                    pn = pEnt->next; p = -1;
                    while ((pn->flag&ENT_ISLINE) &&
                        ((p = m_entity.XLineLine(a, b, c, pn, xp2.x, xp2.y)) == PTUNKNOW)) {
                        pn = pn->next;
                    }
                    if (!ZERO(a*pn->xstart + b*pn->ystart + c)) {
                        ERROR_MSG("NcXLineLoop error\n");
                        return;
                    }
                    p1 = EntOnLineStatus(a, b, c, angle, pEnt->pre, true);
                    p2 = EntOnLineStatus(a, b, c, angle, pn, false);
                    if (p1*p2 == 0) {
                        ERROR_MSG("NcXLineLoop error 2\n");
                        return;
                    }
                    if (p1*p2<0)
                        pxps->push_back(xp1);
                    pEnt = pn;
                }
                else if (p == PTONSTART) {
                    p1 = EntOnLineStatus(a, b, c, angle, pEnt->pre, true);
                    p2 = EntOnLineStatus(a, b, c, angle, pEnt, false);
                    if (p1*p2 >= 0)
                        pxps->push_back(xp1);
                    pEnt = pEnt->next;
                }
                else pEnt = pEnt->next;
            }
            else {
                float ag1, ag2;
                p = m_entity.XLineArc(a, b, c, pEnt, xp1.x, xp1.y, ag1, xp2.x, xp2.y, ag2, TRUE);
                p1 = p & 0xffff;
                if (p1) {
                    if ((p1&PTONEXTRIM) && (p1&PTINCURVE)) {
                    }
                    else if ((p1 & 0xff) == PTONSTART) {
                        p1 = EntOnLineStatus(a, b, c, angle, pEnt->pre, true);
                        p2 = EntOnLineStatus(a, b, c, angle, pEnt, false);
                        if (p1*p2 >= 0)
                            pxps->push_back(xp1);
                    }
                    else	
                        pxps->push_back(xp1);
                }
                p2 = p >> 16;
                if (p2) {
                    if (p2 == PTONSTART) {
                        p1 = EntOnLineStatus(a, b, c, angle, pEnt->pre, true);
                        p2 = EntOnLineStatus(a, b, c, angle, pEnt, false);
                        if (p1*p2 >= 0)
                            pxps->push_back(xp2);
                    }
                    else 
                        pxps->push_back(xp2);
                }
                pEnt = pEnt->next;
            }
        } while (pEnt != pHead);
    }
    ploop = ploop->inner;
    while (ploop) {
        NcXLineLoop(a, b, c, angle, ploop, pxps);
        ploop = ploop->next;
    }
}

int Loop::EntOnLineStatus(double a, double b, double c, double angle, EntFull* p, bool end) {
    if (p->flag & ENT_ISLINE) {
        double xp;
        if (end)
            xp = a*p->xstart + b*p->ystart + c;
        else
            xp = a*p->xend + b*p->yend + c;
        if (ZERO(xp / 10))
            return 0;
        else if (xp > 0)
            return 1;
        else
            return -1;
    } else {
        float g;
        if (end)
            g = p->gout;
        else
            g = p->gin;
        m_entity.CenterAngle(g, (float)angle);
        if (AEQU(angle, g) || AEQU(angle, g + M_PI)) {
            double xp = a*p->xc + b*p->yc + c;
            if (ZERO(xp)) {
                ERROR_MSG("EntOnLineStatus error\n");
                return 0;
            }
            if (xp > 0)
                return 1;
            else
                return -1;
        }
        else {
            if (g < angle)
                return 1;
            else
                return -1;
        }

    }
}

void Loop::LoopContourToNC(ELoop*ploop, float r, vector<NcEnt>& ncar) {
    if (!ploop || !ploop->loop) {
        ERROR_MSG("LoopContourToNC error\n");
        return;
    }
    EntFull*pEnt = ploop->loop;
    NcEnt e;
    e.on_contour = FALSE; e.ms = FALSE; e.iscircle = 0; e.reserved = 0;
    e.type = NCG00;
    if (pEnt->flag&ENT_ISCIRCLE) {
        ///以最右点为起点的方式， 两条指令
        e.x = pEnt->xc + pEnt->r; e.y = pEnt->yc; ncar.push_back(e);///////
        e.ms = TRUE; e.on_contour = TRUE; e.iscircle = FALSE;

        e.type = pEnt->angle>0 ? NCARC_CCW : NCARC_CW;
        e.x = pEnt->xc - pEnt->r; e.y = pEnt->yc;///////
        ncar.push_back(e);
        e.type = NCARC_CENTER;
        e.x = pEnt->xc; e.y = pEnt->yc;
        ncar.push_back(e);

        e.type = pEnt->angle>0 ? NCARC_CCW : NCARC_CW;
        e.x = pEnt->xc + pEnt->r; e.y = pEnt->yc;///////
        ncar.push_back(e);
        e.type = NCARC_CENTER;
        e.x = pEnt->xc; e.y = pEnt->yc;
        ncar.push_back(e);
    } else {
        e.x = pEnt->xstart; e.y = pEnt->ystart;
        ncar.push_back(e); e.ms = TRUE; e.on_contour = TRUE;
        do {
            e.x = pEnt->xend; e.y = pEnt->yend;
            if (pEnt->flag&ENT_ISLINE) {
                e.type = NCLINE;
                ncar.push_back(e);
            } else {
                if (pEnt->angle > 0)
                    e.type = NCARC_CCW;
                else
                    e.type = NCARC_CW;
                ncar.push_back(e);
                e.type = NCARC_CENTER;
                e.x = pEnt->xc; e.y = pEnt->yc;
                ncar.push_back(e);
            }
            pEnt = pEnt->next;
        } while (pEnt != ploop->loop);
    }
    ploop = ploop->inner;
    while (ploop) {
        LoopContourToNC(ploop, r, ncar);
        ploop = ploop->next;
    }
}

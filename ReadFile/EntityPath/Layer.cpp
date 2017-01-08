#include "Layer.h"

#include <Windows.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "PolyLine.h"

using namespace std;

void Layer::BuildLayer(ELayer*pLayer, float cncAngle, float z) {
    ///////////////////////原始图形
    pLayer->z = z * m_fScalez;
    pLayer->pLoopOriginalBTree = BuildLayerLoopOriginal(pLayer);
    if (pLayer->pLoopOriginalBTree)
        m_cLoop.LoopArrangeDirection(pLayer->pLoopOriginalBTree);
    pLayer->pLoopOriginalTree = m_cLoop.LoopTreeCopyNoEnt(pLayer->pLoopOriginalBTree);
    m_cLoop.BuildLoopTreeFromBTree(pLayer->pLoopOriginalTree);
    ////////////////////////极限图形
    pLayer->pLoopExtremeTree = m_cLoop.LoopTreeCopy(pLayer->pLoopOriginalBTree, m_fScale);
    //////////偏移环上的实体
    pLayer->pLoopOffsetBTree = m_cLoop.BuildLayerLoopOffset(pLayer->pLoopExtremeTree, m_fOffsetDist, TRUE);
    //////检查环上的自交情况
    if (m_bCheckSelfXAfterOffset && pLayer->pLoopOffsetBTree)
        m_cLoop.LoopSfxCheck(pLayer->pLoopOffsetBTree, TRUE);
    else 
        m_cLoop.LoopTreeGetExt(pLayer->pLoopOffsetBTree);
    pLayer->pLoopOffsetTree = m_cLoop.LoopTreeCopyNoEnt(pLayer->pLoopOffsetBTree);
    m_cLoop.BuildLoopTreeFromBTree(pLayer->pLoopExtremeTree);
    m_cLoop.BuildLoopTreeFromBTree(pLayer->pLoopOffsetTree);
    /////生成扫描路径
    BuildLayerNC(pLayer, cncAngle, m_fCncLineDist, m_fCncLenMin, m_fOffsetDist, m_bCncOrder, m_bCncLineCross);
    pLayer->pPolygonsExtreme = pLayer->pPolygonsOriginal = NULL;
    pLayer->iPolygonsExtreme = pLayer->iPolygonsOriginal = 0;
}

ELoop* Layer::BuildLayerLoopOriginal(ELayer*pLayer) {
    int i, LoopNum = 0;
    ELoop *pELoopTree = NULL; EntFull *pEf = NULL;
    //////计算该层的线条的起始
    ////////生成环/\/\/\/\\/\/\/\/\\/\/\/\\\/\/\/
    vector<EPolyLine*> PlArray, LoopArray;
    vector<ELoop*>ELoopArray;
    EPolyLine* pEpl;
    int j, appendok = 0;
    for (i = 0; i<pLayer->iEntNum; i++) {
        ///////生成该层的环元素
        pEf = new EntFull;
        memset(pEf, 0, sizeof(EntFull));
        m_cLoop.m_entity.EntGetFull(pEf, pLayer->pEntRaw + i);
        pEpl = new EPolyLine(pEf);
        if (pEf->flag == ENT_ISCIRCLE) {
            LoopArray.push_back(pEpl);
            continue;
        }
        else
        {
            appendok = 0;
            do {
                for (j = 0; j<PlArray.size(); j++) {
                    if (appendok = PlArray[j]->Append(pEpl))
                        break;
                }
                if (appendok == 0)
                    PlArray.push_back(pEpl);
                else if (appendok == 2) {
                    pEpl = PlArray[j];
                    PlArray.erase(PlArray.begin() + j);
                    LoopArray.push_back(pEpl);
                }
                else if (appendok == 1) {
                    if (PlArray.size() <= 1)break;
                    pEpl = PlArray[j];
                    PlArray.erase(PlArray.begin() + j);
                }
                else if (appendok == 3) {
                    //				ASSERT(FALSE);
                    LoopArray.push_back(pEpl);
                }
            } while (appendok == 1 && PlArray.size());
        }
    }

    if (PlArray.size() > 0)
        for (i = 0; i < PlArray.size(); i++)
            delete PlArray[i];
    LoopNum = (int)LoopArray.size();
    if (LoopNum == 0) {
        return NULL;
    }
    for (i = 0; i<LoopNum; i++) {
        ELoopArray.push_back(m_cLoop.EGetLoop(LoopArray[i]->pLoop, LoopArray[i]->pRear));
        delete LoopArray[i];
    }
    LoopArray.clear();
    ////\/\/\/\\/\\/生成环结束 \/\/\/\/\\/\/\\/\/\/\\/\/
    //////////////处理环的内外关系\\\\\\\\\\\\\\\\\\\\\\/
    pELoopTree = m_cLoop.BuildLoopBTree(ELoopArray);
    ELoopArray.clear();
    //////////////处理环的内外关系结束\\\\\\\\\\\\\\\\\\/
    return pELoopTree;
}

void Layer::BuildLayerNC(ELayer* pLayer, double angle, float d, float lenmin, float r, int cncOrder, int linecross) {
    if (pLayer->pLoopOffsetBTree == NULL || pLayer->pLoopOffsetBTree->loop == NULL) {
        pLayer->iNcEntNum = 0; pLayer->pNcEnt = NULL;
        return;
    }
    if (!(angle >= 0 && angle < M_PI)) {
        ERROR_MSG("BuildLayerNC error\n");
        return;
    }
    vector<NcEnt>ncar;
    ELoop*ploop = pLayer->pLoopOffsetBTree;
    do {
        if ((cncOrder & 0x3) == 2)
        {
            m_cLoop.LoopToParellelNC(ploop, angle, d, lenmin, ncar);
            if (linecross)
                m_cLoop.LoopToParellelNC(ploop, angle + M_PI_2, d, lenmin, ncar);
        }
        else if ((cncOrder & 0x03) == 1)
            m_cLoop.LoopContourToNC(ploop, r, ncar);
        if ((cncOrder & 0x0c) == 0x08)
        {
            m_cLoop.LoopToParellelNC(ploop, angle, d, lenmin, ncar);
            if (linecross)
                m_cLoop.LoopToParellelNC(ploop, angle + M_PI_2, d, lenmin, ncar);
        }
        else if ((cncOrder & 0x0c) == 0x04)
            m_cLoop.LoopContourToNC(ploop, r, ncar);
        ploop = ploop->next;
    } while (ploop);
    pLayer->iNcEntNum = (int)ncar.size();
    if (pLayer->iNcEntNum>0) {
        pLayer->pNcEnt = new NcEnt[pLayer->iNcEntNum];
        memcpy(pLayer->pNcEnt, ncar.data(), pLayer->iNcEntNum*sizeof(NcEnt));
    }
}


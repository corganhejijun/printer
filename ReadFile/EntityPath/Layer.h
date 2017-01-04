#ifndef __LAYER_H__
#define __LAYER_H__

#include "EntityCommon.h"

#include "Loop.h"

class Layer {
    float m_fScalez;
    float m_fScale;
    float m_fOffsetDist;
    long m_bCheckSelfXAfterOffset;
    float m_fCncLineDist;
    float m_fCncLenMin;
    long m_bCncOrder;
    long m_bCncLineCross;

public:
    Layer() {
        m_fScalez = m_fScale = 1.0f;
        m_fOffsetDist = 5.0f;
        m_bCheckSelfXAfterOffset = 0;
        m_fCncLenMin = m_fCncLineDist = 10;
        m_bCncOrder = 6;
        m_bCncLineCross = 0;
    }
    Loop m_cLoop;
    ELoop* BuildLayerLoopOriginal(ELayer* pLayer);
    void BuildLayer(ELayer*pLayer, float cncAngle = 0, float z = 0);
    void BuildLayerNC(ELayer* pLayer, double angle, float d, float lenmin, float r, int cncOrder, int linecross);

};
#endif
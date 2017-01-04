#include "readFile.h"

#define _USE_MATH_DEFINES
#include <math.h>

int Reader::readDxfFile(char* szFile, ESpace *pEntSpace){
    ////初始化内存和文件 
    vector<EntRaw> era;
    ifstream dxfFile(szFile);
    if (!dxfFile || dxfFile.bad()) 
        return -1;
    string dxfCode = "", dxfValue = "";//声明并初始化组码及其对应值
    bool max, min, enti = false;//设置完成标记
    bool entiRead = false;
    while (true) {
        if (dxfCode == "  0" && entiRead) 
            entiRead = false;
        else {
            if (!(getline(dxfFile, dxfCode) && getline(dxfFile, dxfValue)))
                break;
        }
        //读取图幅大小
        if (enti) {
            if (dxfValue == "ARC")
                ReadArc(dxfFile, era, dxfValue, pEntSpace);
            else if (dxfValue == "CIRCLE") 
                ReadCircle(dxfFile, era, dxfValue, pEntSpace);
            else if (dxfValue == "LINE") 
                ReadLine(dxfFile, era, dxfValue, pEntSpace);
            entiRead = true;
        }
        if ((dxfCode == "  9") && (dxfValue == "$EXTMIN")) {
            min = true;
            ReadSize(dxfFile, pEntSpace, true);
        }
        if ((dxfCode == "  9") && (dxfValue == "$EXTMAX")) {
            max = true;
            ReadSize(dxfFile, pEntSpace, false);
        }
        //读取实体信息
        if ((dxfCode == "  2") && (dxfValue == "ENTITIES")) 
            enti = true;
        if (dxfValue == "ENDSEC" && max && min && enti) 
            break;   // 实体读取完毕，其它不再读取
    }
    dxfFile.close();
    if (pEntSpace->EntNum > 0) {
        Iar.push_back(pEntSpace->EntNum - 1);
        pEntSpace->LayerNum++;
        pEntSpace->pEntRaw = new EntRaw[pEntSpace->EntNum];
        memset(pEntSpace->pEntRaw, 0, sizeof(EntRaw) * pEntSpace->EntNum);
        memcpy(pEntSpace->pEntRaw, era.data(), pEntSpace->EntNum*sizeof(EntRaw));
        ReadArrange(pEntSpace, &Zar, &Iar);
    }
    era.clear();
    Iar.clear();
    Zar.clear();
    return 0;
}

//////////读取图幅的信息
void Reader::ReadSize(ifstream& dxfFile, ESpace *pEntSpace, bool min)
{
    string dxfCode, dxfValue = "";
    for (int i = 0; i < 3; i++) {
        getline(dxfFile, dxfCode); getline(dxfFile, dxfValue);
        if (dxfCode == " 10")
            min ? pEntSpace->xmin = stof(dxfValue) : pEntSpace->xmax = stof(dxfValue);
        if (dxfCode == " 20")
            min ? pEntSpace->ymin = stof(dxfValue) : pEntSpace->ymax = stof(dxfValue);
    }
}

////////读取圆弧的信息
void Reader::ReadArc(ifstream& dxfFile, vector<EntRaw>& era, string&dxfValue, ESpace *pEntSpace) {
    float Lh, normal = 0, sa, ea = 0; //层高, 法线方向, 起始角, 终止角
    int code; 
    EntRaw et;
    memset(&et, 0, sizeof(EntRaw));
    string dxfCode = "";
    //提取圆弧数据
    while (dxfCode != "  0") {
        getline(dxfFile, dxfCode);
        getline(dxfFile, dxfValue);
        code = stoi(dxfCode);
        switch (code) {
            case 10:et.xc = stof(dxfValue); break;
            case 20:et.yc = stof(dxfValue); break;
            case 30:Lh = stof(dxfValue); break;
            case 40:et.r = stof(dxfValue); break;
            case 50:sa = stof(dxfValue); break;
            case 51:ea = stof(dxfValue); break;
            case 230:normal = stof(dxfValue); break;
        }
    }
    if (normal == -1.0) {
        et.xc = -et.xc; Lh = -Lh; 
        float t = sa; 
        sa = -ea - 180; ea = -t + 180;
        if ((ea - sa) > 360)
            sa += 360;
    }
    //计算圆弧取值范围，sa和ea均在-2*Pi到2*Pi范围内
    if (ea <= sa)
        ea = ea + 360;
    if (ea > 360) { 
        ea -= 360; sa -= 360; 
    }
    if (ea < 0 && sa < 0) {
        ea += 360; sa += 360; 
    }
    sa = (float)(sa * M_PI / 180); //角度弧度换算 
    ea = (float)(ea * M_PI / 180);
    et.angle = ea - sa; //计算圆弧所占范围
    et.sa = sa;
    ReadEntAdd(pEntSpace, &Zar, &Iar, &era, &et, Lh);
}

////////读取圆的信息
void Reader::ReadCircle(ifstream& dxfFile, vector<EntRaw>& era, string&dxfValue, ESpace *pEntSpace) {
    float Lh, normal = 0;//层高, 法线方向
    int code; 
    EntRaw et;
    memset(&et, 0, sizeof(EntRaw));
    string dxfCode = "";
    while (!(dxfCode == "  0"))
    {
        getline(dxfFile, dxfCode);
        getline(dxfFile, dxfValue);
        code = stoi(dxfCode);
        switch (code) {
            case 10:et.xc = stof(dxfValue); break;
            case 20:et.yc = stof(dxfValue); break;
            case 30:Lh = stof(dxfValue); break;
            case 40:et.r = stof(dxfValue); break;
            case 230:normal = stof(dxfValue); break;
        }
    }
    if (normal == -1.0) { et.xc = -et.xc; Lh = -Lh; }
    et.angle = (float)((double)M_PI * 2);
    et.sa = 0;	
    ReadEntAdd(pEntSpace, &Zar, &Iar, &era, &et, Lh);
}

/////////读取直线的信息
void Reader::ReadLine(ifstream& dxfFile, vector<EntRaw>& era, string&dxfValue, ESpace *pEntSpace) {
    float Lh;//层高
    int code; 
    EntRaw et;
    memset(&et, 0, sizeof(EntRaw));
    string dxfCode = "";
    et.bulge = 0;
    while (!(dxfCode == "  0"))
    {
        getline(dxfFile, dxfCode);
        getline(dxfFile, dxfValue);
        code = stoi(dxfCode);
        switch (code)
        {
        case 10:et.xstart = stof(dxfValue); break;
        case 20:et.ystart = stof(dxfValue); break;
        case 30:Lh = stof(dxfValue); break;
        case 11:et.xend = stof(dxfValue); break;
        case 21:et.yend = stof(dxfValue); break;
        }
    }
    ReadEntAdd(pEntSpace, &Zar, &Iar, &era, &et, Lh);
}

void Reader::ReadEntAdd(ESpace *pEntSpace, vector<float>*pZar, vector<int>*pIar, vector<EntRaw>*pear, EntRaw*pe, float Lz) {
    if (pEntSpace->EntNum == 0) {
        pZar->push_back(Lz);
        pear->push_back(*pe);
    } else {
        if (EQU(pZar->at(pEntSpace->LayerNum), Lz)) {
            pear->push_back(*pe);
        } else {
            int i = pEntSpace->LayerNum - 1;
            for (; i >= 0; i--) { 
                if (EQU(pZar->at(i), Lz)) 
                    break; 
            }
            if (i >= 0) {
                int inspos = pIar->at(i) + 1;
                pear->insert(pear->begin() + inspos, *pe);
                for (; i<pEntSpace->LayerNum; i++)(*pIar)[i]++;
            } else {
                pear->push_back(*pe);
                pIar->push_back(pEntSpace->EntNum - 1);
                pZar->push_back(Lz);
                pEntSpace->LayerNum++;
            }
        }
    }
    pEntSpace->EntNum++;
}

static int LayerCompare(const void*l1, const void*l2) {
    float dz = ((ELayer*)l1)->z - ((ELayer*)l2)->z;
    if (ZERO(dz)) 
        return 0;
    if (dz < 0) 
        return -1;
    return 1;
}

void Reader::ReadArrange(ESpace *pEntSpace, vector<float>*pZar, vector<int>*pIar) {
    EntRaw*m_pEnts = pEntSpace->pEntRaw;
    float XO = .5f*(pEntSpace->xmax + pEntSpace->xmin);
    float YO = .5f*(pEntSpace->ymax + pEntSpace->ymin);
    ///////坐标偏移
    for (int i = 0; i<pEntSpace->EntNum; i++) {
        m_pEnts[i].xstart = m_pEnts[i].xstart - XO;
        m_pEnts[i].ystart = m_pEnts[i].ystart - YO;
        if (m_pEnts[i].bulge == 0) {
            m_pEnts[i].xend = m_pEnts[i].xend - XO;
            m_pEnts[i].yend = m_pEnts[i].yend - YO;
        }
        else {
            m_pEnts[i].yend = m_pEnts[i].yend;
        }
    }
    pEntSpace->xmin -= XO;	pEntSpace->xmax -= XO;
    pEntSpace->ymin -= YO;	pEntSpace->ymax -= YO;

    //////按层
    pEntSpace->pLayer = new ELayer[pEntSpace->LayerNum];
    memset(pEntSpace->pLayer, 0, sizeof(ELayer) * pEntSpace->LayerNum);
    ELayer*pLayer = pEntSpace->pLayer;
    float z;
    pLayer[0].pEntRaw = pEntSpace->pEntRaw;
    pLayer[0].iEntNum = pIar->at(0) + 1;
    pLayer[0].z = pZar->at(0);
    for (int i = 1; i<pEntSpace->LayerNum; i++) {
        pLayer[i].pEntRaw = pEntSpace->pEntRaw + pIar->at(i - 1) + 1;
        pLayer[i].iEntNum = pIar->at(i) - pIar->at(i - 1);
        pLayer[i].z = pZar->at(i);
    }
    qsort(pLayer, pEntSpace->LayerNum, sizeof(ELayer), LayerCompare);
    pEntSpace->pz = new float[pEntSpace->LayerNum]; z = pLayer[0].z;
    for (int i = 0; i<pEntSpace->LayerNum; i++)
        pEntSpace->pz[i] = pLayer[i].z - z;
    pEntSpace->zmin = 0;
    pEntSpace->zmax = pEntSpace->pz[pEntSpace->LayerNum - 1];
}

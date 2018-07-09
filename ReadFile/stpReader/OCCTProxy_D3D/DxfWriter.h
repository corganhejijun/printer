#pragma once

#include "dl_dxf.h"

class DxfWriter {
    DL_Dxf dxf;
    DL_WriterA* dw;
    int wtochar(char **ppDest, const wchar_t *pSrc);
public:
    DxfWriter(const wchar_t* fileName);
    ~DxfWriter(void);

    void writeHead();
    void writeEnd();

    void writeLine(double startX, double startY, double startZ, double endX, double endY, double endZ);
    void writeCircle(double x, double y, double z, double r, int normal);
    void writeArc(double x, double y, double z, double r, double startAngle, double endAngle, int normal);
};
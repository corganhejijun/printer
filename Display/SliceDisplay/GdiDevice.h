#pragma once
#include <windows.h>
#include "slice.h"

#define PAINT_MARGIN 30
class GdiDevice
{
public:
    GdiDevice(HWND hWnd);
    ~GdiDevice();

    int drawSlice(int count, GetSliceData getSlice);
    int fitScreen(double left, double top, double width, double height);

private:
    HWND m_hWnd;
    double m_scale;
    double m_left;
    double m_top;
    int drawBSpline(HDC hDc, BSpline* bSpline);
    int drawCircle(HDC hDc, Circle* circle);
    int drawLine(HDC hDc, Line* line);
    double coordinateXTrans(double x);
    double coordinateYTrans(double y);
};

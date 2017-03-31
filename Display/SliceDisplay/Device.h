#ifndef __DEVICE_H__
#define __DEVCIE_H__

#include <windows.h>
#include <D2D1.h>// header for Direct2D

#include "slice.h"
#include <vector>

using std::vector;

class SliceDevice{
    struct BoundBox {
        double left;
        double right;
        double top;
        double bottom;
    };
    HWND m_hWnd;
    ID2D1Factory* m_pD2DFactory; // Direct2D factory
    ID2D1HwndRenderTarget* m_pRenderTarget; // Render target
    ID2D1SolidColorBrush* m_pBlackBrush; // A black brush, reflect the line color
    const D2D1::ColorF CLEAR_COLOR = D2D1::ColorF(D2D1::ColorF::White);
    float m_curveWith;
    float m_manuStep;

    int CreateD2DResource();
    int drawCurve(Slice* curve, float lineWidth);
    int drawLine(Line* line, float lineWidth);
    int drawCircle(Circle* circle, float lineWidth);
    int drawBSpline(BSpline* spline, float lineWidth);
    void getBoundBox(BoundBox* box, Slice* slice);
    void setBoundBox(BoundBox* box, double top, double bottom, double left, double right);
    vector<vector<Point>*> getInterSect(Slice* slice, BoundBox boundBox);
    vector<Point>* interSecLine(Line* line, BoundBox bound);
    vector<Point>* interSecCircle(Circle* circle, BoundBox bound);
    vector<Point>* interSecBspline(BSpline* bSplice, BoundBox bound);
public:
    SliceDevice(HWND hWnd);
    ~SliceDevice();
    int init();
    int clearScene();
    int drawSlice(Slice* slice);
    int resizeWindow();
};

#endif
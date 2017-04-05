#ifndef __DEVICE_H__
#define __DEVCIE_H__

#include <windows.h>
#include <D2D1.h>// header for Direct2D

#include "slice.h"
#include <vector>

#define EQU_FLOAT(x, y) (abs((x) - (y)) < 0.001f)

using std::vector;

bool sortInterSects(Point pt1, Point pt2);

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
    float m_sceneScale;
    float m_sceneMargin;
    int CreateD2DResource();
    int drawCurve(Slice* curve);
    int drawLine(Line* line);
    int drawCircle(Circle* circle);
    int drawBSpline(BSpline* spline);
    void getBoundBox(BoundBox* box, Slice* slice);
    void getBSplineBoundBox(BoundBox* box, BSpline* spline);
    void setBoundBox(BoundBox* box, double top, double bottom, double left, double right);
    vector<Point>* getInterSect(Slice* slice, BoundBox boundBox);
    double interSec2Point(bool* noInter, double x, Point pt1, Point pt2);
    void interSecLine(vector<Point>* list, Line* line, BoundBox bound);
    void interSecCircle(vector<Point>* list, Circle* circle, BoundBox bound);
    void interSecBspline(vector<Point>* list, BSpline* bSplice, BoundBox bound);
    int drawInterSec(vector<Point>* list);
public:
    SliceDevice(HWND hWnd);
    ~SliceDevice();
    int init();
    void resetScene();
    int clearScene();
    int drawSlice(Slice* slice);
    int resizeWindow();
};

#endif
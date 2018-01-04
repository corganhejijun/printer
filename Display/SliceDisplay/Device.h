#ifndef __DEVICE_H__
#define __DEVCIE_H__

#include <windows.h>
#include <D2D1.h>// header for Direct2D

#include "slice.h"
#include <vector>

#define EQU_FLOAT(x, y) (abs((x) - (y)) < 0.001f)

using std::vector;

bool sortXInterSects(Point pt1, Point pt2);
bool sortYInterSects(Point pt1, Point pt2);

class SliceDevice{
    HWND m_hWnd;
    ID2D1Factory* m_pD2DFactory; // Direct2D factory
    ID2D1HwndRenderTarget* m_pRenderTarget; // Render target
    ID2D1SolidColorBrush* m_pBlackBrush; // A black brush, reflect the line color
    const D2D1::ColorF CLEAR_COLOR = D2D1::ColorF(D2D1::ColorF::White);
    float m_curveWith;
    float m_manuStepX;
    float m_manuStepY;
    float m_sceneScale;
    float m_sceneMargin;
    float m_precise;
    double getLength(Point pt1, Point pt2);
    int CreateD2DResource();
    int drawLine(Line* line);
    int drawCircle(Circle* circle);
    int drawBSpline(BSpline* spline);
    void getBoundBox(BoundBox* box, EdgeType* type, void* slice, int count);
    void getBSplineBoundBox(BoundBox* box, BSpline* spline);
    void setBoundBox(BoundBox* box, double top, double bottom, double left, double right);
    void getInterSect(vector<Point>* listX, vector<Point>* listY, EdgeType* type, void* slice, int count, BoundBox boundBox);
    double xInterSec2Point(bool* noInter, double x, Point pt1, Point pt2);
    double yInterSec2Point(bool* noInter, double y, Point pt1, Point pt2);
    void interSecLine(vector<Point>* listX, vector<Point>* listY, Line* line, BoundBox bound);
    bool angleInCircle(double angle, Circle* circle);
    double atan2Pi(double atanValue);
    void interSecCircle(vector<Point>* listX, vector<Point>* listY, Circle* circle, BoundBox bound);
    void interSecBspline(vector<Point>* listX, vector<Point>* listY, BSpline* bSplice, BoundBox bound);
    bool deleteDuplicatePt(vector<Point>* list, vector<Point>::iterator it);
    int drawInterSec(vector<Point>* listX, vector<Point>* listY);
public:
    SliceDevice(HWND hWnd);
    ~SliceDevice();
    int init();
    void resetScene();
    int clearScene();
    int drawSlice(BoundBox* boundBox, int count, GetSliceData getSlice);
    int resizeWindow();
};

#endif
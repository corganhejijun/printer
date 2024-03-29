#include "Device.h"
#include <algorithm>    // std::sort
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

SliceDevice::SliceDevice(HWND hWnd) {
    m_hWnd = hWnd;
    m_pD2DFactory = NULL;
    m_pRenderTarget = NULL;
    m_pBlackBrush = NULL;
    resetScene();
}

SliceDevice::~SliceDevice() {
    if (m_pD2DFactory) {
        m_pD2DFactory->Release();
        m_pD2DFactory = NULL;
    }
    if (m_pRenderTarget) {
        m_pRenderTarget->Release();
        m_pRenderTarget = NULL;
    }
    if (m_pBlackBrush) {
        m_pBlackBrush->Release();
        m_pBlackBrush = NULL;
    }
}

int SliceDevice::init(){
    return CreateD2DResource();
}

int SliceDevice::CreateD2DResource() {
    HRESULT hr;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr))
        return hr;

    RECT rc; // Render area
    // Obtain the size of the drawing area
    GetClientRect(m_hWnd, &rc);

    // Create a Direct2D render target
    hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
            &m_pRenderTarget
        );
    if (FAILED(hr))
        return hr;

    // Create a brush
    hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
    if (FAILED(hr))
        return hr;
    return S_OK;
}

void SliceDevice::resetScene() {
    m_curveWith = 1;
    m_manuStepX = 1;
    m_manuStepY = 1;
    m_sceneScale = 1;
    m_xMove = 0;
    m_yMove = 0;
    m_sceneMargin = 1;
    m_precise = 1.0f;
}

int SliceDevice::clearScene() {
    if (m_pRenderTarget == NULL)
        return E_HANDLE;
    m_pRenderTarget->BeginDraw();
    // Clear background color white
    m_pRenderTarget->Clear(CLEAR_COLOR);
    m_pRenderTarget->EndDraw();
    ValidateRect(m_hWnd, NULL);
    return S_OK;
}

int SliceDevice::setScale(float scale) {
    m_sceneScale = scale;
    return S_OK;
}

int SliceDevice::move(float x, float y) {
    m_xMove = x; m_yMove = y;
    return S_OK;
}

int SliceDevice::fitScreen(float width, float height) {
    RECT rc; // Render area
    GetClientRect(m_hWnd, &rc);
    float rcWidth = (float)(rc.right - rc.left);
    float rcHeight = (float)(rc.bottom - rc.top);
    float xScale = -1;
    if (rcWidth > 0)
        xScale = rcWidth / (width * (1 + m_sceneMargin));
    float yScale = -1;
    if (rcHeight > 0)
        yScale = rcHeight / (height * (1 + m_sceneMargin));
    m_sceneScale = xScale < yScale ? xScale : yScale;
    return S_OK;
}

int SliceDevice::drawSlice(int count, GetSliceData getSlice) {
    if (m_pRenderTarget == NULL)
        return E_HANDLE;
    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    m_pRenderTarget->Clear(CLEAR_COLOR);

	/*
    // 获取网格线与图形的交点列表
    vector<Point>* listX = new vector<Point>;
    vector<Point>* listY = new vector<Point>;
    getInterSect(listX, listY, slice, boundBox);
	*/

    // 通过变换调整图像比例，使图像放大居中
    D2D1_SIZE_F size = m_pRenderTarget->GetSize();
    /*
    float xLength = (float)(boundBox->right - boundBox->left);
    float yLength = (float)(boundBox->bottom - boundBox->top);
    float xScale = -1;
    if (size.width > 0)
        xScale = xLength * (1 + m_sceneMargin) / size.width;
    float yScale = -1;
    if (size.height > 0)
        yScale = yLength * (1 + m_sceneMargin) / size.height;
    m_sceneScale = m_sceneScale > xScale ? m_sceneScale : xScale;
    m_sceneScale = m_sceneScale > yScale ? m_sceneScale : yScale;
    // objectCenter may be not at (0, 0)
    float objCenterX = float((abs(boundBox->left) - abs(boundBox->right)) / 2 / xLength * size.width);
    float objCenterY = float((abs(boundBox->top) - abs(boundBox->bottom)) / 2 / yLength * size.height);
    // Move object center to screen center
    D2D1_MATRIX_3X2_F moveTrans = D2D1::Matrix3x2F::Translation(size.width / 2 + objCenterX, size.height / 2 + objCenterY);
    // 相同的比例保证缩放后不变形
    D2D1_MATRIX_3X2_F scaleTrans = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1 / m_sceneScale, 1 / m_sceneScale), D2D1::Point2F());
    m_pRenderTarget->SetTransform(scaleTrans * moveTrans);
    */
    D2D1_MATRIX_3X2_F scaleTrans = D2D1::Matrix3x2F::Scale(D2D1::SizeF(m_sceneScale, m_sceneScale));
    D2D1_MATRIX_3X2_F moveTrans = D2D1::Matrix3x2F::Translation(size.width / 2 + m_xMove, size.height / 2 + m_yMove);
    m_pRenderTarget->SetTransform(scaleTrans * moveTrans);

    //D2D1_RECT_F rectangle = D2D1::Rect(boundBox.left, boundBox.top, boundBox.right, boundBox.bottom);
    //m_pRenderTarget->DrawRectangle(rectangle, m_pBlackBrush, m_curveWith * m_sceneScale);
    for (int i = 0; i < count; i++){
        int result = S_OK;
        Slice* slice = getSlice(i);
        switch (slice->type) {
            case EdgeType::bSplice:
                result = drawBSpline((BSpline*)(slice->data));
                break;
            case EdgeType::circle:
                result = drawCircle((Circle*)(slice->data));
                break;
            case EdgeType::line:
                result = drawLine((Line*)(slice->data));
                break;
            default:
                printf_s("unknow curve type\n");
        }
        if (result != S_OK)
            return result;
    }
    //drawInterSec(listX, listY);
    m_pRenderTarget->EndDraw();
    return S_OK;
}

bool SliceDevice::deleteDuplicatePt(vector<Point>* list, vector<Point>::iterator it) {
    if ((it + 1) == list->end())
        return false;
    Point pt1 = *it;
    Point pt2 = *(it + 1);
    while (EQU_FLOAT(pt1.x, pt2.x) && EQU_FLOAT(pt1.y, pt2.y)) {
        list->erase(it + 1);
        if ((it + 1) == list->end()) {
            return false;
        }
        pt2 = *(it + 1);
    }
    while ((it + 2) != list->end()) {
        Point pt3 = *(it + 2);
        if (EQU_FLOAT(pt3.x, pt2.x) && EQU_FLOAT(pt3.y, pt2.y))
            list->erase(it + 2);
        else
            break;
    }
    return true;
}

int SliceDevice::drawInterSec(vector<Point>* listX, vector<Point>* listY) {
    for (vector<Point>::iterator it = listX->begin(); it < listX->end(); it += 2) {
        // 不应有重复的点，圆的切线点等奇异点应删除
        if (!deleteDuplicatePt(listX, it))
            break;
        /*
        int add = 2;
        if (EQU_FLOAT(pt1.x, pt2.x) && EQU_FLOAT(pt1.y, pt2.y)) {
            listX->erase(it + 1);
            if ((it + 1) == listX->end())
                break;
            pt2 = *(it + 1);
            add = 1;
        }
        if ((it + add) != listX->end()) {
            Point pt3 = *(it + add);
            if (EQU_FLOAT(pt3.x, pt2.x) && EQU_FLOAT(pt3.y, pt2.y))
                listX->erase(it + 2);
        }
        it++;
        */
        Point pt1 = *it;
        Point pt2 = *(it + 1);
        if (!EQU_FLOAT(pt1.x, pt2.x))
            continue;
        m_pRenderTarget->DrawLine(D2D1::Point2F((float)pt1.x, (float)pt1.y), D2D1::Point2F((float)pt2.x, (float)pt2.y),
            m_pBlackBrush, m_curveWith / m_sceneScale);
    }
    for (vector<Point>::iterator it = listY->begin(); it < listY->end(); it += 2) {
        if (!deleteDuplicatePt(listY, it))
            break;
        Point pt1 = *it;
        Point pt2 = *(it + 1);
        /*
        int add = 2;
        if (EQU_FLOAT(pt1.x, pt2.x) && EQU_FLOAT(pt1.y, pt2.y)) {
            listY->erase(it + 1);
            if ((it + 1) == listY->end())
                break;
            pt2 = *(it + 1);
            add = 1;
        }
        if ((it + add) != listY->end()) {
            Point pt3 = *(it + add);
            if (EQU_FLOAT(pt3.x, pt2.x) && EQU_FLOAT(pt3.y, pt2.y))
                listY->erase(it + 2);
        }
        it++;
        */
        if (!EQU_FLOAT(pt1.y, pt2.y))
            continue;
        m_pRenderTarget->DrawLine(D2D1::Point2F((float)pt1.x, (float)pt1.y), D2D1::Point2F((float)pt2.x, (float)pt2.y),
            m_pBlackBrush, m_curveWith / m_sceneScale);
    }
    return S_OK;
}

void SliceDevice::getInterSect(vector<Point>* listX, vector<Point>* listY, EdgeType* type, void* slice, int count, BoundBox boundBox) {
    for (int i = 0; i < count; i++){
        switch (type[i])
        {
        case EdgeType::bSplice:
            interSecBspline(listX, listY, (BSpline*)((int*)slice + i), boundBox);
            break;
        case EdgeType::circle:
            interSecCircle(listX, listY, (Circle*)((int*)slice + i), boundBox);
            break;
        case EdgeType::line:
            interSecLine(listX, listY, (Line*)((int*)slice + i), boundBox);
            break;
        default: break;
        }
    }
    std::sort(listX->begin(), listX->end(), sortXInterSects);
    std::sort(listY->begin(), listY->end(), sortYInterSects);
    // TODO 应该剔除与轮廓线重合的部分
}

void SliceDevice::interSecLine(vector<Point>* listX, vector<Point>* listY, Line* line, BoundBox boundBox) {
    Point pt;
    pt.x = boundBox.left;
    while (pt.x < boundBox.right) {
        pt.x += m_manuStepX;
        if (EQU_FLOAT(pt.x, boundBox.right) || pt.x > boundBox.right)
            break;
        bool noInter = false;
        pt.y = xInterSec2Point(&noInter, pt.x, line->start, line->end);
        if (noInter)
            continue;
        listX->push_back(pt);
    }
    pt.y = boundBox.top;
    while (pt.y < boundBox.bottom) {
        pt.y += m_manuStepY;
        if (EQU_FLOAT(pt.y, boundBox.bottom) || pt.y > boundBox.right)
            break;
        bool noInter = false;
        pt.x = yInterSec2Point(&noInter, pt.y, line->start, line->end);
        if (noInter)
            continue;
        listY->push_back(pt);
    }
}

double SliceDevice::yInterSec2Point(bool* noInter, double y, Point pt1, Point pt2) {
    if (EQU_FLOAT(y, pt1.y) && EQU_FLOAT(y, pt2.y)) {
        *noInter = true;
        return 0;
    }
    double yMax = pt2.y > pt1.y ? pt2.y : pt1.y;
    double yMin = pt2.y < pt1.y ? pt2.y : pt1.y;
    if ((y < yMin && !EQU_FLOAT(y, yMin)) || (y > yMax && !EQU_FLOAT(y, yMax))) {
        *noInter = true;
        return 0;
    }
    // 直线表达式：x = (y - b) / k
    if (EQU_FLOAT(pt2.x, pt1.x))
        return pt1.x;
    double k = (pt2.y - pt1.y) / (pt2.x - pt1.x);
    double b = pt2.y - k * pt2.x;
    *noInter = false;
    return (y - b) / k;
}

double SliceDevice::xInterSec2Point(bool* noInter, double x, Point pt1, Point pt2){
    if (EQU_FLOAT(x, pt1.x) && EQU_FLOAT(x, pt2.x)) {
        *noInter = true;
        return 0;
    }
    double xMax = pt2.x > pt1.x ? pt2.x : pt1.x;
    double xMin = pt2.x < pt1.x ? pt2.x : pt1.x;
    if ((x < xMin && !EQU_FLOAT(x, xMin)) || (x > xMax && !EQU_FLOAT(x, xMax))){
        *noInter = true;
        return 0;
    }
    // 直线表达式：y = kx + b
    double k = (pt2.y - pt1.y) / (pt2.x - pt1.x);
    double b = pt2.y - k * pt2.x;
    *noInter = false;
    return k * x + b;
}

void SliceDevice::interSecCircle(vector<Point>* listX, vector<Point>* listY, Circle* circle, BoundBox boundBox) {
    Point pt1;
    memset(&pt1, 0, sizeof(Point));
    Point pt2;
    memset(&pt2, 0, sizeof(Point));
    double xMax = circle->center.x + circle->radius;
    double xMin = circle->center.x - circle->radius;
    double xAxis = boundBox.left;
    while (xAxis < boundBox.right) {
        xAxis += m_manuStepX;
        if (xAxis < xMin || xAxis > xMax || EQU_FLOAT(xAxis, boundBox.right) || xAxis > boundBox.right)
            continue;
        // 圆弧表达式：x^2 + y^2 = r^2
        double x = xAxis - circle->center.x;
        double y = sqrt(circle->radius * circle->radius - x * x);
        // 切线点不应作为一个交点
        if (EQU_FLOAT(y, 0))
            continue;
        // 圆弧角度y=0, x>0为0度，顺时针方向由0~2pi，y=0，x<0为180度
        double yAngle1 = atan2Pi(atan2(y, x));
        pt1.x = pt2.x = xAxis;
        if (angleInCircle(yAngle1, circle)) {
            // y轴方向向上为负，向下为正
            pt1.y = circle->center.y + y;
            listX->push_back(pt1);
        }
        double yAngle2 = atan2Pi(atan2(-y, x));
        if (angleInCircle(yAngle2, circle)) {
            pt2.y = circle->center.y - y;
            listX->push_back(pt2);
        }
    }
    double yMax = circle->center.y + circle->radius;
    double yMin = circle->center.y - circle->radius;
    double yAxis = boundBox.top;
    while (yAxis < boundBox.bottom) {
        yAxis += m_manuStepY;
        if (yAxis < yMin || yAxis > yMax || EQU_FLOAT(yAxis, boundBox.bottom) || yAxis > boundBox.bottom)
            continue;
        double y = yAxis - circle->center.y;
        double x = sqrt(circle->radius * circle->radius - y * y);
        if (EQU_FLOAT(x, 0))
            continue;
        double xAngle = atan2Pi(atan2(y, x));
        pt1.y = pt2.y = yAxis;
        if (angleInCircle(xAngle, circle)) {
            // x轴方向左为负，右为正
            pt1.x = circle->center.x + x;
            listY->push_back(pt1);
        }
        xAngle = atan2Pi(atan2(y, -x));
        if (angleInCircle(xAngle, circle)) {
            pt2.x = circle->center.x - x;
            listY->push_back(pt2);
        }
    }
}

double SliceDevice::atan2Pi(double atanValue) {
    if (atanValue < 0)
        return atanValue + 2 * M_PI;
    if (EQU_FLOAT(0, atanValue))
        return 0;
    return atanValue;
}

bool SliceDevice::angleInCircle(double angle, Circle* circle) {
    double startAngle = circle->startAngle;
    double endAngle = circle->endAngle;
    double angleDiff = abs(startAngle - endAngle);
    double yStart = circle->start.y - circle->center.y;
    double xStart = circle->start.x - circle->center.x;
    double yEnd = circle->end.y - circle->center.y;
    double xEnd = circle->end.x - circle->center.x;
    bool overPI = false;
    if (angleDiff > M_PI && !EQU_FLOAT(angleDiff, M_PI))
        overPI = true;

    startAngle = atan2Pi(atan2(yStart, xStart));
    if (EQU_FLOAT(startAngle, 2 * M_PI))
        startAngle = 0;
    // y轴向下为正向
    if (EQU_FLOAT(circle->end.x, circle->start.x) && EQU_FLOAT(circle->end.y, circle->end.y)) {
        endAngle = 2 * M_PI;
        startAngle = 0;
    }
    else
        endAngle = atan2Pi(atan2(yEnd, xEnd));
    if (EQU_FLOAT(endAngle, 0))
        endAngle = 2 * M_PI;
    if ((abs(endAngle - startAngle) < M_PI && overPI) || (abs(endAngle - startAngle) > M_PI && !overPI)) {
        if (EQU_FLOAT(abs(endAngle - startAngle), M_PI) && !overPI)
            ;
        else
            endAngle = 2 * M_PI - endAngle;
    }
    if (endAngle < startAngle) {
        if (((angle > startAngle || EQU_FLOAT(angle, startAngle)) && (angle < 2 * M_PI || EQU_FLOAT(angle, 2 * M_PI)))
            || ((angle > 0 || EQU_FLOAT(angle, 0)) && (angle < endAngle || EQU_FLOAT(angle, endAngle))))
        {
            return true;
        }
    }
    if ((angle < endAngle || EQU_FLOAT(angle, endAngle)) && (angle > startAngle || EQU_FLOAT(angle, startAngle)))
        return true;
    double angle1 = angle;
    if (EQU_FLOAT(angle1, 2 * M_PI) && EQU_FLOAT(startAngle, 0))
        return true;
    if (EQU_FLOAT(angle1, 0) && EQU_FLOAT(endAngle, 2 * M_PI))
        return true;
    return false;
}

double SliceDevice::getLength(Point pt1, Point pt2) {
    double x = pt1.x - pt2.x;
    double y = pt1.y - pt2.y;
    return x*x + y*y;
}

void SliceDevice::interSecBspline(vector<Point>* listX, vector<Point>* listY, BSpline* bSplice, BoundBox boundBox) {
    Point pt;
    memset(&pt, 0, sizeof(Point));
    pt.x = boundBox.left;
    while(pt.x < boundBox.right){
        pt.x += m_manuStepX;
        for (int i = 1; i < bSplice->polesCnt; i++){
            Point pole1 = bSplice->poles[i - 1];
            Point pole2 = bSplice->poles[i];
            bool noInter = false;
            pt.y = xInterSec2Point(&noInter, pt.x, pole1, pole2);
            if (noInter)
                continue;
            // 每个短线段不取最后一点，防止出现大量重复点，但最后一点要计入
            if (EQU_FLOAT(pt.x, pole2.x) && EQU_FLOAT(pt.y, pole2.y) && i != bSplice->polesCnt - 1)
                continue;
            listX->push_back(pt);
        }
    }
    memset(&pt, 0, sizeof(Point));
    pt.y = boundBox.top;
    while (pt.y < boundBox.bottom) {
        pt.y += m_manuStepY;
        for (int i = 1; i < bSplice->polesCnt; i++) {
            Point pole1 = bSplice->poles[i - 1];
            Point pole2 = bSplice->poles[i];
            bool noInter = false;
            pt.x = yInterSec2Point(&noInter, pt.y, pole1, pole2);
            if (noInter)
                continue;
            if (EQU_FLOAT(pt.x, pole2.x) && EQU_FLOAT(pt.y, pole2.y) && i != bSplice->polesCnt - 1)
                continue;
            listY->push_back(pt);
        }
    }
}

bool sortXInterSects(Point pt1, Point pt2){
    if (EQU_FLOAT(pt1.x, pt2.x))
        return pt1.y < pt2.y;
    else
        return pt1.x < pt2.x;
}

bool sortYInterSects(Point pt1, Point pt2) {
    if (EQU_FLOAT(pt1.y, pt2.y))
        return pt1.x < pt2.x;
    else
        return pt1.y < pt2.y;
}

int SliceDevice::drawLine(Line* line) {
    // 屏幕坐标系与绘图坐标系的y轴相反
    m_pRenderTarget->DrawLine(
        D2D1::Point2F((float)line->start.x, -(float)line->start.y),
        D2D1::Point2F((float)line->end.x, -(float)line->end.y),
        m_pBlackBrush, m_curveWith / m_sceneScale);
    return S_OK;
}

int SliceDevice::drawCircle(Circle* circle) {
    // 屏幕坐标系与绘图坐标系的y轴相反
    if (abs(circle->endAngle - circle->startAngle - M_PI * 2) < 0.0001) {
        m_pRenderTarget->DrawEllipse(
            D2D1::Ellipse(D2D1::Point2F((float)circle->center.x, -(float)circle->center.y),
                (float)circle->radius, (float)circle->radius),
                m_pBlackBrush, m_curveWith / m_sceneScale);
        return S_OK;
    }
    ID2D1PathGeometry *pGeometry = NULL;
    ID2D1GeometrySink *pSink = NULL;
    HRESULT hr = m_pD2DFactory->CreatePathGeometry(&pGeometry);
    if (FAILED(hr))
        return hr;
    hr = pGeometry->Open(&pSink);
    if (FAILED(hr)) {
        pGeometry->Release();
        return hr;
    }
    pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
    double beginX = circle->start.x;
    double beginY = -circle->start.y;
    double endX = circle->end.x;
    double endY = -circle->end.y;
    D2D1_ARC_SIZE size = D2D1_ARC_SIZE_SMALL;
    D2D1_SWEEP_DIRECTION direction = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;
    if (circle->endAngle - circle->startAngle > M_PI)
        size = D2D1_ARC_SIZE_LARGE;
    if (circle->endAngle < circle->startAngle)
        direction = D2D1_SWEEP_DIRECTION_CLOCKWISE;
    pSink->BeginFigure(D2D1::Point2F((float)circle->start.x, -(float)circle->start.y), D2D1_FIGURE_BEGIN_HOLLOW);
    pSink->AddArc(D2D1::ArcSegment(
        D2D1::Point2F((float)circle->end.x, -(float)circle->end.y), D2D1::SizeF((float)circle->radius, (float)circle->radius), 0.0,
        direction, size));
    pSink->EndFigure(D2D1_FIGURE_END_OPEN);
    pSink->Close();
    pSink->Release();
    m_pRenderTarget->DrawGeometry(pGeometry, m_pBlackBrush, m_curveWith / m_sceneScale);
    pGeometry->Release();
    return S_OK;
}

int SliceDevice::drawBSpline(BSpline* spline) {
    // 屏幕坐标系与绘图坐标系的y轴相反
    ID2D1PathGeometry *pGeometry = NULL;
    ID2D1GeometrySink *pSink = NULL;
    HRESULT hr = m_pD2DFactory->CreatePathGeometry(&pGeometry);
    if (FAILED(hr))
        return hr;
    hr = pGeometry->Open(&pSink);
    if (FAILED(hr)) {
        pGeometry->Release();
        return hr;
    }
    pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
    D2D1_POINT_2F point = D2D1::Point2F((float)spline->start.x, -(float)spline->start.y);
    pSink->BeginFigure(point, D2D1_FIGURE_BEGIN_HOLLOW);
    D2D1_POINT_2F point2 = point;
    for (int i = 0; i < spline->polesCnt; i++) {
        point2 = D2D1::Point2F((float)spline->poles[i].x, -(float)spline->poles[i].y);
        float x = point2.x - point.x;
        float y = -(point2.y - point.y);
        pSink->AddLine(point2);
        point = point2;
    }
    pSink->AddLine(D2D1::Point2F((float)spline->end.x, -(float)spline->end.y));
    pSink->EndFigure(D2D1_FIGURE_END_OPEN);
    pSink->Close();
    pSink->Release();
    m_pRenderTarget->DrawGeometry(pGeometry, m_pBlackBrush, m_curveWith / m_sceneScale);
    pGeometry->Release();
    return S_OK;
}

int SliceDevice::resizeWindow() {
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    m_pRenderTarget->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
    return S_OK;
}

#include "Device.h"
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

SliceDevice::SliceDevice(HWND hWnd) {
    m_hWnd = hWnd;
    m_pD2DFactory = NULL;
    m_pRenderTarget = NULL;
    m_pBlackBrush = NULL;
    m_curveWith = 1;
    m_manuStep = 5;
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

int SliceDevice::drawSlice(Slice* slice) {
    if (m_pRenderTarget == NULL)
        return E_HANDLE;
    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    m_pRenderTarget->Clear(CLEAR_COLOR);
    BoundBox boundBox;
    boundBox.left = boundBox.top = D2D1::FloatMax();
    boundBox.right = boundBox.bottom = -D2D1::FloatMax();

    getBoundBox(&boundBox, slice);

    vector<vector<Point>*> interSectPoints = getInterSect(slice, boundBox);

    // 通过变换调整图像比例，使图像放大居中
    D2D1_SIZE_F size = m_pRenderTarget->GetSize();
    D2D1_MATRIX_3X2_F moveTrans = D2D1::Matrix3x2F::Translation(size.width / 2 + (float)boundBox.left, size.height / 2 + (float)boundBox.top);
    float margin = 10;
    D2D1_SIZE_F scaleSize = D2D1::SizeF(size.width/((float)(boundBox.right - boundBox.left + margin)), size.height/((float)(boundBox.bottom - boundBox.top + margin)));
    D2D1_MATRIX_3X2_F scaleTrans = D2D1::Matrix3x2F::Scale(scaleSize, D2D1::Point2F());
    float curveWith = m_curveWith / scaleSize.width;
    m_pRenderTarget->SetTransform(scaleTrans * moveTrans);

    //D2D1_RECT_F rectangle = D2D1::Rect(boundBox.left, boundBox.top, boundBox.right, boundBox.bottom);
    //m_pRenderTarget->DrawRectangle(rectangle, m_pBlackBrush, curveWith);
    Slice* curve = slice;
    while (curve != NULL) {
        drawCurve(curve, curveWith);
        curve = curve->next;
    }
    m_pRenderTarget->EndDraw();
    return S_OK;
}

void SliceDevice::getBoundBox(BoundBox* box, Slice* slice) {
    Slice* ss = slice;
    BSpline* b = NULL;
    Line* line = NULL;
    Circle* circle = NULL;
    while (ss != NULL) {
        switch (ss->type)
        {
        case EdgeType::bSplice:
            b = (BSpline*)(ss->data);
            // TODO
            break;
        case EdgeType::line:
            line = (Line*)(ss->data);
            setBoundBox(box, line->start.y, line->end.y, line->start.x, line->end.x);
            break;
        case EdgeType::circle:
            circle = (Circle*)(ss->data);
            setBoundBox(box, circle->center.y - circle->radius, circle->center.y + circle->radius, circle->center.x - circle->radius, circle->center.x + circle->radius);
            break;
        default:
            break;
        }
        ss = ss->next;
    }
}

vector<vector<Point>*> SliceDevice::getInterSect(Slice* slice, BoundBox boundBox) {
    vector<vector<Point>*> list;
    Slice* ss = slice;
    while (ss != NULL) {
        if (ss->data != NULL) {
            switch (ss->type)
            {
            case EdgeType::bSplice:
                list.push_back(interSecBspline((BSpline*)(ss->data), boundBox));
                break;
            case EdgeType::circle:
                list.push_back(interSecCircle((Circle*)(ss->data), boundBox));
                break;
            case EdgeType::line:
                list.push_back(interSecLine((Line*)(ss->data), boundBox));
                break;
            default:
                break;
            }
        }
        ss = ss->next;
    }
    return list;
}

vector<Point>* SliceDevice::interSecLine(Line* line, BoundBox boundBox) {
    vector<Point>* list = new vector<Point>;
    Point pt;
    double xMax = line->end.x > line->start.x ? line->end.x : line->start.x;
    double xMin = line->end.x < line->start.x ? line->end.x : line->start.x;
    // 直线表达式：y = kx + b
    double k = (line->end.y - line->start.y) / (line->end.x - line->start.x);
    double b = line->end.y - k * line->end.x;
    pt.x = boundBox.left;
    while (pt.x < boundBox.right) {
        pt.x += m_manuStep;
        if (pt.x < xMin || pt.x > xMax)
            continue;
        pt.y = k * pt.x + b;
        list->push_back(pt);
    }
    return list;
}

vector<Point>* SliceDevice::interSecCircle(Circle* circle, BoundBox boundBox) {
    vector<Point>* list = new vector<Point>;
    Point pt;
    double xMax = circle->center.x + circle->radius;
    double xMin = circle->center.x - circle->radius;
    pt.x = boundBox.left;
    while (pt.x < boundBox.right) {
        pt.x += m_manuStep;
        if (pt.x < xMin || pt.x > xMax)
            continue;
        // 圆弧表达式：x^2 + y^2 = r^2
        double x = circle->center.x - pt.x;
        double y = sqrt(circle->radius * circle->radius - x * x);
        double yAngle = atan2(y, x) + M_PI;
        if (yAngle < circle->endAngle && yAngle > circle->startAngle) {
            pt.y = pt.y + y;
            list->push_back(pt);
        }
        yAngle = atan2(-y, x) + M_PI;
        if (yAngle < circle->endAngle && yAngle > circle->startAngle) {
            pt.y = pt.y - y;
            list->push_back(pt);
        }
    }
    return list;
}

vector<Point>* SliceDevice::interSecBspline(BSpline* bSplice, BoundBox boundBox) {
    vector<Point>* list = new vector<Point>;
    return list;
}
    
int SliceDevice::drawCurve(Slice* curve, float lineWidth) {
    if (curve->data == NULL)
        return S_OK;
    switch (curve->type) {
    case EdgeType::bSplice:
        return drawBSpline((BSpline*)(curve->data), lineWidth);
        break;
    case EdgeType::circle:
        return drawCircle((Circle*)(curve->data), lineWidth);
        break;
    case EdgeType::line:
        return drawLine((Line*)(curve->data), lineWidth);
        break;
    default:
        printf_s("unknow curve type\n");
    }
    return S_OK;
}

int SliceDevice::drawLine(Line* line, float lineWidth) {
    m_pRenderTarget->DrawLine(D2D1::Point2F(line->start.x, line->start.y), D2D1::Point2F(line->end.x, line->end.y), m_pBlackBrush, lineWidth);
    return S_OK;
}

int SliceDevice::drawCircle(Circle* circle, float lineWidth) {
    m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(circle->center.x, circle->center.y), circle->radius, circle->radius), m_pBlackBrush, lineWidth);
    return S_OK;
}

int SliceDevice::drawBSpline(BSpline* spline, float lineWidth) {
    return S_OK;
}

void SliceDevice::setBoundBox(SliceDevice::BoundBox* box, double top, double bottom, double left, double right) {
    double t = top < bottom ? top : bottom;
    double b = top < bottom ? bottom : top;
    double l = left < right ? left : right;
    double r = left < right ? right : left;
    if (box->left > l)
        box->left = l;
    if (box->right < r)
        box->right = r;
    if (box->top > t)
        box->top = t;
    if (box->bottom < b)
        box->bottom = b;
}

int SliceDevice::resizeWindow() {
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    m_pRenderTarget->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
    return S_OK;
}

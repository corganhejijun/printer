#include <stdio.h>
#include "GdiDevice.h"

GdiDevice::GdiDevice(HWND hWnd) {
    m_hWnd = hWnd;
}

GdiDevice::~GdiDevice() {

}

int GdiDevice::fitScreen(double left, double top, double width, double height) {
    RECT rc; // Render area
    GetClientRect(m_hWnd, &rc);
    float rcWidth = (float)(rc.right - rc.left);
    float rcHeight = (float)(rc.bottom - rc.top);
    double scaleX = (rcWidth - PAINT_MARGIN) / width;
    double scaleY = (rcHeight - PAINT_MARGIN) / height;
    m_scale = scaleX < scaleY ? scaleX : scaleY;
    m_top = top;
    m_left = left;
    return S_OK;
}

double GdiDevice::coordinateXTrans(double x) {
    return (x - m_left) * m_scale + PAINT_MARGIN;
}

double GdiDevice::coordinateYTrans(double y) {
    return (m_top - y) * m_scale + PAINT_MARGIN;
}

int GdiDevice::drawSlice(int count, GetSliceData getSlice) {
    PAINTSTRUCT ps;
    HDC hdc;   // DC(可画图的内存对象) 的句柄
    HPEN hpen; // 画笔
    // 通过窗口句柄获取该窗口的 DC
    hdc = BeginPaint(m_hWnd, &ps);
    // 创建画笔
    hpen = CreatePen(PS_SOLID, 30, RGB(0, 0, 0));
    // DC 选择画笔
    SelectObject(hdc, hpen);
    MoveToEx(hdc, 100, 200, NULL);
    LineTo(hdc, 300, 200);
    LineTo(hdc, 200, 50);
    LineTo(hdc, 100, 200);
    /*
    for (int i = 0; i < count; i++) {
        int result = S_OK;
        Slice* slice = getSlice(i);
        switch (slice->type) {
        case EdgeType::bSplice: {
            BSpline* bs = (BSpline*)(slice->data);
            if (i == 0) {
                double startX = coordinateXTrans(bs->start.x);
                double startY = coordinateYTrans(bs->start.y);
                MoveToEx(hdc, (int)startX, (int)startY, NULL);
            }
            result = drawBSpline(hdc, bs);
        }
            break;
        case EdgeType::circle: {
            Circle * c = (Circle*)(slice->data);
            if (i == 0) {
                double startX = coordinateXTrans(c->start.x);
                double startY = coordinateYTrans(c->start.y);
                MoveToEx(hdc, (int)startX, (int)startY, NULL);
            }
            result = drawCircle(hdc, c);
        }
            break;
        case EdgeType::line: {
            Line * l = (Line*)(slice->data);
            if (i == 0) {
                double startX = coordinateXTrans(l->start.x);
                double startY = coordinateYTrans(l->start.y);
                MoveToEx(hdc, (int)startX, (int)startY, NULL);
            }
            result = drawLine(hdc, l);
        }
            break;
        default:
            printf_s("unknow curve type\n");
        }
    }
    */
    DeleteObject(hpen);
    EndPaint(m_hWnd, &ps);
    return S_OK;
}

int GdiDevice::drawLine(HDC hDc, Line* line) {
    double endX = coordinateXTrans(line->end.x);
    double endY = coordinateYTrans(line->end.y);
    LineTo(hDc, (int)endX, (int)endY);
    return S_OK;
}

int GdiDevice::drawCircle(HDC hDc, Circle* circle) {
    double rectLeft = coordinateXTrans(circle->center.x - circle->radius);
    double rectTop = coordinateYTrans(circle->center.y + circle->radius);
    double rectRight = coordinateXTrans(circle->center.x + circle->radius);
    double rectBottom = coordinateYTrans(circle->center.y - circle->radius);
    double startX = coordinateXTrans(circle->start.x);
    double startY = coordinateYTrans(circle->start.y);
    double endX = coordinateXTrans(circle->end.x);
    double endY = coordinateYTrans(circle->end.y);
    Arc(hDc, (int)rectLeft, (int)rectTop, (int)rectRight, (int)rectBottom, (int)startX, (int)startY, (int)endX, (int)endY);
    return S_OK;
}

int GdiDevice::drawBSpline(HDC hDc, BSpline* bSpline) {
    for (int i = 0; i < bSpline->polesCnt - 1; i++) {
        double endX = coordinateXTrans(bSpline->poles[i].x);
        double endY = coordinateYTrans(bSpline->poles[i].y);
        LineTo(hDc, (int)endX, (int)endY);
    }
    double finalX = coordinateXTrans(bSpline->end.x);
    double finalY = coordinateYTrans(bSpline->end.y);
    LineTo(hDc, (int)finalX, (int)finalY);
    return S_OK;
}
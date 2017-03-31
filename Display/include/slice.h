#ifndef __SLICE_H__
#define __SLICE_H__
#define _EXTERN_C_  extern "C"  _declspec(dllexport)
typedef struct {
    double x;
    double y;
}Point;

typedef struct {
    Point start;
    Point end;
}Line;

typedef struct {
    Point center;
    double startAngle;
    double endAngle;
    double radius;
}Circle;

typedef struct {
    Point start;
    Point end;
    Point* poles;
    int polesCnt;
}BSpline;

enum EdgeType { unknown, line, circle, bSplice };

typedef struct Slice{
    EdgeType type;
    void* data;
    Slice* next;
    Slice* prev;
}Slice;

#endif
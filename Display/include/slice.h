#ifndef __SLICE_H__
#define __SLICE_H__
#define _EXTERN_C_  extern "C"  _declspec(dllexport)

enum EdgeType { unknown, line, circle, bSplice };

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
    Point start;
    Point end;
}Circle;

typedef struct {
    Point start;
    Point end;
    int polesCnt;
    Point* poles;
}BSpline;

typedef struct Slice{
    EdgeType type;
    void* data;
    Slice* next;
}Slice;

struct BoundBox {
    double left;
    double right;
    double top;
    double bottom;
};

typedef Slice*(_stdcall *GetSliceData)(int index);

#endif
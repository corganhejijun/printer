#ifndef __PRINTER_DISPLAY_H__
#define __PRINTER_DISPLAY_H__
#define _EXTERN_C_  extern "C"  _declspec(dllexport)

_EXTERN_C_ void* glInit(void* hWnd);
_EXTERN_C_ int set2DView(void* gl, void* gl2D);
_EXTERN_C_ int paint(void* hWnd, void* param);
_EXTERN_C_ int resize(void* hWnd, void* param);
_EXTERN_C_ int glDispose(void* param);
_EXTERN_C_ int displayDxf(void *param, char* fileName);
_EXTERN_C_ int drawEntity(void* hWnd, void* param);
_EXTERN_C_ int slowDrawEntity(void* hWnd, void* gl, int sleepMillisecond);
_EXTERN_C_ int getLayerCount(void* gl);
_EXTERN_C_ int getEntCount(void* gl, int layerNum);
_EXTERN_C_ int draw1Step(void* hWnd, void* gl, int currentEnt, int currentLayer, int* prevMs, float* prevX, float* prevY);

#endif
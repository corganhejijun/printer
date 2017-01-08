#ifndef __PRINTER_DISPLAY_H__
#define __PRINTER_DISPLAY_H__
#define _EXTERN_C_  extern "C"  _declspec(dllexport)

_EXTERN_C_ void* glInit(void* hWnd);
_EXTERN_C_ int paint(void* hWnd, void* param);
_EXTERN_C_ int resize(void* hWnd, void* param);
_EXTERN_C_ int glDispose(void* param);
_EXTERN_C_ int displayDxf(void *param, char* fileName);
_EXTERN_C_ int drawEntity(void* hWnd, void* param);
_EXTERN_C_ int slowDrawEntity(void* hWnd, void* gl, int sleepMillisecond);

#endif
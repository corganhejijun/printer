#ifndef __SLICE_DISPLAY_H__
#define __SLICE_DISPLAY_H__
#define _EXTERN_C_  extern "C"  _declspec(dllexport)

_EXTERN_C_ void* create2D(void* hWnd);
_EXTERN_C_ int displaySlice(void* device, void** slice, int sliceNum);
_EXTERN_C_ void cleanScreen(void* device);
_EXTERN_C_ void resizeWindow(void* device);
_EXTERN_C_ void delete2DSlice(void** slice, int count);
_EXTERN_C_ void release2D(void* device);

#endif
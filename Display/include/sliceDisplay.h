#ifndef __SLICE_DISPLAY_H__
#define __SLICE_DISPLAY_H__
#define _EXTERN_C_  extern "C"  _declspec(dllexport)

_EXTERN_C_ void* create(void* hWnd);
_EXTERN_C_ int displaySlice(void* device, BoundBox* box, int sliceNum, GetSliceData getSlice);
_EXTERN_C_ void cleanScreen(void* device);
_EXTERN_C_ void resizeWindow(void* device);
_EXTERN_C_ void release(void* device);
_EXTERN_C_ void reset(void* device);

#endif
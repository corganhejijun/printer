#include "slice.h"
#include "sliceDisplay.h"
#include "Device.h"

void* create(void* hWnd){
    SliceDevice* device = new SliceDevice((HWND)hWnd);
    if (FAILED(device->init())) {
        delete device;
        return NULL;
    }
    return device;
}

int displaySlice(void* device, EdgeType* type, void* slice, int sliceNum){
    SliceDevice* cDevice = (SliceDevice*)device;
    if (slice == NULL) {
        cleanScreen(device);
        return S_OK;
    }
    return cDevice->drawSlice(type, slice, sliceNum);
}

void cleanScreen(void* device) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->clearScene();
}

void resizeWindow(void* device) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->resizeWindow();
}

void release(void* device) {
    SliceDevice* cDevice = (SliceDevice*)device;
    delete cDevice;
}

void reset(void* device) {
    SliceDevice* cDevcie = (SliceDevice*)device;
    cDevcie->resetScene();
}

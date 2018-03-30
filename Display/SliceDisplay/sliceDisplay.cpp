#include "slice.h"
#include "sliceDisplay.h"

#include "GdiDevice.h"

void* create(void* hWnd) {
    GdiDevice* device = new GdiDevice((HWND)hWnd);
    return device;
}

int displaySlice(void* device, int sliceNum, GetSliceData getSlice) {
    GdiDevice* cDevice = (GdiDevice*)device;
    return cDevice->drawSlice(sliceNum, getSlice);
}

void fitScreen(void* device, float left, float top, float width, float height) {
    GdiDevice* cDevice = (GdiDevice*)device;
    cDevice->fitScreen(left, top, width, height);
}

void move(void* device, float x, float y) {
    GdiDevice* cDevice = (GdiDevice*)device;
}

void cleanScreen(void* device) {
    GdiDevice* cDevice = (GdiDevice*)device;
}

void release(void* device) {
    GdiDevice* cDevice = (GdiDevice*)device;
    delete cDevice;
}

void reset(void* device) {
    GdiDevice* cDevice = (GdiDevice*)device;
}

/* use Direct2D deprecated

#include "Device.h"

void* create(void* hWnd){
    SliceDevice* device = new SliceDevice((HWND)hWnd);
    if (FAILED(device->init())) {
        delete device;
        return NULL;
    }
    return device;
}

int displaySlice(void* device, int sliceNum, GetSliceData getSlice){
    SliceDevice* cDevice = (SliceDevice*)device;
    return cDevice->drawSlice(sliceNum, getSlice);
}

void fitScreen(void* device, float width, float height) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->fitScreen(width, height);
}

void setScale(void* device, float scale) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->setScale(scale);
}

void move(void* device, float x, float y) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->move(x, y);
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

*/
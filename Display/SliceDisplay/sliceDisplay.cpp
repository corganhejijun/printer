#include "slice.h"
#include "sliceDisplay.h"
#include "Device.h"

void* create2D(void* hWnd){
    SliceDevice* device = new SliceDevice((HWND)hWnd);
    if (FAILED(device->init())) {
        delete device;
        return NULL;
    }
    return device;
}

int displaySlice(void* device, void** slice, int sliceNum){
    SliceDevice* cDevice = (SliceDevice*)device;
    if (slice == NULL) {
        cleanScreen(device);
        return S_OK;
    }
    Slice** sliceArray = (Slice**)slice;
    return cDevice->drawSlice(&(*sliceArray)[sliceNum]);
}

void cleanScreen(void* device) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->clearScene();
}

void resizeWindow(void* device) {
    SliceDevice* cDevice = (SliceDevice*)device;
    cDevice->resizeWindow();
}

void deleteSlice(Slice* slice) {
    if (slice->next != NULL) {
        deleteSlice(slice->next);
        delete slice->next;
        slice->next = NULL;
    }
    if (slice->type != EdgeType::bSplice)
        delete slice->data;
    else 
        delete[] slice->data;
}

void delete2DSlice(void** sl, int count) {
    if (sl == NULL)
        return;
    Slice** slice = (Slice**)sl;
    for (int i = 0; i < count; i++) {
        Slice* oneSlice = &(*slice)[i];
        deleteSlice(oneSlice);
    }
    delete[] (*slice);
}

void release2D(void* device) {
    SliceDevice* cDevice = (SliceDevice*)device;
    delete cDevice;
}

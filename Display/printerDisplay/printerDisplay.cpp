#include "printerDisplay.h"
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "EntityCommon.h"
#include "readFile.h"
#include "Layer.h"
#include "DrawEntity.h"

typedef struct _Global {
    Reader* reader;
    ESpace* eSpace;
    Layer* layer;
    DrawEntity* drawEntity;
}Global;

class GlParam {
public:
    HGLRC hrc;
    COLORREF bkColor;
    float RotateX, RotateY, RotateZ;
    float TransX, TransY, Scale;
    float fCncAngleDelta;
    float fOffsetDist;
    Global global;
    bool is2D;
    GlParam() {
        is2D = false;
        hrc = NULL;
        bkColor = 0xffffff;
        RotateX = RotateY = RotateZ = 0;
        TransX = TransY = 0;
        fCncAngleDelta = (float)M_PI_2;
        fOffsetDist = 5;
        Scale = 1;
    }
};

void* glInit(void* hWnd) {
    int pixelformat; 
    GlParam* glParam = new GlParam();
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
        PFD_TYPE_RGBA,24,0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,16,0,0,
        PFD_MAIN_PLANE,0,0, 0, 0
    };
    HDC hdc = GetDC((HWND)hWnd);
    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0)return NULL;
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)return NULL;
    pixelformat = ::GetPixelFormat(hdc);
    ::DescribePixelFormat(hdc, pixelformat, sizeof(pfd), &pfd);
    glParam->hrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, glParam->hrc);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDrawBuffer(GL_BACK);

    return glParam;
}

int set2DView(void* gl, void* gl2D){
    GlParam* glParam = (GlParam*)gl;
    GlParam* glParam2D = (GlParam*)gl2D;
    glParam2D->global.drawEntity = glParam->global.drawEntity;
    glParam2D->global.eSpace = glParam->global.eSpace;
    glParam2D->global.layer = glParam->global.layer;
    glParam2D->global.reader = glParam->global.reader;
    glParam2D->is2D = true;
    return 0;
}

void displayClear(void* param) {
    GlParam* glParam = (GlParam*)param;
    // ÇåÆÁÄ»
    glClearColor((glParam->bkColor & 0xff) / 255.0f, ((glParam->bkColor & 0xff00) >> 8) / 255.0f,
        (glParam->bkColor >> 16) / 255.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int paint(void* hWnd, void* param) {
    GlParam* glParam = (GlParam*)param;
    wglMakeCurrent(GetDC((HWND)hWnd), glParam->hrc);
    displayClear(glParam);
    // ×ø±ê±ä»»
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(glParam->TransX, glParam->TransY, 0.0f);
    glScalef(glParam->Scale, glParam->Scale, glParam->Scale);
    glRotatef(glParam->RotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(glParam->RotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(glParam->RotateZ, 0.0f, 0.0f, 1.0f);
    glLoadIdentity();
    SwapBuffers(GetDC((HWND)hWnd));
    return 0;
}

int resize(void* hWnd, void* param) {
    GlParam* glParam = (GlParam*)param;
    wglMakeCurrent(GetDC((HWND)hWnd), glParam->hrc);
    RECT rect;
    GetWindowRect((HWND)hWnd, &rect);
    int cx = rect.right - rect.left;
    int cy = rect.bottom - rect.top;
    float sx = 1.0f, sy = 1.0f;
    if (cx < cy)
        sy = ((float)cy) / cx;
    else 
        sx = ((float)cx) / cy;
    glViewport(0, 0, cx, cy);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-sx, sx, -sy, sy, 0.1f, 10.f);
    glTranslatef(0.0f, 0.0f, -5.0f);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    return 0;
}

int glDispose(void* param) {
    GlParam* glParam = (GlParam*)param;
    wglDeleteContext(glParam->hrc);
    if (!glParam->is2D) {
        delete glParam->global.reader;
        delete glParam->global.eSpace;
        delete glParam->global.layer;
        delete glParam->global.drawEntity;
    }
    delete glParam;
    return 0;
}

int displayDxf(void *gl, char* fileName) {
    GlParam* glParam = (GlParam*)gl;
    glParam->global.reader = new Reader();
    glParam->global.eSpace = new ESpace;
    memset(glParam->global.eSpace, 0, sizeof(ESpace));
    glParam->global.layer = new Layer();
    glParam->global.drawEntity = new DrawEntity();
    glParam->global.reader->readDxfFile(fileName, glParam->global.eSpace);
    if (glParam->global.eSpace->LayerNum == 0 || glParam->global.eSpace->EntNum == 0) {
        return FALSE;
    } else {
        glParam->global.layer->BuildLayer(glParam->global.eSpace->pLayer);
        double angle = 0;
        for (int i = 1; i < glParam->global.eSpace->LayerNum; i++) {
            angle += glParam->fCncAngleDelta;
            if (angle >= M_PI) 
                angle -= M_PI;
            else if (angle < 0)
                angle += M_PI;
            glParam->global.layer->BuildLayer(glParam->global.eSpace->pLayer + i, (float)angle, glParam->global.eSpace->pz[i]);
        }
        glParam->RotateX = glParam->RotateY = glParam->RotateZ = 0;
        glParam->TransX = glParam->TransY = 0;
        glParam->Scale = 1;
    }
    return TRUE;
}

void glCordinationTransform(GlParam* glParam)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(glParam->TransX, glParam->TransY, 0.0f);
    glScalef(glParam->Scale, glParam->Scale, glParam->Scale);
    glRotatef(glParam->RotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(glParam->RotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(glParam->RotateZ, 0.0f, 0.0f, 1.0f);
}

void glPreDraw3D(GlParam* glParam, BOOL useZoom = TRUE) {
    glRotatef(-45, 1.0f, .0f, 0.0f);
    glRotatef(30, .0f, .0f, 1.0f);
    float scale = 1.0f;
    float ext = (1.2f)*(sqrt(SQUARE(glParam->global.eSpace->xmax - glParam->global.eSpace->xmin) +
        SQUARE(glParam->global.eSpace->ymax - glParam->global.eSpace->ymin) + SQUARE(glParam->global.eSpace->zmax - glParam->global.eSpace->zmin)));
    if (ext == 0)
        scale = 1;
    else
        scale = 2 / ext;
    if (useZoom)
        scale /= glParam->Scale;
    glTranslatef(.0f, .0f, -.6f);
    glScalef(scale, scale, scale);
}

BOOL glDrawEntOn3dView(GlParam* glParam, void* hWnd, EntFull*pEnt, float z, float zh, BOOL prems, BOOL ms) {
    RECT rect;
    GetClientRect((HWND)hWnd, &rect);
    if (rect.right>0) {
        if (ms) {
            glParam->global.drawEntity->glDrawNcEntSolid(pEnt, glParam->fOffsetDist, z, zh, prems);
            glFlush();
        }
        return FALSE;
    }
    else
        return TRUE;
}

void glPreDraw2D(GlParam* glParam) {
    float scale = 1.0f;
    float xext = 1.1f*(glParam->global.eSpace->xmax - glParam->global.eSpace->xmin);
    float yext = 1.1f*(glParam->global.eSpace->ymax - glParam->global.eSpace->ymin);
    if (xext == 0 || yext == 0)scale = 1.0f;
    else if (xext>yext)scale = 2 / xext;
    else scale = 2 / yext;
    scale /= glParam->Scale;
    glScalef(scale, scale, 1.0f);


}


BOOL glDrawEntOn2dView(GlParam* glParam, void* hWnd, EntFull*pEnt, BOOL ms, BOOL on_contour) {
    RECT rect;
    GetClientRect((HWND)hWnd, &rect);
    if(rect.right>0) {
        glCordinationTransform(glParam);
        glPreDraw2D(glParam);
        if (ms) {
            if (on_contour) {
                glParam->global.drawEntity->glDrawEntFace(pEnt, glParam->fOffsetDist);
            } else {
                glParam->global.drawEntity->glDrawEntFace(pEnt, glParam->fOffsetDist);
            }
        } else {
            glParam->global.drawEntity->glDrawEntLine(pEnt);
        }
        return FALSE;
    }
    return TRUE;
}

int drawEntity(void* hWnd, void* gl) {
    GlParam* glParam = (GlParam*)gl;
    NcEnt*pEnt; 
    EntFull e; 
    memset(&e, 0, sizeof(EntFull));
    float z, zh, Px, Py;
    if (glParam->global.eSpace == NULL)
        return -1;
    wglMakeCurrent(GetDC((HWND)hWnd), glParam->hrc);
    displayClear(glParam);
    glCordinationTransform(glParam);
    glPreDraw3D(glParam);
    for (int iz = 0; iz < glParam->global.eSpace->LayerNum - 1; iz++) {
        BOOL prems = FALSE;
        pEnt = glParam->global.eSpace->pLayer[iz].pNcEnt;
        z = glParam->global.eSpace->pLayer[iz].z;
        zh = glParam->global.eSpace->pLayer[iz + 1].z;
        Px = Py = 0;
        for (int i = 0; i < glParam->global.eSpace->pLayer[iz].iNcEntNum; i++) {
            if (!glParam->global.layer->m_cLoop.m_entity.NcToEntFull(pEnt + i, Px, Py, e))
                continue;
            if (glParam->is2D)
                glDrawEntOn2dView(glParam, hWnd, &e, pEnt[i].ms, pEnt[i].on_contour);
            else
                glDrawEntOn3dView(glParam, hWnd, &e, z, zh, prems, pEnt[i].ms);
            prems = pEnt[i].ms;
        }
    }
    glFinish();
    SwapBuffers(GetDC((HWND)hWnd));
    return 0;
}

int slowDrawEntity(void* hWnd, void* gl, int sleepMillisecond) {
    GlParam* glParam = (GlParam*)gl;
    NcEnt*pEnt;
    EntFull e;
    memset(&e, 0, sizeof(EntFull));
    float z, zh, Px, Py;
    if (glParam->global.eSpace == NULL)
        return -1;
    wglMakeCurrent(GetDC((HWND)hWnd), glParam->hrc);
    displayClear(glParam);
    for (int iz = 0; iz < glParam->global.eSpace->LayerNum - 1; iz++) {
        BOOL prems = FALSE;
        pEnt = glParam->global.eSpace->pLayer[iz].pNcEnt;
        z = glParam->global.eSpace->pLayer[iz].z;
        zh = glParam->global.eSpace->pLayer[iz + 1].z;
        Px = Py = 0;
        for (int i = 0; i < glParam->global.eSpace->pLayer[iz].iNcEntNum; i++) {
            memset(&e, 0, sizeof(EntFull));
            if (!glParam->global.layer->m_cLoop.m_entity.NcToEntFull(pEnt + i, Px, Py, e))
                continue;
            if (glParam->is2D) {
                displayClear(glParam);
                glDrawEntOn2dView(glParam, hWnd, &e, pEnt[i].ms, pEnt[i].on_contour);
                SwapBuffers(GetDC((HWND)hWnd));
            }
            else
                glDrawEntOn3dView(glParam, hWnd, &e, z, zh, prems, pEnt[i].ms);
            prems = pEnt[i].ms;
            glFlush();
            glFinish();
            if (pEnt[i].ms) {
                SwapBuffers(GetDC((HWND)hWnd));
                Sleep(sleepMillisecond);
            }
        }
    }
    return 0;
}

int getLayerCount(void* gl) {
    GlParam* glParam = (GlParam*)gl;
    return glParam->global.eSpace->LayerNum - 1;
}

int getEntCount(void* gl, int layerNum) {
    GlParam* glParam = (GlParam*)gl;
    return glParam->global.eSpace->pLayer[layerNum].iNcEntNum;
}

int draw1Step(void* hWnd, void* gl, int currentEnt, int currentLayer, int* prevMs, float* prevX, float* prevY) {
    GlParam* glParam = (GlParam*)gl;
    NcEnt*pEnt = glParam->global.eSpace->pLayer[currentLayer].pNcEnt;
    EntFull e;
    memset(&e, 0, sizeof(EntFull));
    if (glParam->global.eSpace == NULL)
        return -1;
    wglMakeCurrent(GetDC((HWND)hWnd), glParam->hrc);

    if (!glParam->global.layer->m_cLoop.m_entity.NcToEntFull(pEnt + currentEnt, *prevX, *prevY, e))
        return FALSE;
    if (glParam->is2D) {
        displayClear(glParam);
        glDrawEntOn2dView(glParam, hWnd, &e, pEnt[currentEnt].ms, pEnt[currentEnt].on_contour);
        SwapBuffers(GetDC((HWND)hWnd));
        wglMakeCurrent(NULL, NULL);
    } else {
        if (currentEnt == 0 && currentLayer == 0)
            displayClear(glParam);
        glDrawEntOn3dView(glParam, hWnd, &e, glParam->global.eSpace->pLayer[currentLayer].z, 
            glParam->global.eSpace->pLayer[currentLayer + 1].z, *prevMs, pEnt[currentEnt].ms);
        glFlush();
        glFinish();
        if (pEnt[currentEnt].ms || (currentEnt == 0 && currentLayer == 0)) {
            SwapBuffers(GetDC((HWND)hWnd));
        }
        wglMakeCurrent(NULL, NULL);
    }
    *prevMs = pEnt[currentEnt].ms;
    return TRUE;
}

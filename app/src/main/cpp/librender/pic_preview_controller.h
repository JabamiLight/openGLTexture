//
// Created by 唐宇 on 2018/5/20.
//

#ifndef ANDROIDOPENGLDEMO_PIC_PREVIEW_CONTROLLER_H
#define ANDROIDOPENGLDEMO_PIC_PREVIEW_CONTROLLER_H

#include <android/native_window.h>
#include "pic_preview_render.h"
#include "pic_preview_texture.h"
#include "../libpng/png_decoder.h"
#include "egl_core.h"
#include <unistd.h>


class PicPreviewController {

public:
    PicPreviewController(JNIEnv*env,jobject assetManager,int type);
    virtual ~PicPreviewController();
    bool start(char* spriteFilePath);
    void stop();
    void setWindow(ANativeWindow* window);
    void resetSize(int width, int height);

private:
    int type;
    PicPreviewRender* render;
    PicPreviewTexture* picPreviewTexture;

    PngPicDecoder* decoder;
    int screenWidth;
    int screenHeight;
    enum RenderThreadMessage{
        MSG_NONE=0,MSG_WINDOW_SET,MSG_RENDER_LOOP_EXIT
    };
    pthread_t _threadId;
    pthread_mutex_t mLock;
    pthread_cond_t mCondition;
    enum RenderThreadMessage _msg;

    // android window, supported by NDK r5 and newer
    ANativeWindow* _window;
    EGLCore* eglCore= nullptr;
    EGLSurface previewSurface;

    // Helper method for starting the thread
    static void* threadStartCallback(void *myself);
    // RenderLoop is called in a rendering thread started in start() method
    // It creates rendering context and renders scene until stop() is called
    void renderLoop();
    bool initialize();
    void updateTexImage();
    void drawFrame();
    void destroy();


};



#endif //ANDROIDOPENGLDEMO_PIC_PREVIEW_CONTROLLER_H

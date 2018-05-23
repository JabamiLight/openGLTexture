//
// Created by 唐宇 on 2018/5/20.
//

#include "pic_preview_controller.h"
#include "../libcommon/AssetsReader.h"
#include "../librender/fbo_render.h"

#define LOG_TAG "PicPreviewController"


void *PicPreviewController::threadStartCallback(void *myself) {
    PicPreviewController *controller = (PicPreviewController *) myself;
    controller->renderLoop();
//    pthread_exit(0);
    return nullptr;
}

void PicPreviewController::renderLoop() {
    bool renderingEnabled = true;
    LOGI("renderLoop()");
    while (renderingEnabled) {
        pthread_mutex_lock(&mLock);
        /*process incoming messages*/
        switch (_msg) {
            case MSG_WINDOW_SET:
                initialize();
                break;
            case MSG_RENDER_LOOP_EXIT:
                renderingEnabled = false;
                destroy();
                break;
            default:
                break;
        }
        _msg = MSG_NONE;

        if (eglCore) {
            eglCore->makeCurrent(previewSurface);
            this->drawFrame();
            pthread_cond_wait(&mCondition, &mLock);
            usleep(100 * 1000);
        }

        pthread_mutex_unlock(&mLock);
    }
    LOGI("Render loop exits");

    return;
}

bool PicPreviewController::initialize() {
    eglCore = new EGLCore();
    eglCore->init();
    previewSurface = eglCore->createWindowSurface(_window);
    LOGI("INIT previewsurface %d",previewSurface);
    eglCore->makeCurrent(previewSurface);
    picPreviewTexture = new PicPreviewTexture();
    bool createTexFlag = picPreviewTexture->createTexture();

    if (!createTexFlag) {
        LOGE("createTexFlag is failed...");
        destroy();
        return false;
    }

    updateTexImage();
    bool isRendererInitialized = render->init(screenWidth, screenHeight, picPreviewTexture);
    if (!isRendererInitialized) {
        LOGI("Renderer failed on initialized...");
        return false;
    }
    LOGI("Initializing context Success");
    return true;
}

void PicPreviewController::updateTexImage() {
    if (decoder) {
        const RawImageData raw_image_data = decoder->getRawImageData();
        LOGI("raw_image_data Meta: width is %d height is %d size is %d colorFormat is %d",
             raw_image_data.width, raw_image_data.height, raw_image_data.size,
             raw_image_data.gl_color_format);
        LOGI("colorFormat is %d", GL_RGBA);
        picPreviewTexture->updateTexImage((byte *) raw_image_data.data, raw_image_data.width,
                                          raw_image_data.height);
        if(type==1){
            picPreviewTexture->createFramBuffer();
        }
        decoder->releaseRawImageData(&raw_image_data);
    }

}

void PicPreviewController::drawFrame() {
    render->render();
    if (!eglCore->swapBuffers(previewSurface)) {
        LOGE("eglSwapBuffers() returned error %d", eglGetError());
    }



}

void PicPreviewController::destroy() {
    LOGI("dealloc renderer ...");
    if (nullptr != render) {
        render->dealloc();
        delete render;
        render = NULL;
    }
    if (eglCore) {
        eglCore->releaseSurface(previewSurface);
        eglCore->release();
        eglCore = NULL;
    }
    return;

}


PicPreviewController::~PicPreviewController() {
    LOGI("VideoDutePlayerController instance destroyed");
    pthread_mutex_destroy(&mLock);
    pthread_cond_destroy(&mCondition);
}

bool PicPreviewController::start(char *spriteFilePath) {
    LOGI("Creating VideoDutePlayerController thread");
    /*send message to render thread to stop rendering*/
    decoder = new PngPicDecoder();
    decoder->openFile(spriteFilePath);
    pthread_create(&_threadId, nullptr, threadStartCallback, this);
    return true;
}

void PicPreviewController::stop() {
    LOGI("Stopping VideoDutePlayerController Render thread");
    /*send message to render thread to stop rendering*/
    pthread_mutex_lock(&mLock);
    _msg = MSG_RENDER_LOOP_EXIT;
    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mLock);

    LOGI("we will join render thread stop");
    pthread_join(_threadId, nullptr);
    LOGI("VideoDutePlayerController Render thread stopped");
}

void PicPreviewController::setWindow(ANativeWindow *window) {
    pthread_mutex_lock(&mLock);
    _msg = MSG_WINDOW_SET;
    _window = window;
    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mLock);


}

void PicPreviewController::resetSize(int width, int height) {
    LOGI("VideoDutePlayerController::resetSize width:%d; height:%d", width, height);
    pthread_mutex_lock(&mLock);
    this->screenWidth = width;
    this->screenHeight = height;
    render->resetRenderSize(0, 0, width, height);
    pthread_cond_signal(&mCondition);
    pthread_mutex_unlock(&mLock);
}

PicPreviewController::PicPreviewController(JNIEnv *env, jobject assetManager, int type) {
    LOGI("VideoDutePlayerController instance created");
    pthread_mutex_init(&mLock, nullptr);
    pthread_cond_init(&mCondition, nullptr);
    screenWidth = 720;
    screenHeight = 720;
    this->type=type;
    char *vertexContent = nullptr;
    char *fragContent = nullptr;
    if (type == 0) {
        readSource(env, "texture/vertex_shader.glsl", "texture/fragment_shader.glsl",
                   assetManager, vertexContent,
                   fragContent);
    } else {
        readSource(env, "fbo/vertex_shader.glsl", "fbo/fragment_shader.glsl", assetManager,
                   vertexContent,
                   fragContent);
    }
    if (!vertexContent || !fragContent) {
        LOGI("read source failed");
        return;
    }
    LOGI("glsl content vertex %s", vertexContent);
    LOGI("glsl content frag %s", fragContent);
    if (type == 0) {
        render = new PicPreviewRender(vertexContent, fragContent);
    } else {
        render = new FboRender(vertexContent, fragContent);
    }
    decoder = nullptr;
}

//
// Created by 唐宇 on 2018/5/19.
//

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "PngPreviewController.h"
#include "librender/pic_preview_controller.h"

#define LOG_TAG "PicPreviewController"

static ANativeWindow *window = 0;
static PicPreviewController *controller = 0;

void
Java_com_example_yllds_androidopengldemo_PngPreviewController_init(JNIEnv *env, jobject thiz,
                                                                   jstring picPath,
                                                                   jobject assetManager,jint type) {
    controller = new PicPreviewController(env,assetManager,type);
    char* pngFilePath = (char*) env->GetStringUTFChars(picPath, NULL);
    LOGI("filepath %s",pngFilePath);
    controller->start(pngFilePath);
    env->ReleaseStringUTFChars(picPath, pngFilePath);
}

void Java_com_example_yllds_androidopengldemo_PngPreviewController_setSurface(JNIEnv *env, jobject thiz,
                                                                              jobject surface) {
    if (surface != 0 && NULL != controller) {
        window = ANativeWindow_fromSurface(env, surface);
        LOGI("Got window %p", window);
        controller->setWindow(window);
    } else if (window != 0) {
        LOGI("Releasing window");
        ANativeWindow_release(window);
        window = 0;
    }

}

void
Java_com_example_yllds_androidopengldemo_PngPreviewController_resetSize(JNIEnv *, jobject, jint width,
                                                                        jint height) {
    if (NULL != controller) {
        controller->resetSize(width, height);
    }
}

void Java_com_example_yllds_androidopengldemo_PngPreviewController_stop(JNIEnv *, jobject) {
    if (NULL != controller) {
        controller->stop();
        delete controller;
        controller = NULL;
    }
}

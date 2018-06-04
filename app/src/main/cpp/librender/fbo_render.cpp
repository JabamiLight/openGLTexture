//
// Created by TY on 2018/5/22.
//

#include "fbo_render.h"
#include "../libpng/png_write.h"

#define LOG_TAG "FboRender"

FboRender::FboRender(char *vertex, char *frag) : PicPreviewRender(vertex, frag) {}

void FboRender::render() {
    LOGI("render start");
    _backingWidth=720;
    _backingHeight=720;
    picPreviewTexture->bindFrameBuffer();
    LOGE("glViewport pixel width %d height %d", _backingWidth, _backingHeight);
    //大于fbo的缓冲样本数读取像素会出问题<=720
    glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight);
//    _backingWidth=96;
//    _backingHeight=96;
    //设置一个颜色状态
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    //使能颜色状态的值来清屏
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program);
    static const GLfloat _vertices[] = {-1.0f, 1.0f,//左上
                                        -1.0f, -1.0f,//左下
                                        1.0f, 1.0f,//右上
                                        1.0f, -1.0f//右下
    };
    //stride设置为0自动决定步长
    //设置定点缓存指针
    glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, _vertices);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
    //注意位置颠倒
    static const GLfloat texCoords[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    //设置纹理缓存指针，varying变量会被插值传入片元着色器
    glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
    //绑定纹理
    picPreviewTexture->bindTexture(uniformSampler);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    picPreviewTexture->bindTextureFrame(uniformSampler);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    LOGI("pixel width %d height %d", _backingWidth, _backingHeight);
    //超过glViewport的区域不会报错，但是生成带图片会混乱
    //小于则是读取一部分的矩形区域
//    int width=_backingWidth;
//    int height=_backingHeight;
//    unsigned char *buffers = new unsigned char[width * height * 4];
//    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffers);
//    PngWrite pngWrite;
//    FILE* fp2=fopen("/mnt/sdcard/ic_launchergrayrgb.rgb","wb");
//    fwrite(buffers,_backingWidth*_backingHeight*4,1,fp2);
//    fclose(fp2);
//    LOGI("start write png file");
//    pngWrite.writePngFile("/mnt/sdcard/ic_launchergray.png", width, height,
//                          (buffers));
//    delete[]buffers;
//    picPreviewTexture->dealloc();
}


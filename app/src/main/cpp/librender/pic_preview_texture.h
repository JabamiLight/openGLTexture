//
// Created by 唐宇 on 2018/5/20.
//

#ifndef ANDROIDOPENGLDEMO_PIC_PREVIEW_TEXTURE_H
#define ANDROIDOPENGLDEMO_PIC_PREVIEW_TEXTURE_H


#include "../libcommon/CommonTools.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
class PicPreviewTexture {
private:
    GLuint  texture;
    GLuint  textureFrame;
    GLuint  frame;
    GLuint  render;
    int initTexture();
    bool checkGlError(const char* op);


public:
    PicPreviewTexture();
    virtual ~PicPreviewTexture();

    bool createTexture();
    void updateTexImage(byte* pixel,int width,int height);
    bool bindTexture(GLint uniformaSampler);
    void dealloc();
    bool createFramBuffer();
    bool bindFrameBuffer();

    bool bindTextureFrame(GLuint i);
};


#endif //ANDROIDOPENGLDEMO_PIC_PREVIEW_TEXTURE_H

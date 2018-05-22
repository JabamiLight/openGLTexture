//
// Created by 唐宇 on 2018/5/20.
//

#ifndef ANDROIDOPENGLDEMO_PIC_PREVIEW_RENDER_H
#define ANDROIDOPENGLDEMO_PIC_PREVIEW_RENDER_H

#include "../libcommon/CommonTools.h"
#include "pic_preview_texture.h"
#include <android/asset_manager.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


class PicPreviewRender {
protected:
    enum {
        ATTRIBUTE_VERTEX, ATTRIBUTE_TEXCOORD,
    };
    char* PIC_PREVIEW_FRAG_SHADER_2;
    char* PIC_PREVIEW_VERTEX_SHADER_2;
    GLint _backingLeft;
    GLint _backingTop;
    GLint _backingWidth;
    GLint _backingHeight;

    GLuint vertShader;
    GLuint fragShader;

    PicPreviewTexture* picPreviewTexture;
    GLuint  uniformSampler;
    GLuint program;
    int useProgram();
    int initShader();
    GLuint compileShader(GLenum type, const char *source);
    bool checkGlError(const char* op);


public:
    PicPreviewRender(char *vertex, char *frag);
    virtual ~PicPreviewRender();
    virtual bool init(int width, int height, PicPreviewTexture* picPreviewTexture);
    virtual void render();
    virtual void dealloc();

    virtual void resetRenderSize(int left, int top, int width, int height);
};


#endif //ANDROIDOPENGLDEMO_PIC_PREVIEW_RENDER_H

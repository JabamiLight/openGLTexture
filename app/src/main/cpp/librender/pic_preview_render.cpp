//
// Created by 唐宇 on 2018/5/20.
//

#include "pic_preview_render.h"
#include "../libpng/png_write.h"

#define LOG_TAG "PicPreviewRender"


PicPreviewRender::PicPreviewRender(char *vertex, char *frag) {
    PIC_PREVIEW_FRAG_SHADER_2 = frag;
    PIC_PREVIEW_VERTEX_SHADER_2 = vertex;
}

int PicPreviewRender::useProgram() {
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    //可以直接调用这个方法进行绑定位置
    //或者调用glGetAttribLocation()获取
    //或者可以在glsl文件中直接指定(不是特比清楚是不是gles3才有的特性)
    glBindAttribLocation(program, ATTRIBUTE_VERTEX, "position");
    glBindAttribLocation(program, ATTRIBUTE_TEXCOORD, "texcoord");
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0) {
            char message[infoLen];
            glGetProgramInfoLog(program, infoLen, NULL, message);
            LOGE("Error linking program : %s\n", message);
        }

        glDeleteProgram(program);
        return 0;
    }
    glUseProgram(program);
    uniformSampler = glGetUniformLocation(program, "yuvTexSampler");
    return 1;
}

int PicPreviewRender::initShader() {
    vertShader = compileShader(GL_VERTEX_SHADER, PIC_PREVIEW_VERTEX_SHADER_2);
    if (!vertShader) {
        return -1;
    }
    fragShader = compileShader(GL_FRAGMENT_SHADER, PIC_PREVIEW_FRAG_SHADER_2);
    if (!fragShader) {
        return -1;
    }
    return 1;
}

GLuint PicPreviewRender::compileShader(GLenum type, const char *source) {
    GLint status;
    GLuint shader = glCreateShader(type);
    if (shader == 0 || shader == GL_INVALID_ENUM) {
        LOGE("Failed to create shader %d", type);
        return 0;
    }
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0) {
            char message[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, message);
            LOGE("Failed to compile shader : %s\n", message);
        }

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool PicPreviewRender::checkGlError(const char *op) {
    GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("error::after %s() glError (0x%x)\n", op, error);
        return true;
    }
    return false;
}

PicPreviewRender::~PicPreviewRender() {

}

bool PicPreviewRender::init(int width, int height, PicPreviewTexture *picPreviewTexture) {
    this->_backingLeft = 0;
    this->_backingTop = 0;
    this->_backingWidth = width;
    this->_backingHeight = height;
    this->picPreviewTexture = picPreviewTexture;

    vertShader = 0;
    fragShader = 0;
    program = 0;
    int ret = initShader();
    if (ret < 0) {
        LOGI("init shader failed...");
        this->dealloc();
        return false;
    }
    ret = useProgram();
    if (ret < 0) {
        LOGI("use program failed...");
        this->dealloc();
        return false;
    }
    return true;
}

void PicPreviewRender::render() {

    glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight);
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
    static const GLfloat texCoords[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    //设置纹理缓存指针，varying变量会被插值传入片元着色器
    glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
    //绑定纹理
    picPreviewTexture->bindTexture(uniformSampler);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //超过glViewport的区域不会报错，但是生成带图片会混乱
    //小于则是读取一部分的矩形区域
    int width=_backingWidth;
    int height=_backingHeight;
    unsigned char *buffers = new unsigned char[_backingHeight * _backingWidth * 4];
    glReadPixels(0, 0, _backingWidth, _backingHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffers);
    LOGI("读取数据成功");
    PngWrite pngWrite;
//    FILE* fp2=fopen("/mnt/sdcard/ic_launchergrayrgb.rgb","wb");
//    fwrite(buffers1,_backingWidth*_backingHeight*4,1,fp2);
//    fclose(fp2);
    LOGI("start writing png file");
    pngWrite.writePngFile("/mnt/sdcard/ic_launchergraynormal.png", width, height,
                          (buffers));
}

void PicPreviewRender::dealloc() {
    if (vertShader) {
        glDeleteShader(vertShader);
        vertShader = 0;
    }
    if (fragShader) {
        glDeleteShader(fragShader);
        fragShader = 0;
    }
    if (picPreviewTexture) {
        picPreviewTexture->dealloc();
    }
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }


}

void PicPreviewRender::resetRenderSize(int left, int top, int width, int height) {
    this->_backingLeft = left;
    this->_backingTop = top;
    this->_backingWidth = width;
    this->_backingHeight = height;

}

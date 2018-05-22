//
// Created by 唐宇 on 2018/5/20.
//

#ifndef ANDROIDOPENGLDEMO_RGBA_FRAME_H
#define ANDROIDOPENGLDEMO_RGBA_FRAME_H


#include <cstdint>

class RGBAImgaeFrame {
public:
    float position;
    float duration;
    uint8_t * pixels;
    int width;
    int height;
    RGBAImgaeFrame();
    virtual ~RGBAImgaeFrame();

};


#endif //ANDROIDOPENGLDEMO_RGBA_FRAME_H

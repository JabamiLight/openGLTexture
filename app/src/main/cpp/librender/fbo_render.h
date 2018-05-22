//
// Created by TY on 2018/5/22.
//

#ifndef ANDROIDOPENGLDEMO_FBO_RENDER_H
#define ANDROIDOPENGLDEMO_FBO_RENDER_H

#include "pic_preview_render.h"
class FboRender : public PicPreviewRender {
public:
    FboRender(char *vertex, char *frag);
    void render() override;


};


#endif //ANDROIDOPENGLDEMO_FBO_RENDER_H

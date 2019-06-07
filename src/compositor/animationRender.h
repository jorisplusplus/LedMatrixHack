#ifndef ANIMATIONRENDER_H
#define ANIMATIONRENDER_H

#include "imageRender.h"
#include "compositor.h"
#include <string>

class animationRender: public imageRender {
    public:
        animationRender(uint8_t *image, int x, int y, int sizeX, int sizeY, int numframes);
        void render(Color* frame);
    protected:
        int numframes;
        int frameoffset;
};

#endif
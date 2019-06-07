#ifndef IMAGERENDER_H
#define IMAGERENDER_H

#include "compositor.h"
#include <string>

class imageRender: public rendertask {
    public:
        imageRender(uint8_t *image, int x, int y, int sizeX, int sizeY);
        void render(Color* frame);
    protected:
        int sizex;
        int sizey;
        uint8_t *image;
};

#endif
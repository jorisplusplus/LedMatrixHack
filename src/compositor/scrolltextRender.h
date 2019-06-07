#ifndef SCROLLTEXTRENDER_H
#define SCROLLTEXTRENDER_H

#include "textRender.h"
#include "compositor.h"
#include <string>

class scrolltextRender: public textRender {
    public:
        scrolltextRender(std::string text, Color color, int x, int y, int sizex);
        void render(Color* frame);
    protected:
        int sizex;
        int offset;
};

#endif
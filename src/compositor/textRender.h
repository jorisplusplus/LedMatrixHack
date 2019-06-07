#ifndef TEXTRENDER_H
#define TEXTRENDER_H

#include "compositor.h"
#include <string>

class textRender: public rendertask {
    public:
        textRender(std::string text, Color color, int x, int y);
        void render(Color* frame);
    protected:
        void renderCharCol(uint8_t ch, Color color, Color *buffer, int x, int y);
        std::string text;
        Color color;
};

#endif
#include "imageRender.h"

imageRender::imageRender(uint8_t *image, int x, int y, int sizex, int sizey) {
    this->x = x;
    this->y = y;
    this->sizex = sizex;
    this->sizey = sizey;
    this->image = image;
}

void imageRender::render(Color *frame) {
    int xreal, yreal;
        for(int py=0; py<sizey; py++) {
                yreal = y + py;
                for(int px=0; px<sizex; px++) {
                        xreal = x + px;
                        if(yreal >= 0 && yreal < HEIGHT && xreal >= 0 && xreal < WIDTH) {
                                addColor(&frame[yreal*WIDTH+xreal], (Color *)&image[(py*sizex+px)*4]);
                        }
                }
        }
}
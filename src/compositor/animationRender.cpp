#include "animationRender.h"

animationRender::animationRender(uint8_t *image, int x, int y, int sizex, int sizey, int numframes):imageRender(image, x, y, sizex, sizey) {
   this->numframes = numframes;
   frameoffset = 0;
}

void animationRender::render(Color *frame) {
    int xreal, yreal;
        for(int py=0; py<sizey; py++) {
                yreal = y + py;
                for(int px=0; px<sizex; px++) {
                        xreal = x + px;
                        if(yreal >= 0 && yreal < HEIGHT && xreal >= 0 && xreal < WIDTH) {
                                addColor(&frame[yreal*WIDTH+xreal], (Color *)&image[(py*sizex+px+sizex*sizey*frameoffset)*4]);
                        }
                }
        }
   frameoffset++;
   if(frameoffset == numframes) frameoffset = 0;
}
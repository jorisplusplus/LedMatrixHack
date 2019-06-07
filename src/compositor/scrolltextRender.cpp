#include "scrolltextRender.h"

scrolltextRender::scrolltextRender(std::string text, Color color, int x, int y, int sizex):textRender(text, color, x, y) {
    this->sizex = sizex;
    offset = 1;
}

void scrolltextRender::render(Color *frame) {
    int x = this->x;
    int y = this->y;
    int currentpos = offset;
    offset++;
    if(offset > 0 && offset > 5*this->text.length()) offset = -sizex;
    printf(" %d\n", offset);
    if(currentpos < 0) {
            x += -currentpos;
            currentpos = 0;
    }

    for(int i = 0; i < this->text.length(); i++) {
        uint8_t charId = (uint8_t) text[i] - 32;
        for(int i = 0; i<5; i++) {
                if(currentpos > 0) {
                        currentpos--;        
                } else {
                        if(x >= (sizex+this->x)) return;
                        uint8_t cs = tinyFont[charId*5+i];                
                        renderCharCol(cs, color, frame, x, y);
                        x++;
                }        
        }
    }    
}
#include "textRender.h"

textRender::textRender(std::string text, Color color, int x, int y) : text(text), color(color) {
    this->x = x;
    this->y = y;
}

void textRender::render(Color *frame) {
    int x = this->x;
    int y = this->y;
    for(int i = 0; i < this->text.length(); i++) {
        uint8_t charId = (uint8_t) text[i] - 32;
        for(int i = 0; i<5; i++) {
                uint8_t cs = tinyFont[charId*5+i];
                renderCharCol(cs, color, frame, x, y);
                x++;        
        }    
    }
}

/*
* Renders a single column
* ch represents which leds should be on in the column
* Color selects the color of leds
* Buffer is framebuffer where in the bits will be set
* x,y coordinate of the top pixel in the column
* 
* Automaticaly checks every coordinate of the column is valid.
*/
void textRender::renderCharCol(uint8_t ch, Color color, Color *buffer, int x, int y) {
        for(int py = y; py<y+7; py++) {
                if(py >= 0 && py < HEIGHT && x >= 0 && x < WIDTH) {
                        if((ch & (1<<(py-y))) != 0) addColor(&buffer[py*WIDTH+x], &color);
                }
        }
}
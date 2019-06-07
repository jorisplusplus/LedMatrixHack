#include "compositor.h"

#include "textRender.h"
#include "scrolltextRender.h"
#include "imageRender.h"
#include "animationRender.h"

#include <mutex>
#include <list>
#include <cstring>

Color background;
Color *buffer;

std::mutex renderlist_lock;
std::list<rendertask*> renderlist;


Color genColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        Color color;
        color.RGB[0] = r;
        color.RGB[1] = g;
        color.RGB[2] = b;
        color.RGB[3] = a;
        return color;
}

compositor::compositor() {
        background.value = 0;
}

/*
* Sets the background color of the display.
*/
void compositor::setBackground(Color color) {
        background = color;
}

/*
* Add text to be rendered. If the text doesn't fit on the screen it will be cutoff
*
* text is the text to be rendered
* color is the color to be rendered
* x, y is the coordinate of the top left corner of the text block. each character is 8 pixels high and 5 pixels wide
*/
void compositor::addText(char *text, Color color, int x, int y) {
       renderlist.push_back(new textRender(text, color, x, y));
}

/*
* Add text to be rendered but also scroll it from right to left.
*
* text is the text to be rendered
* color is the color to be rendered
* x, y is the coordinate of the top left corner of the text block. each character is 8 pixels high and 5 pixels wide
* sizeX is the length over which text should be drawn
*/
void compositor::addScrollText(char *text, Color color, int x, int y, int sizeX) {
        renderlist.push_back(new scrolltextRender(text, color, x, y, sizeX));
}

/*
* Renders an image
*
* image is pointer to your image
* x,y is the coordinate for the top left corner
* width, length is width and length of the image
*/
void compositor::addImage(uint8_t *image, int x, int y, int width, int length) {
        renderlist.push_back(new imageRender(image, x, y, width, length));
}

/*
* Renders an animation
*
* image is pointer to your image
* x,y is the coordinate for the top left corner
* width, length is width and length of the image
* numframes is the number of frames in the animation
*/
void compositor::addAnimation(uint8_t *image, int x, int y, int width, int length, int numFrames) {
        renderlist.push_back(new animationRender(image, x, y, width, length, numFrames));
}

/*
* push your a custom rendertask.
* custom render task must inherit the rendertask class
*/
void compositor::pushCustomRender(rendertask *task) {
        renderlist.push_back(task);
}

/*
* Clears all rendertasks, keeps background the same
*/
void compositor::clear() {
        std::lock_guard<std::mutex> guard(renderlist_lock);
        renderlist.clear();        
}

void compositor::composite() {
        //Setting the background color
        for(int i = 0; i<WIDTH*HEIGHT; i++) {
                buffer[i] = background;
        }
        //Locking the render list
        std::lock_guard<std::mutex> guard(renderlist_lock);
        for(rendertask *task : renderlist) {
                task->render(buffer);
        }
}

void compositor::setBuffer(Color* framebuffer) {
        buffer = framebuffer;
}

void addColor(Color *target, Color *color) {
        target->RGB[0] = color->RGB[0] + (255-color->RGB[3])*target->RGB[0]/255;
        target->RGB[1] = color->RGB[1] + (255-color->RGB[3])*target->RGB[1]/255;
        target->RGB[2] = color->RGB[2] + (255-color->RGB[3])*target->RGB[2]/255;
}
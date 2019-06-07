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
        Color red = genColor(255, 0, 0, 255);
        Color blue = genColor(0, 0, 255, 255);
        Color green = genColor(50, 205, 50, 255);
        background.value = 0;
}

void compositor::setBackground(Color color) {
        background = color;
}

void compositor::addText(char *text, Color color, int x, int y) {
       renderlist.push_back(new textRender(text, color, x, y));
}

void compositor::addScrollText(char *text, Color color, int x, int y, int sizeX) {
        renderlist.push_back(new scrolltextRender(text, color, x, y, sizeX));
}

void compositor::addImage(uint8_t *image, int x, int y, int width, int length) {
        renderlist.push_back(new imageRender(image, x, y, width, length));
}

void compositor::addAnimation(uint8_t *image, int x, int y, int width, int length, int numFrames) {
        renderlist.push_back(new animationRender(image, x, y, width, length, numFrames));
}

void compositor::pushCustomRender(rendertask *task) {
        renderlist.push_back(task);
}

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
#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

#include "compositor/compositor.h"
  
  
void displayDriver_init();
Color* getFrameBuffer();
void render16();
void setBrightness(int brightness);

#endif

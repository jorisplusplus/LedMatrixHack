#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

#include "compositor/compositor.h"

class displayDriver {
public:
  displayDriver();
  Color* getFrameBuffer();
  void render16();
};

#endif

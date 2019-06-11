#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <stdint.h>
#include <stdbool.h>
#include "esp32-hal.h"

#define HEIGHT 8   //Width and height of the matrix
#define WIDTH 32

typedef union{
		uint32_t value;
		uint8_t RGB[4];
} Color;

typedef struct renderTask{
	struct renderTask *next;
	void *payload;
	int id;
	int x;
	int y;
	int sizeX;
	int sizeY;
	Color color;
} renderTask_t;

typedef struct animation{
	uint8_t *gif;
	int showFrame;
	int numberFrames;
} animation_t;

typedef struct scrollText{
	char *text;
	int skip;
	int speed;
} scrollText_t;

  void compositor_init();
  void compositor_clear();

  void compositor_setBackground(Color color);
  void compositor_addText(char *text, Color color, int x, int y);
  void compositor_addScrollText(char *text, Color color, int x, int y, int sizeX);
  void compositor_addAnimation(uint8_t *image, int x, int y, int width, int length, int numFrames);
  void compositor_addImage(uint8_t *image, int x, int y, int width, int length);

  void composite();

  void compositor_setBuffer(Color* framebuffer);

#endif

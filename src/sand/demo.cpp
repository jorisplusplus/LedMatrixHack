/*!
 * @file demo1-snow.cpp
 *
 * Simple example for Adafruit_PixelDust on Raspberry Pi.
 * REQUIRES rpi-rgb-led-matrix LIBRARY!
 * I2C MUST BE ENABLED using raspi-config!
 *
 */
#include "demo.h"
#include <SparkFunMPU9250-DMP.h>

#define N_FLAKES 1200 ///< Number of snowflakes on 64x64 matrix

volatile bool        running = true;
int                  nFlakes = N_FLAKES; // Runtime flake count (adapts to res)
int                        i;
int 									simOption;
Adafruit_PixelDust        *snow;
dimension_t                x, y;

MPU9250_DMP imu;
Color* sandbuffer;

demo::demo() {
	snow = NULL;
  sandbuffer = (Color *) malloc(HEIGHT*WIDTH*sizeof(Color));
  if (imu.begin() != INV_SUCCESS) {
    printf("Failed to start mpu\n");
  }
	Wire.begin(21,23);
  imu.setSensors(INV_XYZ_ACCEL);
  imu.setAccelFSR(2);
  imu.setLPF(5);
  imu.setSampleRate(100);
}

void demo::selectSim(int option, int flakes) {
	nFlakes = flakes;
	simOption = option;
	if(snow) delete(snow);
	if(simOption == 0) {
		snow = new Adafruit_PixelDust(WIDTH, HEIGHT, nFlakes, 1, 64, true);
		if(!snow->begin()) {
			printf("PixelDust init failed\n");
		}

		snow->randomize(); // Initialize random snowflake positions
	} else if(simOption == 1) {
		snow = new Adafruit_PixelDust(WIDTH, HEIGHT, nFlakes, 1, 64, true);
		if(!snow->begin()) {
			printf("PixelDust init failed\n");
		}
		int w;
		for(i=0; i<HEIGHT; i++) {
			w = (int)((1.0 - cos((double)i * M_PI * 2.0 /
		  (double)(HEIGHT - 1))) * ((double)WIDTH / 4.0 - 1.0) + 0.5);
			for(x=0; x<=w; x++) {
				snow->setPixel(x, i);         // Left
				snow->setPixel(WIDTH-1-x, i); // Right
			}
		}
		snow->randomize(); // Initialize random sand positions
	}
}

void demo::dispSnow() {
		//lis3dh.accelRead(&xx, &yy, &zz);
		// Run one frame of the simulation.  Axis flip here
		// depends how the accelerometer is mounted relative
		// to the LED matrix.
    if ( imu.dataReady() ) {
      imu.update(UPDATE_ACCEL);
    }
		snow->iterate(-imu.ay, imu.ax, imu.az);

		// Erase canvas and draw new snowflake positions
		for(i=0; i<WIDTH*HEIGHT; i++) {
      sandbuffer[i].value = 0;
    }
    if(simOption == 1) {
			int w;
			for(i=0; i<HEIGHT; i++) { // Hourglass...
				w = (int)((1.0 - cos((double)i * M_PI * 2.0 /
			  (double)(HEIGHT - 1))) *
			  ((double)WIDTH / 4.0 - 1.0) + 0.5);
				for(x=0; x<=w; x++) {
					sandbuffer[x+i*WIDTH].RGB[0] = 32;
					sandbuffer[x+i*WIDTH].RGB[1] = 32;
					sandbuffer[x+i*WIDTH].RGB[2] = 96;
					sandbuffer[(WIDTH-1-x)+i*WIDTH].RGB[0] = 32;
					sandbuffer[(WIDTH-1-x)+i*WIDTH].RGB[1] = 32;
					sandbuffer[(WIDTH-1-x)+i*WIDTH].RGB[2] = 96;
				}
			}
		}
		for(i=0; i<nFlakes; i++) {
			snow->getPosition(i, &x, &y);
      sandbuffer[x+y*WIDTH].RGB[0] = x*4;
			sandbuffer[x+y*WIDTH].RGB[1] = y*4;
			sandbuffer[x+y*WIDTH].RGB[2] = 255-x*4;
		}

		// Update matrix contents on next vertical sync
		// and provide a new canvas for the next frame.
		//canvas = led_matrix_swap_on_vsync(matrix, canvas);
}

void demo::setBuffer(Color* framebuffer) {
  sandbuffer = framebuffer;
}

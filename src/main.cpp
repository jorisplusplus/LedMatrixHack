#include <Arduino.h>

//#include "sand/demo.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <nvs_flash.h>

#include "esp_heap_caps.h"
#include "esp32-hal.h"


extern "C" {
#include "displayDriver.h"
#include "compositor/compositor.h"
}


//demo *demos;
bool active;

const char* ssid     = "LedMatrix";

WebServer server(80);

void displayTask(void *pvParameter) {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = 20;
        while(active) {
                vTaskDelayUntil( &xLastWakeTime, 1000/xFrequency );
                //demos->dispSnow();
                composite();
                render16();
        }
        printf("Exiting render task\n");
        vTaskDelete( NULL );
}

void handleRoot() {
        server.send(200, "text/plain", "hello from esp32!");
}

void webServer(void *pvParameter) {
        server.on("/", handleRoot);
        server.begin();

        
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = 20;
        for(;; ) {
                vTaskDelayUntil( &xLastWakeTime, xFrequency );
                server.handleClient();
        }
}

void setup() {
  delay(500);
        pinMode(5, OUTPUT);
        digitalWrite(5, 0);

        active = true;
        displayDriver_init();
        compositor_init();
        //demos = new demo();
        //demos->selectSim(0, 1216);
        //demos->setBuffer(driver->getFrameBuffer());
        compositor_setBuffer(getFrameBuffer());
        Color green;
        green.value = 0x0000AA00;
        Color red;
        red.value = 0x000000FF;
        Color blue;
        blue.value = 0x00550000;
        Color white;
        white.value = 0xFFFFFFFF;
        compositor_addText("B", blue, 15, 0);
        compositor_addText("G", green, 10, 0);
        compositor_addText("W", white, 0, 0);
        compositor_addText("R", red, 5, 0);
        compositor_addScrollText("Hello", red, 20, 0, 10);
        nvs_flash_init();
        xTaskCreatePinnedToCore(
                &displayTask, /* Task function. */
                "display", /* String with name of task. */
                15000,    /* Stack size in words. */
                NULL,     /* Parameter passed as input of the task */
                10,       /* Priority of the task. */
                NULL,     /* Task handle. */
                1);       /* Core ID */
        printf("Free: %d\n", ESP.getFreeHeap());
        // WiFi.softAP(ssid);
        // printf("Free: %d\n", ESP.getFreeHeap());
        // xTaskCreatePinnedToCore(
        //         &webServer, /* Task function. */
        //         "webserver", /* String with name of task. */
        //         5000,    /* Stack size in words. */
        //         NULL,     /* Parameter passed as input of the task */
        //         1,       /* Priority of the task. */
        //         NULL,     /* Task handle. */
        //         0);       /* Core ID */
}

void loop() {
  // put your main code here, to run repeatedly:
}
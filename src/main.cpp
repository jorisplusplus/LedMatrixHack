#include <Arduino.h>

//#include "sand/demo.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
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
#include "compositor/compositor.h"
#include "displayDriver.h"



//demo *demos;
displayDriver *driver;
compositor *comp;
bool active;

const char* ssid     = "LedMatrix";

WebServer server(80);

void displayTask(void *pvParameter) {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = 20;
        while(active) {
                vTaskDelayUntil( &xLastWakeTime, xFrequency );
                //demos->dispSnow();
                comp->composite();
                driver->render16();
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
        active = true;
        driver = new displayDriver();
        comp = new compositor();
        //demos = new demo();
        //demos->selectSim(0, 1216);
        //demos->setBuffer(driver->getFrameBuffer());
        comp->setBuffer(driver->getFrameBuffer());
        Color green;
        green.value = 0x0000FF00;
        comp->addText("Camp", green, 0, 0);
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
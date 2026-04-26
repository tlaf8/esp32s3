#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "ws2812/ws2812.hpp"

static ws2812 led{21};

static void blink_task(void *arg) {
    for(;;) {
        led.set(255, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(250));
        led.set(0, 255, 0);
        vTaskDelay(pdMS_TO_TICKS(250));
        led.set(0, 0, 255);
        vTaskDelay(pdMS_TO_TICKS(250));
        led.set(255, 255, 255);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

extern "C" void app_main(void) {
    led.init();

    xTaskCreate(blink_task, "blink", 2048, NULL, 5, NULL);
}
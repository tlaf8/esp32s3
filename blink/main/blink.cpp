#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "portmacro.h"
#include <string.h>
#include "ws2812.hpp"

static void blink_task(void *arg) {
    WS2812 onboard_led(WS2812_ONBOARD_LED_PIN);

    for(;;) {
        onboard_led.set({255, 0, 0});
        vTaskDelay(pdMS_TO_TICKS(250));
        onboard_led.set({0, 255, 0});
        vTaskDelay(pdMS_TO_TICKS(250));
        onboard_led.set({0, 0, 255});
        vTaskDelay(pdMS_TO_TICKS(250));
        onboard_led.set({255, 255, 255});
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

extern "C" void app_main(void) {
    xTaskCreate(blink_task, "blink", 2048, NULL, 5, NULL);
}
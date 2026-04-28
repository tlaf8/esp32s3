#include "ws2812/ws2812.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static ws2812 led{21};
constexpr uint8_t speed_ms = 10;

void breathe(char channel) {
    uint8_t r = 0, g = 0, b = 0;

    switch(channel) {
        case 'r': r = 1; break;
        case 'g': g = 1; break;
        case 'b': b = 1; break;
        case 'w': r = g = b = 1; break;
    }

    for(int i = 0; i < 255; i++) {
        led.set((r * i), (g * i), (b * i));
        vTaskDelay(pdMS_TO_TICKS(speed_ms));
    }

    for(int i = 255; i >= 0; i--) {
        led.set((r * i), (g * i), (b * i));
        vTaskDelay(pdMS_TO_TICKS(speed_ms));
    }

}

static void breathe_task(void *arg) {
    for(;;) {
        breathe('r');
        breathe('g');
        breathe('b');
        breathe('w');
    }
}

extern "C" void app_main() {
    led.init();

    xTaskCreate(breathe_task, "breathe", 4096, nullptr, 5, nullptr);
}


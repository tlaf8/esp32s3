#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "ws2812/ws2812.hpp"

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

QueueHandle_t led_queue = nullptr;
static ws2812 led{21};

static void led_task(void*) {
    ESP_ERROR_CHECK(led.init());

    rgb command;
    for(;;) {
        if(xQueueReceive(led_queue, &command, portMAX_DELAY)) {
            led.set(command.r, command.g, command.b);
        }
    }
}

static void command_task(void*) {
    rgb command;
    for(;;) {
        command = {255, 0, 0};
        xQueueSend(led_queue, &command, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500));

        command = {0, 255, 0};
        xQueueSend(led_queue, &command, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500));

        command = {0, 0, 255};
        xQueueSend(led_queue, &command, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

extern "C" void app_main(void)
{
    led_queue = xQueueCreate(4, sizeof(rgb));

    xTaskCreate(led_task, "led", 4096, nullptr, 5, nullptr);
    xTaskCreate(command_task, "cmd", 4096, nullptr, 5, nullptr);
}

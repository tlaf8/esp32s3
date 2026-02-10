extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
}

#include "ws2812.hpp"

QueueHandle_t g_led_queue = nullptr;

static void led_task(void*) {
    WS2812 led(WS2812_ONBOARD_LED_PIN);
    ESP_ERROR_CHECK(led.init());

    RGB cmd;

    //Continuously wait for packet from command_task
    for(;;) {
        //Set led based on what was received
        if(xQueueReceive(g_led_queue, &cmd, portMAX_DELAY)) {
            led.set(cmd);
        }
    }
}

static void command_task(void*) {
    RGB cmd;

    //Continuously cycle through available colours and send to led_task
    for(;;) {
        cmd = {255, 0, 0};
        xQueueSend(g_led_queue, &cmd, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500));

        cmd = {0, 255, 0};
        xQueueSend(g_led_queue, &cmd, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500));

        cmd = {0, 0, 255};
        xQueueSend(g_led_queue, &cmd, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

extern "C" void app_main(void)
{
    //Create the queue
    g_led_queue = xQueueCreate(4, sizeof(RGB));
    configASSERT(g_led_queue);

    //Create tasks
    xTaskCreate(led_task, "led", 4096, nullptr, 5, nullptr);
    xTaskCreate(command_task, "cmd", 4096, nullptr, 5, nullptr);
}

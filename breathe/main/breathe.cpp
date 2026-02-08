#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "ws2812.hpp"

#define BREATHING_SPEED_MS 10

static void breathe(WS2812& led, bool r, bool g, bool b)
{
    for (int i = 0; i <= 255; ++i) {
        led.set({
            static_cast<uint8_t>(r ? i : 0),
            static_cast<uint8_t>(g ? i : 0),
            static_cast<uint8_t>(b ? i : 0)
        });

        vTaskDelay(pdMS_TO_TICKS(BREATHING_SPEED_MS));
    }


    for (int i = 255; i >= 0; i--) {
        led.set({
            static_cast<uint8_t>(r ? i : 0),
            static_cast<uint8_t>(g ? i : 0),
            static_cast<uint8_t>(b ? i : 0)
        });

        vTaskDelay(pdMS_TO_TICKS(BREATHING_SPEED_MS));
    }
}

static void breathe_task(void*)
{
    WS2812 led(WS2812_ONBOARD_LED_PIN);
    ESP_ERROR_CHECK(led.init());

    for (;;)
    {
        breathe(led, true, false, false);
        breathe(led, false, true, false);
        breathe(led, false, false, true);
    }
}

extern "C" void app_main(void)
{
    xTaskCreate(breathe_task, "breathe", 2048, nullptr, 5, nullptr);
}

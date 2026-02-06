#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "portmacro.h"
#include <string.h>

#define WS2812_ONBOARD_LED_PIN 21

static void ws2812_init(led_strip_config_t* ws2812) {
    ws2812->strip_gpio_num = WS2812_ONBOARD_LED_PIN;
    ws2812->max_leds = 1;
    ws2812->led_model = LED_MODEL_WS2812;
    ws2812->color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGBW;
    ws2812->flags.invert_out = false;
}

static void rmt_init(led_strip_rmt_config_t* rmt_config) {
    rmt_config->clk_src = RMT_CLK_SRC_DEFAULT;
    rmt_config->resolution_hz = 10 * 1000 * 1000;
    rmt_config->mem_block_symbols = 64,
    rmt_config->flags.with_dma = false;
}

static void blink_task(void *arg) {
    led_strip_config_t led_config = { 0 };
    led_strip_rmt_config_t rmt_config = { 0 };
    
    ws2812_init(&led_config);
    rmt_init(&rmt_config);

    led_strip_handle_t led_handle;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&led_config, &rmt_config, &led_handle));

    for(;;) {
        led_strip_set_pixel_rgbw(led_handle, 0, 255, 0, 0, 0);
        led_strip_refresh(led_handle);

        vTaskDelay(pdMS_TO_TICKS(500));

        led_strip_set_pixel_rgbw(led_handle, 0, 0, 255, 0, 0);
        led_strip_refresh(led_handle);

        vTaskDelay(pdMS_TO_TICKS(500));

        led_strip_set_pixel_rgbw(led_handle, 0, 0, 0, 255, 0);
        led_strip_refresh(led_handle);

        vTaskDelay(pdMS_TO_TICKS(500));

        led_strip_set_pixel_rgbw(led_handle, 0, 255, 255, 255, 255);
        led_strip_refresh(led_handle);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void) {
    xTaskCreate(blink_task, "blink", 2048, NULL, 5, NULL);
}`
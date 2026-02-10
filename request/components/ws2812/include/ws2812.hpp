#pragma once

#include <cstdint>
#include "base_types.hpp"

extern "C" {
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
}

#define WS2812_ONBOARD_LED_PIN 21

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class WS2812
{
PUBLIC:
    WS2812(uint8_t gpio_pin, bool invert = false);
    virtual ~WS2812() {}

    esp_err_t set(const RGB& colour_array);
    esp_err_t off();
    esp_err_t init();

PRIVATE:
    led_strip_config_t led_config_{};
    led_strip_rmt_config_t rmt_config_{};
    led_strip_handle_t led_handle_{nullptr};
    uint8_t gpio_pin_;
    bool invert_;
};

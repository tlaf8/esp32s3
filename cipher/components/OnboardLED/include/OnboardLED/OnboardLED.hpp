#pragma once

#include "esp_err.h"
#include "led_strip_types.h"
#include <cstdint>

class OnboardLED {
public:
    OnboardLED(uint8_t gpio);
    ~OnboardLED();

    esp_err_t init();
    esp_err_t set(uint8_t r, uint8_t g, uint8_t b);
    esp_err_t clear();

    inline bool is_initialized() { return initialized_; }

private:
    uint8_t gpio_;
    bool initialized_{false};
    led_strip_handle_t handle_{nullptr};
};

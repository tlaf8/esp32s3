#include "ws2812/ws2812.hpp"
#include "led_strip.h"
#include "led_strip_rmt.h"
#include "led_strip_types.h"
#include "soc/clk_tree_defs.h"
#include "esp_check.h"
#include <cstdint>

OnboardLED::OnboardLED(uint8_t gpio)
    : gpio_(gpio) {}

OnboardLED::~OnboardLED() {}

esp_err_t OnboardLED::init() {
    led_strip_config_t led_conf = {
        .strip_gpio_num = gpio_,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {
            .invert_out = 0
        }
    };

    led_strip_rmt_config_t rmt_conf = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000, // 10 MHz
        .mem_block_symbols = 0,
        .flags = {
            .with_dma = false,
        }
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&led_conf, &rmt_conf, &handle_));

    initialized_ = true;
    return ESP_OK;
}

esp_err_t OnboardLED::set(uint8_t r, uint8_t g, uint8_t b) {
    ESP_RETURN_ON_FALSE(initialized_, ESP_ERR_INVALID_STATE, "LED", "Not initialized");

    ESP_ERROR_CHECK(led_strip_set_pixel(handle_, 0, r, g, b));
    ESP_ERROR_CHECK(led_strip_refresh(handle_));

    return ESP_OK;
}

esp_err_t OnboardLED::clear() {
    ESP_RETURN_ON_FALSE(initialized_, ESP_ERR_INVALID_STATE, "LED", "Not initialized");

    ESP_ERROR_CHECK(led_strip_clear(handle_));
    ESP_ERROR_CHECK(led_strip_refresh(handle_));

    return ESP_OK;
}

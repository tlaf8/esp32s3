#include "ws2812.hpp"

WS2812::WS2812(uint8_t gpio_pin, bool invert): 
gpio_pin_(gpio_pin), invert_(invert) {
    init();
}

esp_err_t WS2812::init() {
    led_config_.strip_gpio_num = gpio_pin_;
    led_config_.max_leds = 1;
    led_config_.led_model = LED_MODEL_WS2812;
    led_config_.color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB;
    led_config_.flags.invert_out = invert_;
    return led_strip_new_rmt_device(&led_config_, &rmt_config_, &led_handle_);
}

esp_err_t WS2812::set(const RGB& colour_array) {
    if (!led_handle_) return ESP_ERR_INVALID_STATE;

    led_strip_set_pixel(led_handle_, 0, colour_array.r, colour_array.g, colour_array.b);
    return led_strip_refresh(led_handle_);
}

esp_err_t WS2812::off() {
    if (!led_handle_) return ESP_ERR_INVALID_STATE;

    led_strip_set_pixel(led_handle_, 0, 0, 0, 0);
    return led_strip_refresh(led_handle_);
}

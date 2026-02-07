idf_component_register(
    SRCS "src/ws2812.cpp"
    INCLUDE_DIRS "include"
    REQUIRES base_types led_strip freertos
)
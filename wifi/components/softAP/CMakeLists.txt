idf_component_register(
    SRCS "src/softAP.cpp"
    INCLUDE_DIRS "include"
    REQUIRES cipher OnboardLED esp_wifi esp_event esp_netif esp_http_server nvs_flash
)
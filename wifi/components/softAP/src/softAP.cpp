#include "softAP/softAP.hpp"
#include "esp_err.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types_generic.h"
#include <cstring>

softAP::softAP(const char* ssid, const char* password, uint8_t max_conn)
    : ssid_(ssid), password_(password), max_conn_(max_conn) {}

void softAP::init() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}

void softAP::start() {
    wifi();
    server();
}

void softAP::wifi() {
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_conf = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_conf));

    wifi_config_t wifi_conf{};
    std::strncpy(reinterpret_cast<char*>(wifi_conf.ap.ssid), ssid_, sizeof(wifi_conf.ap.ssid));
    std::strncpy(reinterpret_cast<char*>(wifi_conf.ap.password), password_, sizeof(wifi_conf.ap.password));
    wifi_conf.ap.ssid_len = std::strlen(ssid_);
    wifi_conf.ap.channel = 1;
    wifi_conf.ap.max_connection = max_conn_;
    wifi_conf.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    if (std::strlen(password_) == 0) {
        wifi_conf.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_conf));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("softAP", "Broadcast started SSID: %s password: %s", ssid_, password_);
}

void softAP::server() {
    httpd_config_t server_conf = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server_, &server_conf) == ESP_OK) {
        httpd_uri_t root_uri{};
        root_uri.uri = "/";
        root_uri.method = HTTP_GET;
        root_uri.handler = root_handler;
        root_uri.user_ctx = nullptr;

        httpd_register_uri_handler(server_, &root_uri);
    }

    ESP_LOGI("softAP", "HTTP server started");
}

esp_err_t softAP::root_handler(httpd_req_t* req) {
    const char* resp =
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>ESP32S3</title></head>"
        "<body>"
        "<p>HTTP web server is working</p>"
        "</body>"
        "</html>";

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

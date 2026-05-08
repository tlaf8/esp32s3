#include "softAP/softAP.hpp"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_event.h"
#include "softAP/web_assets.hpp"
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
    wifi_conf.ap.ssid[sizeof(wifi_conf.ap.ssid) - 1] = '\0';

    std::strncpy(reinterpret_cast<char*>(wifi_conf.ap.password), password_, sizeof(wifi_conf.ap.password));
    wifi_conf.ap.password[sizeof(wifi_conf.ap.password) - 1] = '\0';

    wifi_conf.ap.ssid_len = std::strlen(ssid_);
    wifi_conf.ap.channel = 1;
    wifi_conf.ap.max_connection = max_conn_;
    wifi_conf.ap.authmode = WIFI_AUTH_WPA3_PSK;

    if (std::strlen(password_) == 0) {
        wifi_conf.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_conf));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("softAP", "AP started: %s", ssid_);
}

void softAP::add_route(const std::string& uri, httpd_method_t method, std::function<esp_err_t(httpd_req_t*)> handler) {
    routes_.push_back({uri, method, handler});
}

esp_err_t softAP::dispatch(httpd_req_t* req) {
    auto* route = static_cast<Route*>(req->user_ctx);

    if (!route || !route->handler) {
        ESP_LOGE("softAP", "Invalid route or handler");
        return ESP_FAIL;
    }

    return route->handler(req);
}

void softAP::server() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server_, &config) == ESP_OK) {
        for (auto& route : routes_) {
            httpd_uri_t uri{};
            uri.uri = route.uri.c_str();
            uri.method = route.method;
            uri.handler = dispatch;
            uri.user_ctx = &route;

            httpd_register_uri_handler(server_, &uri);
        }

        ESP_LOGI("softAP", "HTTP server started");
    } else {
        ESP_LOGE("softAP", "Failed to start HTTP server");
    }
}

std::string softAP::recv_data(httpd_req_t* req) {
    std::string body;
    body.resize(req->content_len);

    int received = 0;

    while (received < req->content_len) {
        int r = httpd_req_recv(req, body.data() + received, req->content_len - received);

        if (r <= 0) {
            ESP_LOGE("softAP", "recv failed");
            return {};
        }

        received += r;
    }

    return body;
}
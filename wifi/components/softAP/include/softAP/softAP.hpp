#pragma once

#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"

class softAP {
public:
    softAP(const char* ssid, const char* password, uint8_t max_conn = 4);

    void init();
    void start();

private:
    const char* ssid_;
    const char* password_;
    uint8_t max_conn_;
    httpd_handle_t server_{nullptr};

    void wifi();
    void server();

    static esp_err_t root_handler(httpd_req_t* req);
};

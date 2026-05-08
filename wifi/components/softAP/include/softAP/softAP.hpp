#pragma once

#include "esp_http_server.h"
#include <string>
#include <list>
#include <functional>

struct Route {
    std::string uri;
    httpd_method_t method;
    std::function<esp_err_t(httpd_req_t*)> handler;
};

class softAP {
public:
    softAP(const char* ssid, const char* password, uint8_t max_conn);

    void init();
    void start();
    void add_route(const std::string& uri, httpd_method_t method, std::function<esp_err_t(httpd_req_t*)> handler);

    std::string recv_data(httpd_req_t* req);

private:
    void wifi();
    void server();

    static esp_err_t dispatch(httpd_req_t* req);

    std::list<Route> routes_;
    httpd_handle_t server_ = nullptr;
    const char* ssid_;
    const char* password_;
    uint8_t max_conn_;
};
#include "softAP/softAP.hpp"
#include "softAP/web_assets.hpp"
#include "esp_log.h"
#include <nlohmann/json.hpp>
#include "cipher/queue.hpp"
#include "cipher/task.hpp"
#include "ws2812/ws2812.hpp"

using json = nlohmann::json;

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void led_proc();

static softAP ap("totally-not-a-virus", "fuckmefeixiao", 4);
static cipher::queue led_queue = cipher::queue(sizeof(rgb), 16);
static cipher::task led_proc_task = cipher::task("led_proc", 4096, 5, led_proc);

void led_proc() {
    rgb command;
    ws2812 led{21};

    led.init();

    for(;;) {
        if (led_queue.receive(&command, portMAX_DELAY)) {
            led.set(command.r, command.g, command.b);
        }
    }
}

extern "C" void app_main() {
    led_proc_task.run();

    ap.init();

    ap.add_route("/", HTTP_GET, [](httpd_req_t* req) {
        httpd_resp_send(req, reinterpret_cast<const char*>(index_html_start), (index_html_end - index_html_start));
        return ESP_OK;
    });

    ap.add_route("/led", HTTP_POST, [&](httpd_req_t* req) {
        auto raw_data = ap.recv_data(req);
        if (raw_data.empty()) {
            return ESP_FAIL;
        }

        ESP_LOGI("softAP/led", "JSON: %s", raw_data.c_str());

        try {
            const auto data = json::parse(raw_data);

            const rgb command = {
                data.at("r").get<uint8_t>(),
                data.at("g").get<uint8_t>(),
                data.at("b").get<uint8_t>()
            };

            ESP_LOGI("softAP/led", "Parsed value: %d %d %d", command.r, command.g, command.b);

            led_queue.send(&command, 0);

            httpd_resp_set_type(req, "application/json");
            httpd_resp_send(req, R"({"status":"ok"})", HTTPD_RESP_USE_STRLEN);
        } catch (...) {
            ESP_LOGE("softAP/led", "Invalid JSON");
            return ESP_FAIL;
        }

        httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    });

    ap.start();
}

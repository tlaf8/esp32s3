#include "cipher/Queue.hpp"
#include "cipher/Task.hpp"
#include "cipher/Mutex.hpp"
#include "cipher/OS.hpp"
#include "ws2812/ws2812.hpp"
#include "esp_log.h"
#include <inttypes.h>

using namespace cipher;

static Queue queue(sizeof(uint32_t), 1);
static Mutex mutex;
static OnboardLED led{21};

void blink(char channel) {
    mutex.try_lock(100);

    switch(channel) {
        case 'r': {
            led.set(255, 0, 0);
            OS::sleep(25);
            led.clear();
            break;
        }
        case 'g': {
            led.set(0, 255, 0);
            OS::sleep(25);
            led.clear();
            break;
        }
        case 'b': {
            led.set(0, 0, 255);
            OS::sleep(25);
            led.clear();
            break;
        }
    }

    mutex.unlock();
}

void ping_task() {
    uint32_t value = 0;

    for(;;) {
        blink('g');

        value++;
        queue.send(&value, 1000);

        ESP_LOGI("ping send", "[%" PRIu32 "]", value);

        if (queue.receive(&value, 1000)) {
            ESP_LOGI("ping recv", "[%" PRIu32 "]", value);
        }

        blink('r');
        OS::sleep(1000);
    }
}

void pong_task() {
    uint32_t value;

    for(;;) {
        if(queue.receive(&value, 1000)) {
            blink('g');

            ESP_LOGI("pong recv", "[%" PRIu32 "]", value);

            value++;
            queue.send(&value, 1000);

            ESP_LOGI("pong send", "[%" PRIu32 "]", value);

            blink('b');
            OS::sleep(1000);
        }
    }
}

extern "C" void app_main() {
    led.init();
    led.clear();

    static Task ping("ping", 4096, 5, ping_task);
    static Task pong("pong", 4096, 5, pong_task);

    ping.run();
    pong.run();
}
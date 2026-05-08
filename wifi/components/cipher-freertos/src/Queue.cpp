#include "cipher/queue.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/queue.h"
#include "portmacro.h"

namespace cipher {

queue::queue(size_t item_size, size_t length) {
    handle_ = xQueueCreate(length, item_size);
}

queue::~queue() {
    if (handle_) {
        vQueueDelete(handle_);
    }
}

bool queue::send(const void* item, uint32_t timeout_ms) {
    return xQueueSend(handle_, item, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

bool queue::receive(void* item, uint32_t timeout_ms) {
    return xQueueReceive(handle_, item, (timeout_ms == portMAX_DELAY) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

size_t queue::size() const {
    return uxQueueMessagesWaiting(handle_);
}

}
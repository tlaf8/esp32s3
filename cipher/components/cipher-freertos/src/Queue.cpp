#include "cipher/Queue.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/queue.h"

namespace cipher {

Queue::Queue(size_t item_size, size_t length) {
    handle_ = xQueueCreate(length, item_size);
}

Queue::~Queue() {
    if (handle_) {
        vQueueDelete(handle_);
    }
}

bool Queue::send(const void* item, uint32_t timeout_ms) {
    return xQueueSend(handle_, item, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

bool Queue::receive(void* item, uint32_t timeout_ms) {
    return xQueueReceive(handle_, item, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

size_t Queue::size() const {
    return uxQueueMessagesWaiting(handle_);
}

}
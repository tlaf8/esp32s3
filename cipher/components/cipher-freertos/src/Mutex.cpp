#include "cipher/Mutex.hpp"
#include "freertos/semphr.h"
#include "portmacro.h"
#include <cstdint>

namespace cipher {

Mutex::Mutex() {
    handle_ = xSemaphoreCreateMutex();
}

Mutex::~Mutex() {
    if (handle_) {
        vSemaphoreDelete(handle_);
    }
}

void Mutex::lock() {
    xSemaphoreTake(handle_, portMAX_DELAY);
}

void Mutex::unlock() {
    xSemaphoreGive(handle_);
}

bool Mutex::try_lock(uint32_t timeout_ms) {
    return xSemaphoreTake(handle_, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

}
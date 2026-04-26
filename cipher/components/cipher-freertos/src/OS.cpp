#include "cipher/OS.hpp"
#include "freertos/FreeRTOS.h"

using namespace cipher;

void OS::sleep(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}
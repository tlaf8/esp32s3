#pragma once
#include "cipher/interfaces/IQueue.hpp"
#include "freertos/idf_additions.h"
#include <cstdint>

namespace cipher {

class Queue : public interfaces::IQueue {
public:
    Queue(size_t item_size, size_t length);
    ~Queue();

    bool send(const void* item, uint32_t timeout_ms) override;
    bool receive(void* item, uint32_t timeout_ms) override;

    size_t size() const override;

private:
    QueueHandle_t handle_;
};

}
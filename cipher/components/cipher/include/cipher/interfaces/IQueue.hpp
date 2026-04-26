#pragma once
#include <cstddef>
#include <cstdint>

namespace cipher::interfaces {

class IQueue {
public:
    virtual ~IQueue() = default;

    virtual bool send(const void* item, uint32_t timeout_ms) = 0;
    virtual bool receive(void* item, uint32_t timeout_ms) = 0;

    virtual size_t size() const = 0;
};

}
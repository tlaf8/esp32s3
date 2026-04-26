#pragma once
#include <cstdint>

namespace cipher::interfaces {

class IMutex {
public:
    virtual ~IMutex() = default;

    virtual void lock() = 0;
    virtual void unlock() = 0;

    virtual bool try_lock(uint32_t timeout_ms) = 0;
};

}
#pragma once
#include <cstdint>

namespace cipher::interfaces {

class i_mutex {
public:
    virtual ~i_mutex() = default;

    virtual void lock() = 0;
    virtual void unlock() = 0;

    virtual bool try_lock(uint32_t timeout_ms) = 0;
};

}
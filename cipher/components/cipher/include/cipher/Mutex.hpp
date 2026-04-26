#pragma once
#include "cipher/interfaces/IMutex.hpp"
#include "freertos/idf_additions.h"

namespace cipher {

class Mutex : public interfaces::IMutex {
public:
    Mutex();
    ~Mutex();

    void lock() override;
    void unlock() override;

    bool try_lock(uint32_t timeout_ms) override;

private:
    SemaphoreHandle_t handle_;
};

}

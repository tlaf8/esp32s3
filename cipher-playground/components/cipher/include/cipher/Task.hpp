#pragma once
#include "cipher/interfaces/ITask.hpp"
#include "freertos/idf_additions.h"
#include <functional>

namespace cipher {

class Task : public interfaces::ITask {
public:
    using func_t = std::function<void()>;

    Task(const char* name, uint32_t stack_size, uint32_t priority, func_t entry_point);
    ~Task();

    void run() override;
    void suspend() override;
    void resume() override;

private:
    static void task_entry(void* param);

    const char* name_;
    uint32_t stack_size_;
    uint32_t priority_;
    func_t func_;

    TaskHandle_t handle_;
};

}
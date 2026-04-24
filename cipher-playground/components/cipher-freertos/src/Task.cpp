#include "cipher/Task.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

namespace cipher {

Task::Task(const char* name, uint32_t stack_size, uint32_t priority, func_t func)
    : name_(name), stack_size_(stack_size), priority_(priority), func_(func), handle_(nullptr) {}

Task::~Task() {
    //TODO: implement this if needed
}

void Task::task_entry(void* param) {
    auto* self = static_cast<Task*>(param);
    self->func_();
    vTaskDelete(nullptr);
}

void Task::run() {
    xTaskCreate(task_entry, name_, stack_size_, this, priority_, &handle_);
}

void Task::suspend() {
    vTaskSuspend(handle_);
}

void Task::resume() {
    vTaskResume(handle_);
}

}
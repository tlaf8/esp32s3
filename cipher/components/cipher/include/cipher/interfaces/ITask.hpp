#pragma once

namespace cipher::interfaces {

class ITask {
public:
    virtual ~ITask() = default;

    virtual void run() = 0;
    virtual void suspend() = 0;
    virtual void resume() = 0;
};

}
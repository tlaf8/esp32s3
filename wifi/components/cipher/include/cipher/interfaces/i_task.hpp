#pragma once

namespace cipher::interfaces {

class i_task {
public:
    virtual ~i_task() = default;

    virtual void run() = 0;
    virtual void suspend() = 0;
    virtual void resume() = 0;
};

}
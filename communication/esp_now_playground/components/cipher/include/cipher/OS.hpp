#pragma once
#include <cstdint>

namespace cipher {

class OS {
public:
    static void sleep(uint32_t ms);
};

}
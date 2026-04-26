#include "softAP/softAP.hpp"

extern "C" void app_main() {
    softAP server("totally-not-a-virus", "idkwhattoputlol");

    server.init();
    server.start();
}

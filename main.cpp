#include <iostream>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <thread>

#include "logging.hpp"
#include "fastgl.hpp"

using namespace std;

struct point3_t {
    signed x, y ,z;
};

int main() {
    fastgl_t lib(500, 300);
    lib.create_window(100, 100);
    return 0;
}

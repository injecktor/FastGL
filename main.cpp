#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "image_process.h"

using namespace std;

int main() {
    image_process_t ip(250, 250);

    ip.line(color_t(color_t::red), 1, 127, 50, 240, 150);

    ip.generate_image("image", image_process_t::ppm);

    return 0;
}

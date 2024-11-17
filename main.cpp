#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "image_process.h"

using namespace std;

int main() {
    image_process_t ip(250, 250);

    ip.square(color_t(color_t::red), 1, 127, 50, 50);

    ip.set_background(color_t(color_t::green));

    ip.generate_image("image", image_process_t::ppm);

    return 0;
}

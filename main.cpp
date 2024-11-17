#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "image_process.h"

using namespace std;

int main() {
    image_process_t i_p(250, 250);

    i_p.line(color_t(color_t::red), 1, 50, 50, 50, 50);
    // i_p.triangle(color_t(color_t::red), 1, 127, 50, 200, 100, 70, 20);

    i_p.generate_image("image", image_process_t::ppm);

    return 0;
}

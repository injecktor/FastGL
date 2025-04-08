#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "frame_process.h"

using namespace std;

int main() {
    frame_process_t frame_proc(250, 250);

    frame_proc.line(color_t(color_t::red), 1, 50, 50, 50, 50);
    // i_p.triangle(color_t(color_t::red), 1, 127, 50, 200, 100, 70, 20);

    frame_proc.generate_image("image", frame_process_t::ppm);

    return 0;
}

#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "frame_process.h"

using namespace std;

int main() {
    frame_process_t frame_proc(250, 250);

    //frame_proc.line(line_t(color_t(color_t::red), 1), {130, 120}, {200, 200});
    frame_proc.line(line_t(color_t(color_t::red), 1), {50, 150}, {50, 50});

    frame_proc.generate_image("image", frame_process_t::image_type::ppm);

    return 0;
}

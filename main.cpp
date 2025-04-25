#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "draw_process.h"

using namespace std;

int main() {
    draw_process_t frame_proc(250, 250);

    frame_proc.line(line_t(color_t(color_t::red)), {20, 20}, {120, 220});
    frame_proc.line(line_t(color_t(color_t::red)), {120, 20}, {20, 220});

    // frame_proc.circle(color_t::red, {100, 100}, 55);
    // frame_proc.set_pixel(color_t::black, {100, 100});
    // frame_proc.square(line_t(color_t(color_t::green, 0.5)), {50, 50}, 100, true);
    // frame_proc.square(line_t(color_t(color_t::green, 0.5)), {100, 100}, 100, true);

    frame_proc.generate_image("image", draw_process_t::image_type::ppm);

    return 0;
}

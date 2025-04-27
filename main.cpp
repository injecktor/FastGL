#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.hpp"
#include "fastgl.hpp"

using namespace std;

int main() {
    fastgl_t frame(250, 250);

    line_params_t param;
    param.aa = line_antialiasing::wu;
    param.effect = line_effects::gradient;
    param.extra_color = color_t::blue;

    // frame.line(line_t(color_t(color_t::red), 1, param), {20, 20}, {120, 220});
    // frame.line(line_t(color_t(color_t::red), 1, param), {120, 20}, {20, 220});

    // frame.circle(color_t::red, {100, 100}, 55);
    // frame.set_pixel(color_t::black, {100, 100});
    // frame.square(line_t(color_t(color_t::green, 0.5)), {50, 50}, 100, true);
    frame.square(line_t(color_t(color_t::green, 0.5)), {100, 100}, 100, true);

    frame.render();

    frame.generate_image("image", image_type_t::ppm);

    return 0;
}

#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.hpp"
#include "fastgl.hpp"

#include <X11/Xlib.h>

using namespace std;

struct point3_t {
    signed x, y ,z;
};

int main() {
    // fastgl_t frame(10, 10);
    // rect_params_t rect_params;
    // rect_params.use_inner_color = true;
    // rect_params.inner_color = color_t(color_t::green, 0.5);
    // frame.rectangle(line_t(color_t::red), {50, 50}, 100, 80, true, rect_params);
    // circle_params_t cir_params;
    // cir_params.use_inner_color = true;
    // cir_params.inner_color = color_t(color_t::green, 0.5);
    // frame.circle(color_t::red, {100, 170}, 50, true, cir_params);

    fastgl_t frame(1, 1);
    frame.set_pixel(color_t::red, {0, 0});

    frame.render();
    frame.generate_image("image.png", image_type_t::png);

    return 0;
}

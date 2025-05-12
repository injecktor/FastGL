#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.hpp"
#include "fastgl.hpp"

using namespace std;

int main() {
    fastgl_t scene(250, 250);

    line_params_t line_param;
    line_param.effect = line_effects::gradient;
    line_param.extra_color = color_t::blue;

    rect_params_t rect_param;
    rect_param.rotation = 1;

    // scene.line(line_t(color_t(color_t::red), 1), {100, 100}, {194, 129});
    // scene.line(line_t(color_t(color_t::red), 1), {100, 100}, {85, 146});
    // scene.line(line_t(color_t(color_t::red), 1), {179, 175}, {194, 129});
    // scene.line(line_t(color_t(color_t::red), 1), {179, 175}, {85, 146});

    // scene.circle(color_t::red, {100, 100}, 55);
    // scene.set_pixel(color_t::black, {100, 100});
    // scene.square(line_t(color_t(color_t::green, 0.5)), {50, 50}, 100, true);
    // scene.square(line_t(color_t(color_t::red), 1, line_param), {100, 100}, 50, false);
    scene.rectangle(line_t(color_t(color_t::red), 1, line_param), {100, 100}, 100, 50, false);
    scene.rectangle(line_t(color_t(color_t::red), 1, line_param), {100, 100}, 100, 50, false, rect_param);

    scene.render();

    scene.generate_image("image.ppm", image_type_t::ppm);

    return 0;
}

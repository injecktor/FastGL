#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.hpp"
#include "fastgl.hpp"

using namespace std;

int main() {
    fastgl_t scene(250, 250);

    line_params_t param;
    param.aa = line_antialiasing::wu;
    param.effect = line_effects::gradient;
    param.extra_color = color_t::blue;

    color_t green(color_t::green, 0.9);
    color_t blue(color_t::blue, 0.45);

    // scene.line(line_t(color_t(color_t::red), 1, param), {20, 20}, {120, 220});
    // scene.line(line_t(color_t(color_t::red), 1, param), {120, 20}, {20, 220});

    // scene.circle(color_t::red, {100, 100}, 55);
    // scene.set_pixel(color_t::black, {100, 100});
    // scene.square(line_t(color_t(color_t::green, 0.5)), {50, 50}, 100, true);
    scene.square(line_t(blue), {100, 100}, 50, true);
    scene.rectangle(line_t(green), {100, 100}, 100, 50, true);

    scene.render();

    printf("green: %08x\n", green.get_hex());
    printf("blue: %08x\n", blue.get_hex());
    printf("Color: %08x\n", scene.get_pixel({120, 120}).get_hex());

    scene.generate_image("image.ppm", image_type_t::ppm);

    return 0;
}

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

    scene.rectangle({color_t::red}, { 100, 100 }, 100, 80, true, rect_param);
    // scene.triangle(line_t(color_t::red), {50, 50}, {100, 100}, {30, 80});

    scene.render();

    scene.generate_image("image.ppm", image_type_t::ppm);

    return 0;
}

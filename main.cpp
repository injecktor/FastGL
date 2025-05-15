#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.hpp"
#include "fastgl.hpp"

#include <X11/Xlib.h>

using namespace std;

int main() {
    quad_params_t quad_params;
    quad_params.use_inner_color = true;
    quad_params.inner_color = color_t::green;

    fastgl_t scene(250, 250);
    scene.quadrangle({color_t::red}, {30, 30}, {50, 100}, {200, 70}, {190, 30}, true, quad_params);
    scene.render();
    scene.generate_image("image.ppm", image_type_t::ppm);

    return 0;
}

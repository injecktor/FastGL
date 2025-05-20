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
    // signed width = 250, height = 250;
    // point3_t point1{30, 60, 10};
    // point3_t point2{30, 30, 10};
    // point3_t point3{60, 30, 10};
    // point3_t point4{60, 60, 10};
    // point3_t point5{60, 30, 30};
    // point3_t point6{60, 60, 30};
    // point3_t point7{30, 30, 30};
    // point3_t point8{30, 60, 30};
    // point3_t points[] = {point1, point2, point3, point4, point5, point6, point7, point8};
    // double d[8];

    // double k = width * cos(50 / 180 * 3.1415926535 / sqrt(2));

    fastgl_t frame(1, 1);
    frame.set_pixel(color_t::red, {1, 1});

    frame.render();
    frame.generate_image("image.png", image_type_t::png);

    return 0;
}

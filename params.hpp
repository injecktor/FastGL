#pragma once

#include "param_values.hpp"
#include "color.hpp"

struct point2_t {
    signed x;
    signed y;
    point2_t(signed _x = 0, signed _y = 0) : x(_x), y(_y){};
};

struct line_params_t {
    unsigned fillness;
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;

    line_params_t() : fillness(line_fillness::solid), aa(line_antialiasing::wu),
        effect(line_effects::none) {};
};

struct circle_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;

    circle_params_t() : aa(circle_antialiasing::wu), effect(circle_effects::none) {};
};

struct rect_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;
    color_t inner_extra_color;
    double rotation;

    rect_params_t() : aa(rect_antialiasing::wu), effect(rect_effects::none) {};
};
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

    line_params_t() : fillness(line_fillness::solid), aa(antialiasing::wu),
        effect(line_effects::none) {};
};

struct circle_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;
    bool use_inner_color;
    color_t inner_color;
    double rotation;

    circle_params_t() : aa(antialiasing::wu), effect(rect_effects::none),
        extra_color(), use_inner_color(false), inner_color(), rotation(0.) {};
};

struct rect_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;
    bool use_inner_color;
    color_t inner_color;
    double rotation;

    rect_params_t() : aa(antialiasing::wu), effect(rect_effects::none),
        extra_color(), use_inner_color(false), inner_color(), rotation(0.) {};
};

struct tri_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;
    bool use_inner_color;
    color_t inner_color;
    double rotation;

    tri_params_t() : aa(antialiasing::wu), effect(tri_effects::none),
        extra_color(), use_inner_color(false), inner_color(), rotation(0.) {};
};

struct quad_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;
    bool use_inner_color;
    color_t inner_color;
    double rotation;

    quad_params_t() : aa(antialiasing::wu), effect(quad_effects::none),
        extra_color(), use_inner_color(false), inner_color(), rotation(0.) {};
};
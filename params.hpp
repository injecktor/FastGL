#pragma once

#include "color.hpp"

struct point2_t {
    unsigned x;
    unsigned y;
    point2_t(unsigned _x = 0, unsigned _y = 0) : x(_x), y(_y){};
};

namespace effects {
    constexpr unsigned none = 0;
    constexpr unsigned gradient = 1;
    constexpr unsigned faded = 2;
};

namespace line_fillness {
    constexpr unsigned solid = 0;
    constexpr unsigned dotted = 1;
    constexpr unsigned dashed = 2;
    constexpr unsigned dotdash = 3;
};

namespace line_antialiasing {
    constexpr unsigned none = 0;
    constexpr unsigned wu = 1;
};

struct line_params_t {
    unsigned fillness;
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;

    line_params_t() : fillness(line_fillness::solid), aa(line_antialiasing::wu),
        effect(effects::none), extra_color() {};
};

struct circle_params_t {
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;

    circle_params_t() : aa(line_antialiasing::wu), effect(effects::none), extra_color() {};
};
#ifndef LINE_H
#define LINE_H

#include "color.hpp"

namespace line_fillness {
    constexpr unsigned solid = 0;
    constexpr unsigned dotted = 1;
    constexpr unsigned dashed = 2;
    constexpr unsigned dotdash = 3;
};

namespace line_antialiasing {
    constexpr unsigned none = 0;
    constexpr unsigned wu = 1;
}

namespace line_effects {
    constexpr unsigned none = 0;
    constexpr unsigned gradient = 1;
    constexpr unsigned faded = 2;
}

struct line_params_t {
    unsigned fillness;
    unsigned aa;
    // effects can be united
    unsigned effect;
    color_t extra_color;

    line_params_t() : fillness(line_fillness::solid), aa(line_antialiasing::wu),
        effect(line_effects::none), extra_color() {};
};

class line_t {
public:
    line_t(color_t color, unsigned thickness = 1, line_params_t params = line_params_t());

    color_t& color();
    unsigned& thickness();
    line_params_t& params();

    // get color according to effects
    /// \param ratio Point on the line where 0. - start, 1. - end
    color_t get_current_color(double ratio);
    uint8_t get_next_mask_bit();

    void set_bit_offset_start();

private:
    color_t m_color;
    unsigned m_thickness;
    line_params_t m_params;
	
    uint16_t m_bit_mask;
    uint8_t m_bit_offset = 0;
};

#endif//LINE_H
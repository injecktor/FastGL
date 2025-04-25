#ifndef LINE_H
#define LINE_H

#include "color.h"

struct line_params_t {
    enum fillness_t {
        solid,
        dotted,
        dashed,
        dotdash,
    };
    enum antialiasing_t {
        none,
        wu
    };

    fillness_t fillness;
    antialiasing_t aa;

    line_params_t() : fillness(fillness_t::solid), aa(antialiasing_t::wu) {};
};

class line_t {
public:
    line_t(color_t color, unsigned thickness = 1, line_params_t params = line_params_t());

    color_t& color();
    unsigned& thickness();
    line_params_t& params();

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
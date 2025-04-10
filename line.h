#ifndef LINE_H
#define LINE_H

#include "color.h"

class line_t {
public:
    line_t(color_t color, unsigned thickness = 1, type_t type = type_t::solid, 
        antialiasing_t aa = antialiasing_t::wu);

    color_t& color();
    unsigned& thickness();
    antialiasing_t& get_antialiasing();

    [[nodiscard]] uint8_t get_next_mask_bit();

    void set_bit_offset_start();

private:
    color_t m_color;
    unsigned m_thickness;
    antialiasing_t m_aa;
	
    uint16_t m_bit_mask;
    uint8_t m_bit_offset = 0;
};

#endif//LINE_H
#ifndef LINE_H
#define LINE_H

#include "params.hpp"

class line_t {
public:
    line_t() {};
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
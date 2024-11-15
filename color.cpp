#include "color.h"

line_t::line_t(color_t color, line_type_t line_type, uint32_t line_width) : m_color(color),
                                                                            m_line_width(line_width) {
    switch (line_type) {
        case line_type_t::solid: {
            m_bit_mask = 0b11111111'11111111;
        } break;
        case line_type_t::dotted: {
            m_bit_mask = 0b11001100'11001100;
        } break;
        case line_type_t::dashed: {
            m_bit_mask = 0b11110000'11110000;
        } break;
        case line_type_t::dotdash: {
            m_bit_mask = 0b11110000'01100000;
        } break;
        default: {
            ASSERT(false, "Unknown line type");
        } break;
    }
}

uint8_t line_t::get_next_mask_bit() {
    const uint8_t mask_bit = (1 << m_bit_offset) & m_bit_mask;
    m_bit_offset = m_bit_offset == 15 ? 0 : m_bit_offset + 1;
    return mask_bit;
}

void line_t::set_bit_offset_start() {
    m_bit_offset = 0;
}

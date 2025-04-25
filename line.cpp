#include "line.h"

line_t::line_t(color_t color, unsigned thickness, line_params_t params)
	: m_color(color), m_thickness(thickness), m_params(params) {
    switch (m_params.fillness) {
        case line_fillness::solid: {
            m_bit_mask = 0b11111111'11111111;
        } break;
        case line_fillness::dotted: {
            m_bit_mask = 0b11001100'11001100;
        } break;
        case line_fillness::dashed: {
            m_bit_mask = 0b11110000'11110000;
        } break;
        case line_fillness::dotdash: {
            m_bit_mask = 0b11110000'01100000;
        } break;
        default: {
            ASSERT(false, "Unknown line type");
        } break;
    }
}

color_t& line_t::color() {
    return m_color;
}

unsigned& line_t::thickness() {
    return m_thickness;
}

line_params_t& line_t::params(){
    return m_params;
}

color_t line_t::get_current_color(double ratio) {
    if (m_params.effect == line_effects::none) {
        return m_color;
    }
    color_t current_color, extra_color = m_params.extra_color;
    if (m_params.effect & line_effects::gradient) {
        auto ca = static_cast<int32_t>(extra_color.a()) - static_cast<int32_t>(m_color.a());
        auto cr = static_cast<int32_t>(extra_color.r()) - static_cast<int32_t>(m_color.r());
        auto cg = static_cast<int32_t>(extra_color.g()) - static_cast<int32_t>(m_color.g());
        auto cb = static_cast<int32_t>(extra_color.b()) - static_cast<int32_t>(m_color.b());
        current_color.a() = static_cast<uint8_t>(m_color.a() + ratio * ca);
        current_color.r() = static_cast<uint8_t>(m_color.r() + ratio * cr);
        current_color.g() = static_cast<uint8_t>(m_color.g() + ratio * cg);
        current_color.b() = static_cast<uint8_t>(m_color.b() + ratio * cb);
    }
    return current_color;
}

uint8_t line_t::get_next_mask_bit() {
    const uint8_t mask_bit = (1 << m_bit_offset) & m_bit_mask;
    m_bit_offset = m_bit_offset == 15 ? 0 : m_bit_offset + 1;
    return mask_bit;
}

void line_t::set_bit_offset_start() {
    m_bit_offset = 0;
}
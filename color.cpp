#include "color.h"

color_t::color_t() {
    set(0xff000000);
}

color_t::color_t(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
    : a(a), r(r), g(g), b(b) {

}

color_t::color_t(const unsigned hex) {
    set(hex);
}

color_t::color_t(unsigned hex, float alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

color_t::color_t(unsigned hex, double alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

void color_t::set(const unsigned hex) {
    a = (hex >> 24) & 0xff;
    r = (hex >> 16) & 0xff;
    g = (hex >> 8) & 0xff;
    b = hex & 0xff;
}
void color_t::set(unsigned hex, float alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}
void color_t::set(unsigned hex, double alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

[[nodiscard]] uint8_t color_t::get_a() const {
    return a;
}
[[nodiscard]] uint8_t color_t::get_r() const {
    return r;
}
[[nodiscard]] uint8_t color_t::get_g() const {
    return g;
}
[[nodiscard]] uint8_t color_t::get_b() const {
    return b;
}
[[nodiscard]] uint32_t color_t::get_hex() const {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

[[nodiscard]] color_t color_t::apply_alpha(color_t color_with_alpha, color_t background) {
    signed r_diff = static_cast<signed>(color_with_alpha.get_r()) - static_cast<signed>(background.get_r());
    signed g_diff = static_cast<signed>(color_with_alpha.get_g()) - static_cast<signed>(background.get_g());
    signed b_diff = static_cast<signed>(color_with_alpha.get_b()) - static_cast<signed>(background.get_b());
    auto alpha = static_cast<float>(color_with_alpha.get_a());
    alpha /= 0xff;
    alpha = 1 - alpha;
    return color_t(0xff, static_cast<uint8_t>(static_cast<signed>(color_with_alpha.get_r()) - static_cast<signed>(alpha * r_diff)),
        static_cast<uint8_t>(static_cast<signed>(color_with_alpha.get_g()) - static_cast<signed>(alpha * g_diff)),
        static_cast<uint8_t>(static_cast<signed>(color_with_alpha.get_b()) - static_cast<signed>(alpha * b_diff)));
}


line_t::line_t(color_t color, unsigned width, type_t type) : m_color(color),
                                                                            m_width(width) {
    switch (type) {
        case type_t::solid: {
            m_bit_mask = 0b11111111'11111111;
        } break;
        case type_t::dotted: {
            m_bit_mask = 0b11001100'11001100;
        } break;
        case type_t::dashed: {
            m_bit_mask = 0b11110000'11110000;
        } break;
        case type_t::dotdash: {
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

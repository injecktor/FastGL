#include "color.hpp"

color_t::color_t() {
    set(0xffffffff);
}

color_t::color_t(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
    : m_a(a), m_r(r), m_g(g), m_b(b) {
}

color_t::color_t(unsigned hex) {
    set(hex);
}

color_t::color_t(color_t color, double alpha) {
    set(color.get_hex() & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

color_t::color_t(unsigned hex, double alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

void color_t::set(unsigned hex) {
    m_a = (hex >> 24) & 0xff;
    m_r = (hex >> 16) & 0xff;
    m_g = (hex >> 8) & 0xff;
    m_b = hex & 0xff;
}
void color_t::set(unsigned hex, double alpha) {
    alpha = math_tools::border(alpha, 0., 1.);
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

uint8_t& color_t::a() {
    return m_a;
}
uint8_t& color_t::r() {
    return m_r;
}
uint8_t& color_t::g() {
    return m_g;
}
uint8_t& color_t::b() {
    return m_b;
}
uint32_t color_t::get_hex() const {
    return (m_a << 24) | (m_r << 16) | (m_g << 8) | m_b;
}

double color_t::get_alpha() const {
    return static_cast<double>(m_a) / 0xff;
}

void color_t::set_alpha(double alpha) {
    alpha = math_tools::border(alpha, 0., 1.);
    m_a = static_cast<uint8_t>(alpha * 0xff);
}

color_t color_t::alpha_to_color(color_t color_with_alpha, color_t background) {
    signed r_diff = static_cast<signed>(color_with_alpha.r()) - static_cast<signed>(background.r());
    signed g_diff = static_cast<signed>(color_with_alpha.g()) - static_cast<signed>(background.g());
    signed b_diff = static_cast<signed>(color_with_alpha.b()) - static_cast<signed>(background.b());
    auto alpha = static_cast<float>(color_with_alpha.a());
    alpha /= 0xff;
    alpha = 1 - alpha;
    return color_t(0xff, static_cast<uint8_t>(static_cast<signed>(color_with_alpha.r()) - static_cast<signed>(alpha * r_diff)),
        static_cast<uint8_t>(static_cast<signed>(color_with_alpha.g()) - static_cast<signed>(alpha * g_diff)),
        static_cast<uint8_t>(static_cast<signed>(color_with_alpha.b()) - static_cast<signed>(alpha * b_diff)));
}

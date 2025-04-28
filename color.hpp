#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>

#include "logging.hpp"
#include "math_tools/math_tools.hpp"

class color_t {
public:
    enum : uint32_t {
        black = 0xff000000,
        white = 0xffffffff,
        red = 0xffff0000,
        green = 0xff00ff00,
        blue = 0xff0000ff
    };

    color_t();
    color_t(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    color_t(unsigned hex);
    color_t(color_t color, double alpha);
    color_t(unsigned hex, double alpha);

    void set(unsigned hex);
    void set(unsigned hex, double alpha);

    uint8_t& a();
    uint8_t& r();
    uint8_t& g();
    uint8_t& b();
    unsigned get_hex() const;
    double get_alpha() const;
    void set_alpha(double alpha);

    static color_t alpha_to_color(color_t color_with_alpha, color_t background);

private:
    uint8_t m_a = 1, m_r = 0, m_g = 0, m_b = 0;
};

#endif //COLOR_H

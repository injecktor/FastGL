#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>

#include "logging.h"

class color_t {
public:
    enum : uint32_t {
        black = 0xFF000000,
        white = 0xFFFFFFFF,
        red = 0xFFFF0000,
        green = 0xFF00FF00,
        blue = 0xFF0000FF
    };

    explicit color_t(const unsigned hex) {
        init(hex);
    }
    explicit color_t(unsigned hex, float alpha) {
        init(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
    }
    explicit color_t(unsigned hex, double alpha) {
        init(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
    }

    [[nodiscard]] uint8_t get_a() const {
        return a;
    }
    [[nodiscard]] uint8_t get_r() const {
        return r;
    }
    [[nodiscard]] uint8_t get_g() const {
        return g;
    }
    [[nodiscard]] uint8_t get_b() const {
        return b;
    }
    [[nodiscard]] uint32_t get_hex() const {
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

private:
    uint8_t a = 1, r = 0, g = 0, b = 0;

    void init(const unsigned hex) {
        a = (hex >> 24) & 0xff;
        r = (hex >> 16) & 0xff;
        g = (hex >> 8) & 0xff;
        b = hex & 0xff;
    }
};

class line_t {
public:
    enum class type_t {
        solid,
        dotted,
        dashed,
        dotdash
    };

    line_t(color_t color, unsigned width, type_t type);

    [[nodiscard]] uint8_t get_next_mask_bit();

    void set_bit_offset_start();

private:
    color_t m_color;
    unsigned m_width;
    uint16_t m_bit_mask;
    uint8_t m_bit_offset = 0;
};

#endif //COLOR_H

#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>

#include "logging.h"

class color_t {
public:
    enum : uint32_t {
        black = 0xff000000,
        white = 0xffffffff,
        red = 0xffff0000,
        green = 0xff00ff00,
        blue = 0xff0000ff
    };

    explicit color_t();
    explicit color_t(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    explicit color_t(const unsigned hex);
    explicit color_t(unsigned hex, float alpha);
    explicit color_t(unsigned hex, double alpha);

    void set(const unsigned hex);
    void set(unsigned hex, float alpha);
    void set(unsigned hex, double alpha);

    [[nodiscard]] uint8_t get_a() const;
    [[nodiscard]] uint8_t get_r() const;
    [[nodiscard]] uint8_t get_g() const;
    [[nodiscard]] uint8_t get_b() const;
    [[nodiscard]] unsigned get_hex() const;

    [[nodiscard]] static color_t apply_alpha(color_t color_with_alpha, color_t background);

private:
    uint8_t a = 1, r = 0, g = 0, b = 0;
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

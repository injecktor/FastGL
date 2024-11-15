#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>

#include "logging.h"

using namespace std;

class color_t {
public:
    enum colors : uint8_t {
        black,
        white,
        red,
        green,
        blue
    };

    explicit color_t(const colors color) {
        switch (color) {
            case black: {
                init(0x000000);
            } break;
            case white: {
                init(0xFFFFFF);
            } break;
            case red: {
                init(0xFF0000);
            } break;
            case green: {
                init(0x00FF00);
            } break;
            case blue: {
                init(0x0000FF);
            } break;
        }
    }

    explicit color_t(const uint32_t hex) {
        init(hex);
    }

    [[nodiscard]] uint8_t getR() const {
        return r;
    }

    [[nodiscard]] uint8_t getG() const {
        return g;
    }

    [[nodiscard]] uint8_t getB() const {
        return b;
    }


private:
    uint8_t r = 0, g = 0, b = 0;

    void init(const uint32_t hex) {
        r = (hex >> 16) & 255;
        g = (hex >> 8) & 255;
        b = hex & 255;
    }
};

class line_t {
    enum class line_type_t {
        solid,
        dotted,
        dashed,
        dotdash
    };

    color_t m_color;
    uint32_t m_line_width;
    uint16_t m_bit_mask;
    uint8_t m_bit_offset = 0;

public:
    line_t(color_t color, line_type_t line_type, uint32_t line_width);

    [[nodiscard]] uint8_t get_next_mask_bit();

    void set_bit_offset_start();
};

#endif //COLOR_H

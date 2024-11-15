#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

class color_t {
public:
    explicit color_t(const uint32_t hex) {
        r = (hex >> 16) & 255;
        g = (hex >> 8) & 255;
        b = hex & 255;
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
    uint8_t r, g, b;
};

#endif //COLOR_H

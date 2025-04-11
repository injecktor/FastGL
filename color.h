#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>

#include "logging.h"

enum class type_t {
    solid,
    dotted,
    dashed,
    dotdash
};
enum class antialiasing_t {
    none,
    wu
};

struct point2_t {
    unsigned x;
    unsigned y;
    point2_t(unsigned _x = 0, unsigned _y = 0) : x(_x), y(_y){};
};

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
    explicit color_t(unsigned hex);
    explicit color_t(color_t color, double alpha);
    explicit color_t(unsigned hex, double alpha);

    void set(unsigned hex);
    void set(unsigned hex, double alpha);

    uint8_t& a();
    uint8_t& r();
    uint8_t& g();
    uint8_t& b();
    [[nodiscard]] unsigned get_hex() const;

    [[nodiscard]] static color_t apply_alpha(color_t color_with_alpha, color_t background);

private:
    uint8_t m_a = 1, m_r = 0, m_g = 0, m_b = 0;
};

#endif //COLOR_H

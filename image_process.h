#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include <cmath>

#include "ppm.h"
#include "color.h"

class image_process_t {
public:
    enum image_type {
        ppm,
        png
    };
    explicit image_process_t(unsigned width, unsigned height);
    void set_pixel(color_t color, unsigned x, unsigned y);
    void set_background(color_t color);
    void line(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void set_border(color_t color, line_t line_type);
    void generate_image(image_type image_type);

private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_resolution;
    // if 1 it's background otherwise it's not
    vector<color_t> m_image_buffer;
    vector<uint8_t> m_background_bit_mask;
};

#endif //IMAGE_PROCESS_H

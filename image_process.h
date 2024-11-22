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

    void circle(color_t color, unsigned x, unsigned y, unsigned radius, bool fill = true);

    void line(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    void border(color_t color, line_t line_type);

    void square(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned length);

    void rectangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    void triangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned x3,
                  unsigned y3);

    /// \param file_name without extension
    void generate_image(const string &file_name, image_type image_type) const;

private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_resolution;
    // if 1 it's background otherwise it's not
    vector<color_t> m_image_buffer;
    vector<uint8_t> m_background_bit_mask;
};

#endif //IMAGE_PROCESS_H

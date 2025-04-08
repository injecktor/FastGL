#ifndef FRAME_PROCESS_H
#define FRAME_PROCESS_H

#include <cmath>
#include <memory>

#include "include_graphic_core.h"
#include "cge_ppm.h"
#include "cge_png.h"
#include "color.h"

#define MAP(value, from_lower_limit, from_upper_limit, to_lower_limit, to_upper_limit) \
    (to_lower_limit + (value - from_lower_limit) * (to_upper_limit - to_lower_limit) / (from_upper_limit - from_lower_limit))

class frame_process_t {
public:
    enum class image_type {
        ppm,
        png
    };

    explicit frame_process_t(unsigned width, unsigned height);

    void set_pixel(color_t color, unsigned x, unsigned y);
    void set_background(color_t color);
    void circle(color_t color, unsigned x, unsigned y, unsigned radius, bool fill = true);
    void line(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void border(color_t color, line_t line_type);
    void square(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned length);
    void rectangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void triangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned x3,
                  unsigned y3);

    // file_name without extension
    void generate_image(const std::string &file_name, image_type image_type) const;

    void generate_frame();

private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_resolution;
    // if 1 it's background otherwise it's not
    std::vector<color_t> m_image_buffer;
    std::vector<uint8_t> m_background_bit_mask;

    std::shared_ptr<frame_generator_t> frame_gen;

    static bool is_in_circle(signed x, signed y, unsigned radius);
};

#endif //FRAME_PROCESS_H

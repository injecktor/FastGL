#ifndef FRAME_PROCESS_H
#define FRAME_PROCESS_H

#include <cmath>
#include <memory>
#include <string>

#include "color.hpp"
#include "line.hpp"
#include "image_formats/gl_png.hpp"
#include "image_formats/gl_ppm.hpp"

enum class image_type_t {
    ppm = 0,
    png = 1,
};

class draw_process_t {
public:
    draw_process_t() = delete;
    draw_process_t(unsigned width, unsigned height);

    void set_pixel(color_t color, point2_t point, bool force = false);
    void clear_pixel(point2_t point);
    void set_background(color_t color);
    void circle(color_t color, point2_t center, unsigned radius, bool fill = true);
    void line(line_t line, point2_t start, point2_t end);
    // void border(color_t color, line_t line_type);
    void square(line_t line, point2_t point, unsigned length, bool fill = true);
    void rectangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void triangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned x3,
                  unsigned y3);

    // file_name without extension
    void generate_image(const std::string &file_name, image_type_t image_type) const;

private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_resolution;

    color_t m_background_color;
    std::vector<color_t> m_frame_buffer;
    std::vector<color_t> m_last_color;
    // if 1 it's background otherwise it's not
    std::vector<uint8_t> m_background_bit_mask;

    static bool is_in_circle(signed x, signed y, unsigned radius);
    void alpha_to_color();
};

#endif //FRAME_PROCESS_H

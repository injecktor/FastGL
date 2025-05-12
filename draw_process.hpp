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
    enum flag_t {
        none = -1,
        background,
        current
    };

public:
    draw_process_t() = delete;
    draw_process_t(signed width, signed height);

    void set_pixel(color_t color, point2_t point, bool force = false, bool mark_pixel = false);
    color_t get_pixel(point2_t point);
    void clear_pixel(point2_t point);
    void set_background(color_t color);
    void clear();
    void circle(color_t color, point2_t center, unsigned radius, bool fill = true);
    void line(line_t line, point2_t start, point2_t end, bool mark_pixel = false, bool include_borders = true);
    // void border(color_t color, line_t line_type);
    void rectangle(line_t line, point2_t point, unsigned width, unsigned height, bool fill = true, rect_params_t rect_params = rect_params_t());
    void triangle(line_t line, point2_t point1, point2_t point2, point2_t point3, bool fill = true);

    // file_name without extension
    void generate_image(const std::string &file_name, image_type_t image_type) const;
    void render();

    const std::vector<color_t>& get_frame_buffer();

private:
    signed m_width;
    signed m_height;
    unsigned m_resolution;

    color_t m_background_color;
    std::vector<color_t> m_frame_buffer;

    // 0 bit = is background
    // 1 bit = is current figure
    std::vector<uint8_t> m_flags;

    inline bool check_flag(flag_t flag, unsigned index);
    inline bool check_flag(flag_t flag, point2_t point);
    inline void set_flag(flag_t flag, unsigned index, bool value);
    void alpha_to_color();
};

#endif //FRAME_PROCESS_H

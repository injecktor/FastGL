#ifndef DRAW_PROCESS_H
#define DRAW_PROCESS_H

#include <cmath>
#include <memory>
#include <string>
#include <array>

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
        flag_none = -1,
        flag_background,
        flag_current
    };

    enum draw_type_t {
        draw_color = 1,
        draw_flag = 2,
        draw_color_and_flag = draw_color | draw_flag
    };

    enum line_border_t {
        border_none = 0,
        border_start = 1,
        border_end = 2,
        border_start_and_end = border_start | border_end
    };

public:
    draw_process_t() = delete;
    draw_process_t(signed width, signed height);

    void set_pixel(color_t color, point2_t point, bool force = false, draw_type_t draw_type = draw_type_t::draw_color);
    color_t get_pixel(point2_t point);
    void clear_pixel(point2_t point);
    void set_background(color_t color);
    void clear();
    void line(line_t line, point2_t start, point2_t end, line_border_t line_border = line_border_t::border_start_and_end, 
        draw_type_t draw_type = draw_type_t::draw_color);
    void circle(color_t color, point2_t center, unsigned radius, bool fill = true);
    void rectangle(line_t line, point2_t point, unsigned width, unsigned height, bool fill = true, 
        rect_params_t rect_params = rect_params_t());
    void triangle(line_t line, point2_t point1, point2_t point2, point2_t point3, bool fill = true,
        tri_params_t tri_params = tri_params_t());
    void quadrangle(line_t line, point2_t point1, point2_t point2, point2_t point3, point2_t point4, 
        bool fill = true, quad_params_t quad_params = quad_params_t());

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
    inline void set_flag(flag_t flag, point2_t point, bool value);
    inline bool is_in_figure(signed x, signed y, signed x_min, signed x_max, signed y_min, signed y_max);
    inline std::array<signed, 4> find_x_y_min_max(std::vector<point2_t> points);
    inline void clear_flag_in_area(point2_t point1, point2_t point2);
    void alpha_to_color();
};

#endif //DRAW_PROCESS_H

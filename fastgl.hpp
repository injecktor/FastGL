#pragma once

#include "draw_process.hpp"

class fastgl_t {
public:
    fastgl_t(unsigned width, unsigned height);

    void set_pixel(color_t color, point2_t point, bool force = false);
    void clear_pixel(point2_t point);
    void set_background(color_t color);
    void circle(color_t color, point2_t center, unsigned radius, bool fill = true);
    void line(line_t line, point2_t start, point2_t end);
    // void border(color_t color, line_t line_type);
    void square(line_t line, point2_t point, unsigned length, bool fill = true);
    // void rectangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    // void triangle(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned x3,
                //   unsigned y3);

    void render();
    
    void generate_image(const std::string &file_name, image_type_t image_type) const;
    
private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_resolution;

    draw_process_t draw_process;

    enum action_t {
        act_none,
        act_set_pixel,
        act_clear_pixel,
        act_set_background,
        act_circle,
        act_line,
        act_square,
        act_rectangle,
        act_triangle
    };

    struct action_params_t {
        action_t action = act_none;
        std::vector<point2_t> points;
        std::vector<color_t> colors;
        std::vector<line_t> lines;
        std::vector<unsigned> unsigneds;
        std::vector<bool> bools;
    };

    std::vector<action_params_t> actions;
};
#pragma once

#include "draw_process.hpp"

class fastgl_t {
public:
    fastgl_t(signed width, signed height);

    void set_pixel(color_t color, point2_t point, bool force = false);
    color_t get_pixel(point2_t point);
    void clear_pixel(point2_t point);
    void set_background(color_t color);
    void circle(color_t color, point2_t center, unsigned radius, bool fill = true);
    void line(line_t line, point2_t start, point2_t end);
    // void border(color_t color, line_t line_type);
    void square(line_t line, point2_t point, unsigned length, bool fill = true);
    void rectangle(line_t line, point2_t point, unsigned width, unsigned height, bool fill = true, 
        rect_params_t rect_params = rect_params_t());
    void triangle(line_t line, point2_t point1, point2_t point2, point2_t point3, bool fill = true);

    void render();
    
    void generate_image(const std::string &file_name, image_type_t image_type) const;
    
private:
    signed m_width;
    signed m_height;

    draw_process_t draw_process;

    enum action_t {
        act_none,
        act_set_pixel,
        act_clear_pixel,
        act_set_background,
        act_circle,
        act_line,
        act_rectangle,
        act_triangle
    };

    struct action_params_t {
        action_t action;
        point2_t points[3];
        color_t colors[3];
        line_t lines;
        unsigned unsigneds[3];
        bool bools[3];
        rect_params_t rect_params;
        action_params_t() : action(act_none) {};
    };

    std::vector<action_params_t> actions;
};
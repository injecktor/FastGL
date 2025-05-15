#include "fastgl.hpp"

fastgl_t::fastgl_t(signed width, signed height) : 
    m_width(width),
    m_height(height),
    draw_process(m_width, m_height) {
}

void fastgl_t::set_pixel(color_t color, point2_t point, bool force) {
    action_params_t params;
    params.action = action_t::act_set_pixel;
    params.colors[0] = color;
    params.points[0] = point;
    params.bools[0] = force;
    actions.push(params);
}

color_t fastgl_t::get_pixel(point2_t point) {
    return draw_process.get_pixel(point);
}

void fastgl_t::clear_pixel(point2_t point) {
    action_params_t params;
    params.action = action_t::act_clear_pixel;
    params.points[0] = point;
    actions.push(params);
}

void fastgl_t::set_background(color_t color) {
    action_params_t params;
    params.action = action_t::act_set_background;
    params.colors[0] = color;
    actions.push(params);
}

void fastgl_t::clear() {
    action_params_t params;
    params.action = action_t::act_clear;
    actions.push(params);
}

void fastgl_t::circle(color_t color, point2_t center, unsigned radius, bool fill) {
    action_params_t params;
    params.action = action_t::act_circle;
    params.colors[0] = color;
    params.points[0] = center;
    params.unsigneds[0] = radius;
    params.bools[0] = fill;
    actions.push(params);
}

void fastgl_t::line(line_t line, point2_t start, point2_t end) {
    action_params_t params;
    params.action = action_t::act_line;
    params.lines = line;
    params.points[0] = start;
    params.points[1] = end;
    actions.push(params);
}

void fastgl_t::square(line_t line, point2_t point, unsigned length, bool fill) {
    rectangle(line, point, length, length, fill);
}

void fastgl_t::rectangle(line_t line, point2_t point, unsigned width, unsigned height, bool fill, rect_params_t rect_params) {
    action_params_t params;
    params.action = action_t::act_rectangle;
    params.lines = line;
    params.points[0] = point;
    params.unsigneds[0] = width;
    params.unsigneds[1] = height;
    params.bools[0] = fill;
    params.rect_params = rect_params;
    actions.push(params);
}

void fastgl_t::triangle(line_t line, point2_t point1, point2_t point2, point2_t point3, bool fill, tri_params_t tri_params) {
    action_params_t params;
    params.action = action_t::act_triangle;
    params.lines = line;
    params.points[0] = point1;
    params.points[1] = point2;
    params.points[2] = point3;
    params.bools[0] = fill;
    params.tri_params = tri_params;
    actions.push(params);
}

void fastgl_t::quadrangle(line_t line, point2_t point1, point2_t point2, point2_t point3, point2_t point4, 
        bool fill) {
    action_params_t params;
    params.action = action_t::act_quadrangle;
    params.lines = line;
    params.points[0] = point1;
    params.points[1] = point2;
    params.points[2] = point3;
    params.points[3] = point4;
    params.bools[0] = fill;
    actions.push(params);
}

void fastgl_t::render() {
    while (actions.size() != 0) {
        auto&& act = actions.front();
        actions.pop();
        switch (act.action) {
            case action_t::act_set_pixel:
                draw_process.set_pixel(act.colors[0], act.points[0], act.bools[0]);
                break;
            case action_t::act_clear_pixel:
                draw_process.clear_pixel(act.points[0]);
                break;
            case action_t::act_set_background:
                draw_process.set_background(act.colors[0]);
                break;
            case action_t::act_clear:
                draw_process.clear();
                break;
            case action_t::act_circle:
                draw_process.circle(act.colors[0], act.points[0], act.unsigneds[0], act.bools[0]);
                break;
            case action_t::act_line:
                draw_process.line(act.lines, act.points[0], act.points[1]);
                break;
            case action_t::act_rectangle:
                draw_process.rectangle(act.lines, act.points[0], act.unsigneds[0], act.unsigneds[1], act.bools[0], act.rect_params);
                break;
            case action_t::act_triangle:
                draw_process.triangle(act.lines, act.points[0], act.points[1], act.points[2], act.bools[0], act.tri_params);
                break;
            case action_t::act_quadrangle:
                draw_process.quadrangle(act.lines, act.points[0], act.points[1], act.points[2], act.points[3], act.bools[0]);
                break;
            default:
                ASSERT(false, "Unknown action");
                break;
        }
    }
}

void fastgl_t::generate_image(const std::string &file_name, image_type_t image_type) const {
    draw_process.generate_image(file_name, image_type);
}
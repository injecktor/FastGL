#include "draw_process.hpp"

draw_process_t::draw_process_t(signed width, signed height) : 
    m_width(width),
    m_height(height),
    m_resolution(width * height),
    m_background_color(color_t::white) {
    m_frame_buffer.reserve(m_resolution);
    m_flags.reserve(m_resolution);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_flags.emplace_back(1);
        m_frame_buffer.emplace_back(m_background_color);
    }
}

void draw_process_t::set_pixel(color_t color, point2_t point, bool force, draw_type_t draw_type) {
    if (point.x >= m_width || point.y >= m_height) return;
    if (point.x < 0 || point.y < 0) return;
    auto index = point.y * m_width + point.x;
    if (draw_type & draw_type_t::draw_color) {
        if (color.a() == 0xff || check_flag(flag_t::flag_background, index) || force) {
            m_frame_buffer[index] = color;
        } else {
            color_t new_color;
            auto prev_color = m_frame_buffer[index];
            auto prev_alpha = prev_color.get_alpha();
            auto alpha = color.get_alpha();
            auto alpha_reciprocal = 1. - alpha;
            new_color.set_alpha(1. - (1. - prev_alpha) * alpha_reciprocal);
            new_color.r() = alpha_reciprocal * prev_color.r() + alpha * color.r();
            new_color.g() = alpha_reciprocal * prev_color.g() + alpha * color.g();
            new_color.b() = alpha_reciprocal * prev_color.b() + alpha * color.b();
            m_frame_buffer[index] = new_color;
        }
        set_flag(flag_t::flag_background, index, false);
    }
    if (draw_type & draw_type_t::draw_flag) {
        set_flag(flag_t::flag_current, index, true);
    }
}

color_t draw_process_t::get_pixel(point2_t point) {
    return m_frame_buffer[point.y * m_width + point.x];
}

void draw_process_t::clear_pixel(point2_t point) {
    if (point.x >= m_width || point.y >= m_height) return;
    auto index = point.y * m_width + point.x;
    m_frame_buffer[index] = m_background_color;
    set_flag(flag_t::flag_background, index, true);
}

void draw_process_t::set_background(color_t color) {
    m_background_color = color;
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (check_flag(flag_t::flag_background, i)) {
            m_frame_buffer[i] = color;
        }
    }
}

void draw_process_t::clear() {
    for (unsigned i = 0; i < m_resolution; ++i) {
        set_flag(flag_t::flag_background, i, true);
        m_frame_buffer[i] = m_background_color;
    }
}

void draw_process_t::circle(color_t color, point2_t center, unsigned radius, bool fill) {
    ASSERT(radius != 0, "Can't draw circle with zero radius");
    unsigned radius_sqr = radius * radius;
    unsigned i_sqr = 0, i_sqr_prev = 0, i_sqr_next = 1;
    unsigned j_sqr = 0, j_sqr_prev = 0, j_sqr_next = 1;
    for (signed i = 0; i <= radius - 1; ++i) {
        for (signed j = 0; j <= radius - 1; ++j) {
            if (i_sqr + j_sqr < radius_sqr && (fill || i == 0 || j == 0
                                                || i_sqr_prev + j_sqr > radius_sqr || i_sqr_next + j_sqr > radius_sqr
                                                || i_sqr + j_sqr_prev > radius_sqr || i_sqr + j_sqr_next > radius_sqr)) {
                set_pixel(color, { center.x + i, center.y + j });
                set_pixel(color, { center.x - i, center.y + j });
                set_pixel(color, { center.x + i, center.y - j });
                set_pixel(color, { center.x - i, center.y - j });
                j_sqr_prev = j_sqr;
                j_sqr = j_sqr_next;
                j_sqr_next += (j << 1) + 3;
            } else {
                break;
            }
        }
        i_sqr_prev = i_sqr;
        i_sqr = i_sqr_next;
        i_sqr_next += (i << 1) + 3;
        j_sqr = 0; 
        j_sqr_prev = 0; 
        j_sqr_next = 1;
    }
}

void draw_process_t::line(line_t line, point2_t start, point2_t end, line_border_t line_border, draw_type_t line_draw_type) {
    double x, y, tangent;
    unsigned last;
    bool along_x, positive;
    const double dx = end.x - start.x;
    const double dy = end.y - start.y;
    double dl;
    unsigned length;
    auto abs_dx = abs(dx);
    auto abs_dy = abs(dy);
    if (abs_dx > abs_dy) {
        x = start.x;
        y = start.y;
        last = end.x;
        tangent = dy / abs_dx;
        along_x = true;
        dl = dx;
    } else {
        x = start.y;
        y = start.x;
        last = end.y;
        tangent = dx / abs_dy;
        along_x = false;
        dl = dy;
    }
    positive = dl > 0;
    dl = abs(dl) + 1;
    length = static_cast<unsigned>(static_cast<signed>(dl));

    double upper_alpha, lower_alpha;
    signed upper_coord, lower_coord;

    auto aa = line.params().aa;
    auto color = line.color();
    auto alpha = color.get_alpha();

    unsigned counter = 0;
    double step = 1 / dl, ratio = 0;
    if (line_border & line_border_t::border_start) {
        y = y + tangent;
        x = positive ? ++x : --x;
        ratio += step;
        counter++;
    }
    if (line_border & line_border_t::border_end) {
        length--;
    }
    while (counter < length) {
        switch (aa) {
            case line_antialiasing::none: {
                upper_coord = round(y);
                lower_coord = upper_coord;  
                upper_alpha = 1.;
                lower_alpha = 1.;
            }
            break;
            case line_antialiasing::wu: {
                upper_coord = ceil(y);
                lower_coord = upper_coord - 1;
                upper_alpha = 1. - (upper_coord - y);
                lower_alpha = 1. - (y - lower_coord);  
            }
            break;
        }
        color_t current_color = line.get_current_color(ratio);
        signed x1, y1, x2, y2;
        if (along_x) {
            x1 = static_cast<signed>(x);
            y1 = upper_coord;
            x2 = x1;
            y2 = lower_coord;
        } else {
            x1 = upper_coord;
            y1 = static_cast<signed>(x);
            x2 = lower_coord;
            y2 = y1;
        }
        set_pixel(color_t(current_color, alpha * upper_alpha), { x1, y1 }, false, line_draw_type);
        if (upper_coord != lower_coord) {
            set_pixel(color_t(current_color, alpha * lower_alpha), { x2, y2 }, false, line_draw_type);
        }
        y = y + tangent;
        x = positive ? ++x : --x;
        ratio += step;
        counter++;
    };
}

void draw_process_t::rectangle(line_t line, point2_t point, unsigned width, unsigned height, bool fill, rect_params_t rect_params) {
    auto sina = sin(rect_params.rotation);
    auto cosa = cos(rect_params.rotation);
    if (width == 0 || height == 0) return;
    signed s_width = width - 1;
    signed s_height = height - 1;
    math_tools::matrix_t<double> rot_mtx(2, 2, {cosa, -sina, sina, cosa});
    math_tools::matrix_t<signed> points(3, 2, { s_width, 0, 0, s_height, s_width, s_height });
    math_tools::matrix_t<signed> start_points(3, 2, {point.x, point.y, point.x, point.y, point.x, point.y});
    points *= rot_mtx;
    points += start_points;
    
    if (fill) {
        draw_process_t::line(line, { point.x, point.y }, { points[0][0], points[0][1] }, 
            line_border_t::border_start_and_end, draw_type_t::draw_flag);
        draw_process_t::line(line, { point.x, point.y }, { points[1][0], points[1][1] }, 
            line_border_t::border_none, draw_type_t::draw_flag);
        draw_process_t::line(line, { points[2][0], points[2][1] }, { points[0][0], points[0][1] }, 
            line_border_t::border_none, draw_type_t::draw_flag);
        draw_process_t::line(line, { points[2][0], points[2][1] }, { points[1][0], points[1][1] }, 
            line_border_t::border_start_and_end, draw_type_t::draw_flag);
        std::array<signed, 4> x_y_min_max;
        x_y_min_max = find_x_y_min_max({ { point.x, point.y }, { points[0][0], points[0][1] }, 
            { points[1][0], points[1][1] }, { points[2][0], points[2][1] } });
        color_t inner_color;
        if (rect_params.use_inner_color) {
            inner_color = rect_params.inner_color;
        } else {
            inner_color = line.color();
        }
        for (signed i = x_y_min_max[0]; i < x_y_min_max[1]; i++) {
            for (signed j = x_y_min_max[2]; j < x_y_min_max[3]; j++) {
                if (is_in_figure(i, j, x_y_min_max[1], x_y_min_max[2], x_y_min_max[3])) {
                    set_pixel(inner_color, { i, j });
                }
            }
        }
        clear_flag_in_area({ x_y_min_max[0], x_y_min_max[2] }, { x_y_min_max[1], x_y_min_max[3] });
    }

    draw_process_t::line(line, { point.x, point.y }, { points[0][0], points[0][1] });
    draw_process_t::line(line, { point.x, point.y }, { points[1][0], points[1][1] }, line_border_t::border_none);
    draw_process_t::line(line, { points[2][0], points[2][1] }, { points[0][0], points[0][1] }, line_border_t::border_none);
    draw_process_t::line(line, { points[2][0], points[2][1] }, { points[1][0], points[1][1] });
}

void draw_process_t::triangle(line_t line, point2_t point1, point2_t point2, point2_t point3, bool fill, 
    tri_params_t tri_params) {
    if (fill) {
        draw_process_t::line(line, point1, point2, line_border_t::border_start_and_end, draw_type_t::draw_flag);
        draw_process_t::line(line, point1, point3, line_border_t::border_end, draw_type_t::draw_flag);
        draw_process_t::line(line, point2, point3, line_border_t::border_none, draw_type_t::draw_flag);
        std::array<signed, 4> x_y_min_max;
        x_y_min_max = find_x_y_min_max({ point1, point2, point3 });
        color_t inner_color;
        if (tri_params.use_inner_color) {
            inner_color = tri_params.inner_color;
        } else {
            inner_color = line.color();
        }
        for (signed i = x_y_min_max[0]; i < x_y_min_max[1]; i++) {
            for (signed j = x_y_min_max[2]; j < x_y_min_max[3]; j++) {
                if (is_in_figure(i, j, x_y_min_max[1], x_y_min_max[2], x_y_min_max[3])) {
                    set_pixel(inner_color, { i, j });
                }
            }
        }
        clear_flag_in_area({ x_y_min_max[0], x_y_min_max[2] }, { x_y_min_max[1], x_y_min_max[3] });
    }
    draw_process_t::line(line, point1, point2);
    draw_process_t::line(line, point1, point3, line_border_t::border_end);
    draw_process_t::line(line, point2, point3, line_border_t::border_none);
}

void draw_process_t::quadrangle(line_t line, point2_t point1, point2_t point2, point2_t point3, point2_t point4, 
    bool fill) {
    if (fill) {
        draw_process_t::line(line, { point1.x, point1.y }, { point2.x, point2.y }, line_border_t::border_start_and_end,
            draw_type_t::draw_flag);
        draw_process_t::line(line, { point2.x, point2.y }, { point3.x, point3.y }, line_border_t::border_end, 
            draw_type_t::draw_flag);
        draw_process_t::line(line, { point3.x, point3.y }, { point4.x, point4.y }, line_border_t::border_end, 
            draw_type_t::draw_flag);
        draw_process_t::line(line, { point1.x, point1.y }, { point4.x, point4.y }, line_border_t::border_none, 
            draw_type_t::draw_flag);
        std::array<signed, 4> x_y_min_max;
        x_y_min_max = find_x_y_min_max({ point1, point2, point3, point4 });
        color_t inner_color;
        // if (tri_params.use_inner_color) {
        //     inner_color = tri_params.inner_color;
        // } else {
            inner_color = line.color();
        // }
        for (signed i = x_y_min_max[0]; i < x_y_min_max[1]; i++) {
            for (signed j = x_y_min_max[2]; j < x_y_min_max[3]; j++) {
                if (is_in_figure(i, j, x_y_min_max[1], x_y_min_max[2], x_y_min_max[3])) {
                    set_pixel(inner_color, { i, j });
                }
            }
        }
        clear_flag_in_area({ x_y_min_max[0], x_y_min_max[2] }, { x_y_min_max[1], x_y_min_max[3] });
    }
    draw_process_t::line(line, { point1.x, point1.y }, { point2.x, point2.y });
    draw_process_t::line(line, { point2.x, point2.y }, { point3.x, point3.y }, line_border_t::border_end);
    draw_process_t::line(line, { point3.x, point3.y }, { point4.x, point4.y }, line_border_t::border_end);
    draw_process_t::line(line, { point1.x, point1.y }, { point4.x, point4.y }, line_border_t::border_none);
}

void draw_process_t::generate_image(const std::string &file_name, image_type_t image_type) const {
    std::shared_ptr<image_format_t> img_gen;
    std::ofstream file;
    switch (image_type) {
        case image_type_t::ppm: {
            img_gen = std::make_shared<ppm_t>(&file, m_width, m_height);
        }
        break;
        case image_type_t::png: {
            ASSERT(false, "Not implemented");
        }
        break;
        default: {
            ASSERT(false, "Unknown image format");
        }
        break;
    }
    file.open("images/" + file_name, std::ofstream::out | std::ofstream::binary);
    ASSERT(file.is_open(), "Could not open file");
    img_gen->init();
    img_gen->generate(m_frame_buffer, m_background_color);
    file.close();
}

void draw_process_t::render() {
}

const std::vector<color_t>& draw_process_t::get_frame_buffer() {
    return m_frame_buffer;
}

inline bool draw_process_t::check_flag(flag_t flag, unsigned index) {
    return m_flags[index] & (1 << flag);
}

inline bool draw_process_t::check_flag(flag_t flag, point2_t point) {
    return m_flags[point.y * m_width + point.x] & (1 << flag);
}

inline void draw_process_t::set_flag(flag_t flag, unsigned index, bool value) {
    if (value) {
        m_flags[index] |= (1 << flag);
    } else {
        m_flags[index] &= ~(1 << flag);
    }
}

inline void draw_process_t::set_flag(flag_t flag, point2_t point, bool value) {
    set_flag(flag, point.y * m_width + point.x, value);
}

inline bool draw_process_t::is_in_figure(signed x, signed y, signed x_max, signed y_min, signed y_max) {
    signed step = y + 1;
    while (step <= y_max && !check_flag(flag_t::flag_current, { x, step })) step++;
    if (step > y_max) return false;
    step = y - 1;
    while (step >= y_min && !check_flag(flag_t::flag_current, { x, step })) step--;
    if (step < y_min) return false;
    step = x + 1;
    while (step <= x_max && !check_flag(flag_t::flag_current, { step, y })) step++;
    if (step > x_max) return false;
    return true;
}

inline std::array<signed, 4> draw_process_t::find_x_y_min_max(std::vector<point2_t> points) {
    if (points.size() == 0) return {0, 0, 0, 0};
    signed x_min = points[0].x, x_max = points[0].x, y_min = points[0].y, y_max = points[0].y;
    size_t points_size = points.size() - 1;
    for (auto&& point : points) {
        if (point.x < x_min) {
            x_min = point.x;
        }
        if (point.x > x_max) {
            x_max = point.x;
        }
        if (point.y < y_min) {
            y_min = point.y;
        }
        if (point.y > y_max) {
            y_max = point.y;
        }
    }
    
    return { x_min, x_max, y_min, y_max };
}

inline void draw_process_t::clear_flag_in_area(point2_t point1, point2_t point2) {
    for (signed i = point1.x; i < point2.x; i++) {
        for (signed j =  point1.y; j < point2.y; j++) {
            set_flag(flag_t::flag_current, { i, j }, false);
        }
    }
}

void draw_process_t::alpha_to_color() {
    for (signed y = 0; y < m_height; y++) {
        for (signed x = 0; x < m_width; x++) {
            set_pixel(color_t::alpha_to_color(m_frame_buffer[y * m_width + x], m_background_color), { x, y });
        }
    }
}
